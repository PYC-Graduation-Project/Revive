#pragma once
#include "client/object/component/core/render_component.h"

namespace client_fw
{
	class Mesh;
	struct VisualTreeNode;

	class MeshComponent : public RenderComponent
	{
	protected:
		MeshComponent(const std::string& name, const std::string& draw_shader_name);
		virtual ~MeshComponent() = default;

		virtual void Shutdown() override;

	protected:
		virtual void UpdateLevelOfDetail(const Vec3& eye);
		virtual void UpdateOrientedBox() override;

	protected:
		SPtr<Mesh> m_mesh;
		UINT m_instance_index = 0;
		float m_max_extent = 0;
		UINT m_level_of_detail = 0;

	public:
		virtual bool SetMesh(const std::string& file_path) = 0; 
		const SPtr<Mesh>& GetMesh() const { return m_mesh; }
		UINT GetInstanceIndex() const { return m_instance_index; }
		void SetInstanceIndex(UINT index) { m_instance_index = index; }
		float GetMaxExtent() const { return m_max_extent; }
		bool IsUseLevelOfDetail() const;
		UINT GetLevelOfDetail() const { return m_level_of_detail; }
		void SetLevelOfDetail(UINT lod);
	};
}


