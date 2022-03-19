#include "stdafx.h"
#include "client/core/window.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"
#include "client/renderer/rootsignature/graphics_super_root_signature.h"

#include "client/renderer/renderlevel/opaque_render_level.h"
#include "client/renderer/renderlevel/deferred_render_level.h"
#include "client/renderer/renderlevel/final_view_render_level.h"
#include "client/renderer/renderlevel/ui_render_level.h"

#include "client/renderer/shader/opaque_mesh_shader.h"	
#include "client/renderer/shader/box_shape_shader.h"
#include "client/renderer/shader/deferred_shader.h"
#include "client/renderer/shader/main_camera_ui_shader.h"
#include "client/renderer/shader/ui_shader.h"
#include "client/renderer/shader/texture_billboard_shader.h"
#include "client/renderer/shader/material_billboard_shader.h"
#include "client/renderer/shader/widget_shader.h"

#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/core/camera_manager.h"

#include "client/object/component/core/render_component.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/component/render/shape_component.h"
#include "client/object/component/render/billboard_component.h"
#include "client/object/component/render/widget_component.h"
#include "client/object/component/util/camera_component.h"

namespace client_fw
{
	RenderSystem* Render::s_render_system = nullptr;

	RenderSystem::RenderSystem(const WPtr<Window>& window)
		: m_window(window)
	{
		Render::s_render_system = this;
		m_graphics_super_root_signature = CreateSPtr<GraphicsSuperRootSignature>();

		m_render_asset_manager = CreateUPtr<RenderResourceManager>();
		m_camera_manager = CreateUPtr<CameraManager>();
	}

	RenderSystem::~RenderSystem()
	{
	}

	bool RenderSystem::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		m_device = device;

		bool ret = m_graphics_super_root_signature->Initialize(device, command_list);

		ret &= RegisterGraphicsRenderLevel<OpaqueRenderLevel>(eRenderLevelType::kOpaque);
		ret &= RegisterGraphicsRenderLevel<DeferredRenderLevel>(eRenderLevelType::kDeferred);
		ret &= RegisterGraphicsRenderLevel<FinalViewRenderLevel>(eRenderLevelType::kFinalView);
		ret &= RegisterGraphicsRenderLevel<UIRenderLevel>(eRenderLevelType::kUI);
		ret &= RegisterGraphicsShader<OpaqueMeshShader>("opaque mesh", eRenderLevelType::kOpaque);
		ret &= RegisterGraphicsShader<BoxShapeShader>("shape box", eRenderLevelType::kOpaque);
		ret &= RegisterGraphicsShader<DeferredShader>("deferred", eRenderLevelType::kDeferred);
		ret &= RegisterGraphicsShader<MainCameraUIShader>("main camera ui", eRenderLevelType::kFinalView);
		ret &= RegisterGraphicsShader<UIShader>("ui", eRenderLevelType::kUI);
		ret &= RegisterGraphicsShader<TextureBillboardShader>("texture billboard", eRenderLevelType::kOpaque);
		ret &= RegisterGraphicsShader<OpaqueMaterialBillboardShader>("opaque material billboard", eRenderLevelType::kOpaque);
		ret &= RegisterGraphicsShader<OpaqueWidgetShader>("opaque widget", eRenderLevelType::kOpaque);
		ret &= RegisterGraphicsShader<MaskedWidgetShader>("masked widget", eRenderLevelType::kOpaque);

		ret &= m_render_asset_manager->Initialize(device);

