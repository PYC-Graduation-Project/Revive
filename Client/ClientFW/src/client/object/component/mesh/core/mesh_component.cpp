#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/renderer/core/render.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/mesh.h"
#include "client/util/octree/octree_manager.h"

namespace client_fw
{
	MeshComponent::MeshComponent(const std::string& name, const std::string& draw_shader_name)
		: RenderComponent(name, 20, eRenderType::kMesh, draw_shader_name)
	{
		m_oriented_box = CreateSPtr<BOrientedBox>();
	}

	void MeshComponent::Shutdown()
	{
		m_mesh = nullptr;
	}

	void MeshComponent::UpdateLevelOfDetail(const Vec3& eye)
	{
		if (IsUseLevelOfDetail())
		{
			float distance = vec3::Length(eye - GetWorldPosition());
			float size = GetMaxExtent() / distance;

			UINT lod = 0;
			if (size > 0.275f)
				lod = 0;
			else if (size <= 0.275f && size > 0.0875f)
				lod = 1;
			else if (size <= 0.0875f && size > 0.0175f)
				lod = 2;
			else
				lod = 3;
			SetLevelOfDetail(lod);
		}
		else
		{
			SetLevelOfDetail(0);
		}
	}

	void MeshComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(m_mesh->GetOrientedBox(), GetWorldMatrix());
		Vec3 extents = m_oriented_box->GetExtents();
		m_max_extent = extents.x;
		m_max_extent = max(m_max_extent, extents.y);
		m_max_extent = max(m_max_extent, extents.z);
	}

	bool MeshComponent::IsUseLevelOfDetail() const
	{
		return m_mesh->GetLODCount() > 1;
	}

	void MeshComponent::SetLevelOfDetail(UINT lod)
	{
		m_level_of_detail = min(lod, m_mesh->GetLODCount() - 1);
		m_mesh->AddLODMeshCount(m_level_of_detail);
	}
}