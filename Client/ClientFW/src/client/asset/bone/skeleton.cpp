#include "stdafx.h"
#include "skeleton.h"
//#include "client/util/d3d_util.h"

namespace client_fw 
{
	void Skeleton::SetChild(SPtr<Skeleton>& child)
	{
		if (child)
		{
			//child->m_parent = this;
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

}