		return ret;
	}

	void RenderSystem::Shutdown()
	{
		for (const auto& [type, render_level] : m_graphics_render_levels)
			render_level->Shutdown();
		for (const auto& [name, shader] : m_graphics_shaders)
			shader->Shutdown();
		m_graphics_super_root_signature->Shutdown();
		m_render_asset_manager->Shutdown();
		m_camera_manager->Shutdown();
		m_device = nullptr;
		Render::s_render_system = nullptr;
	}

	void RenderSystem::Update(ID3D12Device* device)
	{
		m_camera_manager->Update(device, 
			[this](ID3D12Device* device) {
				m_graphics_render_levels.at(eRenderLevelType::kOpaque)->Update(device);
			});

		m_graphics_render_levels.at(eRenderLevelType::kDeferred)->Update(device);

		if (m_camera_manager->GetMainCamera() != nullptr)
		{
			m_graphics_render_levels.at(eRenderLevelType::kFinalView)->Update(device);
		}
		m_graphics_render_levels.at(eRenderLevelType::kUI)->Update(device);

		for (const auto& [name, shader] : m_graphics_shaders)
			shader->UpdateFrameResource(device);
	}


	void RenderSystem::PreDraw(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) const
	{
		m_render_asset_manager->PreDraw(device, command_list);
	}

	void RenderSystem::Draw(ID3D12GraphicsCommandList* command_list) const
	{
		//지금 이 부분은 Shadow나 Compute가 생길 경우 많이 바뀔 부분이다.
		//일단은 Opaque정도만 신경써서 코딩을 하였다.
		//지금으로서는 능력 부족으로 정확한 예측은 커녕 50%의 예측도 불가능하다.

		m_graphics_super_root_signature->Draw(command_list);
		m_render_asset_manager->Draw(command_list);

		if (m_camera_manager->GetMainCamera() != nullptr)
		{
			m_camera_manager->Draw(command_list,
				[this](ID3D12GraphicsCommandList* command_list)
				{
					m_graphics_render_levels.at(eRenderLevelType::kOpaque)->Draw(command_list);
				},
				[this](ID3D12GraphicsCommandList* command_list)
				{
					m_graphics_render_levels.at(eRenderLevelType::kDeferred)->Draw(command_list);
				},
				[this](ID3D12GraphicsCommandList* command_list)
				{
				});
		}
		
	}

	void RenderSystem::DrawMainCameraView(ID3D12GraphicsCommandList* command_list) const
	{
		if (m_camera_manager->GetMainCamera() != nullptr)
		{
			m_camera_manager->DrawMainCameraForUI(command_list);
			m_graphics_render_levels.at(eRenderLevelType::kFinalView)->Draw(command_list);
		}
	}

	void RenderSystem::DrawUI(ID3D12GraphicsCommandList* command_list) const
	{
		m_graphics_render_levels.at(eRenderLevelType::kUI)->Draw(command_list);
	}

	void RenderSystem::UpdateViewport()
	{
		const auto& window = m_window.lock();
		m_camera_manager->UpdateMainCameraViewport(window->width, window->height);
	}

	void RenderSystem::UnregisterGraphicsShader(const std::string& shader_name, eRenderLevelType level_type)
	{
		if (m_added_shaders.find(shader_name) != m_added_shaders.cend() && 
			m_graphics_render_levels.find(level_type) != m_graphics_render_levels.cend())
		{
			auto& shader = m_graphics_shaders[shader_name];
			m_added_shaders.erase(shader_name);
			m_graphics_render_levels[level_type]->UnregisterGraphicsShader(shader);
			shader->Shutdown();
			m_graphics_shaders.erase(shader_name);
		}
	}

	bool RenderSystem::RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name)
	{
		if (m_graphics_shaders.find(shader_name) == m_graphics_shaders.cend())
		{
			LOG_WARN("Could not find shader : {0}", shader_name);
			return false;
		}

		switch (render_comp->GetRenderType())
		{
		case eRenderType::kMesh:
		{
			const auto& mesh_comp = std::static_pointer_cast<MeshComponent>(render_comp);
			return m_graphics_shaders.at(shader_name)->RegisterMeshComponent(m_device, mesh_comp);
		}
		case eRenderType::kShape:
		{
			const auto& shape_comp = std::static_pointer_cast<ShapeComponent>(render_comp);
			return m_graphics_shaders.at(shader_name)->RegisterShapeComponent(m_device, shape_comp);
		}
		case eRenderType::kBillboard:
		{
			const auto& billboard_comp = std::static_pointer_cast<BillboardComponent>(render_comp);
			return m_graphics_shaders.at(shader_name)->RegisterBillboardComponent(m_device, billboard_comp);
		}
		case eRenderType::kWidget:
		{
			const auto& widget_comp = std::static_pointer_cast<WidgetComponent>(render_comp);
			return m_graphics_shaders.at(shader_name)->RegisterWidgetComponent(m_device, widget_comp);
		}
		default:
			break;
		}

		return false;
	}

	void RenderSystem::UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name)
	{
		if (m_graphics_shaders.find(shader_name) == m_graphics_shaders.cend())
		{
			LOG_WARN("Could not find shader : {0}", shader_name);
			return;
		}

		switch (render_comp->GetRenderType())
		{
		case eRenderType::kMesh:
		{
			const auto& mesh_comp = std::static_pointer_cast<MeshComponent>(render_comp);
			m_graphics_shaders.at(shader_name)->UnregisterMeshComponent(mesh_comp);
			break;
		}
		case eRenderType::kShape:
		{
			const auto shape_comp = std::static_pointer_cast<ShapeComponent>(render_comp);
			m_graphics_shaders.at(shader_name)->UnregisterShapeComponent(shape_comp);
			break;
		}
		case eRenderType::kBillboard:
		{
			const auto& billboard_comp = std::static_pointer_cast<BillboardComponent>(render_comp);
			m_graphics_shaders.at(shader_name)->UnregisterBillboardComponent(billboard_comp);
			break;
		}
		case eRenderType::kWidget:
		{
			const auto& widget_comp = std::static_pointer_cast<WidgetComponent>(render_comp);
			m_graphics_shaders.at(shader_name)->UnregisterWidgetComponent(widget_comp);
			break;
		}
		default:
			break;
		}
	}

	bool RenderSystem::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		return m_camera_manager->RegisterCameraComponent(camera_comp);
	}

	void RenderSystem::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		m_camera_manager->UnregisterCameraComponent(camera_comp);
	}

	void RenderSystem::SetMainCamera(const SPtr<CameraComponent>& camera_comp)
	{
		const auto& window = m_window.lock();
		m_camera_manager->SetMainCamera(camera_comp);
		m_camera_manager->UpdateMainCameraViewport(window->width, window->height);
	}
}
