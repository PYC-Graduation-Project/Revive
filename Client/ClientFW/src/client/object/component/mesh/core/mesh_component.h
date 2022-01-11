#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	class Mesh;

	class MeshComponent : public RenderComponent
	{
	protected:
		MeshComponent(const std::string& name, const std::string& draw_shader_name);
		virtual ~MeshComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	protected:
		virtual bool RegisterToRenderSystem() override;
		virtual void UnregisterFromRenderSystem() override;

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


