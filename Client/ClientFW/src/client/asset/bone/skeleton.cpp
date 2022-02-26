#include "stdafx.h"
#include "skeleton.h"
//#include "client/util/d3d_util.h"

namespace client_fw 
{
	const Mat4& Skeleton::GetWorld()
	{
		UpdateTransform(mat4::IDENTITY);
		return m_world;
	}
	void Skeleton::UpdateTransform(const Mat4& to_parent)
	{
		if (m_parent == nullptr)	m_world = m_to_parent;
		else m_world = m_to_parent * to_parent;

		if (m_sibling) m_sibling->UpdateTransform(to_parent);
		if (m_child) m_child->UpdateTransform(m_world);
	}
	void Skeleton::SetChild(SPtr<Skeleton>& child)
	{
		if (child)
		{
			child->m_parent = shared_from_this();
		}
		if (m_child)
		{
			if (child) child->m_sibling = m_child->m_sibling;
			m_child->m_sibling = child;
		}
		else
		{
			m_child = child;
		}
	}

	const SPtr<Skeleton>& Skeleton::FindBone(const std::string& bone_name)
	{
		SPtr<Skeleton> cache_skeleton = nullptr;
		if (m_bone_name.compare(bone_name) == false) return shared_from_this();

		if (m_sibling) if (cache_skeleton = m_sibling->FindBone(bone_name))return cache_skeleton;
		if (m_child) if (cache_skeleton = m_child->FindBone(bone_name))return cache_skeleton;

		return nullptr;
	}

}
