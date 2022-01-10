#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	class Mesh;

	class MeshComponent : public RenderComponent
	{
	protected:
		MeshComponent(const std::string& name = "mesh component");
		virtual ~MeshComponent() = default;

		virtual void Initialize() override;
		virtual void Shutdown() override;

	protected:
		virtual void RegisterToRenderSystem(const std::string& shader_name) override;
		virtual void RegisterToRenderSystem(eShaderType shader_type) override;
		virtual void UnregisterFromRenderSystem(const std::string& shader_name) override;
		virtual void UnregisterFromRenderSystem(eShaderType shader_type) override;

	protected:
		SPtr<Mesh> m_mesh;
		UINT m_instance_index = 0;

	public:
		const SPtr<Mesh>& GetMesh() const { return m_mesh; }
		virtual void SetMesh(const std::string& file_path);
		UINT GetInstanceIndex() const { return m_instance_index; }
		void SetInstanceIndex(UINT index) { m_instance_index = index; }
	};
}


