#pragma once
#include "client/object/component/core/component.h"

namespace client_fw
{
	class Mesh;

	class MeshComponent : public Component, public std::enable_shared_from_this<MeshComponent>
	{
	protected:
		MeshComponent(const std::string& name, const std::string& draw_shader_name);
		virtual ~MeshComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	private:
		bool RegisterToRenderSystem();
		void UnregisterFromRenderSystem();

	protected:
		SPtr<Mesh> m_mesh;
		UINT m_instance_index = 0;
		std::string m_draw_shader_name;

	public:
		const SPtr<Mesh>& GetMesh() const { return m_mesh; }
		virtual void SetMesh(const std::string& file_path);
		UINT GetInstanceIndex() const { return m_instance_index; }
		void SetInstanceIndex(UINT index) { m_instance_index = index; }
		void SetDrawShaderName(const std::string& shader_name) { m_draw_shader_name = shader_name; }
	};
}


