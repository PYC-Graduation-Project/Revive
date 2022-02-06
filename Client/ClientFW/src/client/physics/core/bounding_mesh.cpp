#include "stdafx.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	namespace triangle_test
	{
		bool Intersect(const Vec3& a1, const Vec3& a2, const Vec3& a3, const Vec3& b1, const Vec3& b2, const Vec3& b3)
		{
			return TriangleTests::Intersects(XMLoadFloat3(&a1), XMLoadFloat3(&a2), XMLoadFloat3(&a3),
				XMLoadFloat3(&b1), XMLoadFloat3(&b2), XMLoadFloat3(&b3));
		}

		bool Intersect(const Vec3& origin, const Vec3& direction, const Vec3& v1, const Vec3& v2, const Vec3& v3, float& distance)
		{
			return false;
		}
	}

	BBox::BBox(Vec3 center, Vec3 extents)
	{
		SetCenter(center);
		SetExtents(extents);
	}

	BOrientedBox::BOrientedBox(Vec3 center, Vec3 extents)
	{
		SetCenter(center);
		SetExtents(extents);
	}

	BFrustum::BFrustum(Mat4 mat)
	{
		BoundingFrustum::CreateFromMatrix(m_bounding, XMLoadFloat4x4(&mat));
	}

	BSphere::BSphere(Vec3 center, float radius)
	{
		SetCenter(center);
		SetRadius(radius);
	}
}