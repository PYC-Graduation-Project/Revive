#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/core/render_system.h"
#include "client/renderer/rootsignature/graphics_super_root_signature.h"
#include "client/renderer/renderlevel/opaque_render_level.h"
#include "client/renderer/shader/opaque_mesh_shader.h"	
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/object/component/util/camera_component.h"
#include "client/asset/mesh/mesh.h"

namespace client_fw
{
	RenderSystem* Render::s_render_system = nullptr;

	RenderSystem::RenderSystem()
	{
		Render::s_render_system = this;
		m_graphics_super_root_signature = CreateSPtr<GraphicsSuperRootSignature>();

		m_render_level_order = { {eRenderLevelType::kOpaque, eKindOfRenderLevel::kGraphics} };
	}

	bool RenderSystem::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		m_device = device;
		m_command_list = command_list;

		m_graphics_super_root_signature->Initialize(device, command_list);

		RegisterGraphicsRenderLevel<OpaqueRenderLevel>(eRenderLevelType::kOpaque);
		RegisterGraphicsShader<OpaqueMeshShader>("opaque mesh", eRenderLevelType::kOpaque);

		return true;
	}

	void RenderSystem::Shutdown()
	{
		for (const auto& [type, render_level] : m_graphics_render_levels)
			render_level->Shutdown();
		for (const auto& [name, shader] : m_graphics_shaders)
			shader->Shutdown();
		m_device = nullptr;
		Render::s_render_system = nullptr;
	}

	void RenderSystem::Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		for (const auto& mesh : m_ready_meshes)
		{
			mesh->Initialize(device, command_list);
			LOG_INFO(mesh->GetPath());
		}
		m_ready_meshes.clear();

		for (const auto& [level_name, kind] : m_render_level_order)
		{
			switch (kind)
			{
			case eKindOfRenderLevel::kGraphics:
				m_graphics_render_levels.at(level_name)->Update(device, command_list);
				break;
			case eKindOfRenderLevel::kDeferred:
				break;
			case eKindOfRenderLevel::kCompute:
				break;
			}
		}
	}

	void RenderSystem::Draw(ID3D12GraphicsCommandList* command_list)
	{
		m_graphics_super_root_signature->Draw(command_list);

		for (const auto& [level_name, kind] : m_render_level_order)
		{
			switch (kind)
			{
			case eKindOfRenderLevel::kGraphics:
				m_graphics_render_levels.at(level_name)->Draw(command_list, m_basic_cameras);
				break;
			case eKindOfRenderLevel::kDeferred:
				break;
			case eKindOfRenderLevel::kCompute:
				break;
			}
		}
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

	bool RenderSystem::RegisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		if (m_initialized_assets.find(mesh_comp->GetMesh()->GetPath()) == m_initialized_assets.cend())
		{
			m_initialized_assets.insert(mesh_comp->GetMesh()->GetPath());
			m_ready_meshes.push_back(mesh_comp->GetMesh());
		}

		if (m_graphics_shaders.find(shader_name) == m_graphics_shaders.cend())
		{
			LOG_WARN("Could not find shader : {0}", shader_name);
			return false;
		}

		return m_graphics_shaders.at(shader_name)->RegisterMeshComponent(mesh_comp);
	}

	void RenderSystem::UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp, const std::string& shader_name)
	{
		if (m_graphics_shaders.find(shader_name) != m_graphics_shaders.cend())
			m_graphics_shaders.at(shader_name)->UnregisterMeshComponent(mesh_comp);
	}

	bool RenderSystem::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		switch (camera_comp->GetCameraUsage())
		{
		case eCameraUsage::kBasic:
			m_basic_cameras.push_back(camera_comp);
			break;
		case eCameraUsage::kLight:
			break;
		}
		return true;
	}

	void RenderSystem::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		switch (camera_comp->GetCameraUsage())
		{
		case eCameraUsage::kBasic:
		{
			auto iter = std::find(m_basic_cameras.begin(), m_basic_cameras.end(), camera_comp);
			if (iter != m_basic_cameras.end())
			{
				std::iter_swap(iter, m_basic_cameras.end() - 1);
				m_basic_cameras.pop_back();
			}
			break;
		}
		case eCameraUsage::kLight:
			break;
		}
	}			
}
