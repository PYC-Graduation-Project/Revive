#include "stdafx.h"
#include "skeleton.h"
//#include "client/util/d3d_util.h"

namespace client_fw 
{
	void Skeleton::SetChild(SPtr<Skeleton>& child, SPtr<Skeleton>& parent)
	{
		if (child)
		{
			child->m_parent = parent;
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
		if (bone_name.compare(bone_name) == false) return shared_from_this();

		if (m_sibling) if (cache_skeleton = m_sibling->FindBone(bone_name))return cache_skeleton;
		if (m_child) if (cache_skeleton = m_child->FindBone(bone_name))return cache_skeleton;

		return nullptr;
	}

}
