#include "stdafx.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	BBox::BBox(Vec3 center, Vec3 extents)
	{
		SetCenter(center);
		SetExtents(extents);
	}

	void BBox::Transform(const Mat4& mat)
	{
		bounding_box.Transform(bounding_box, XMLoadFloat4x4(&mat));
	}

	void BBox::Transform(const BBox& in, const Mat4& mat)
	{
		in.GetBoundingBox().Transform(bounding_box, XMLoadFloat4x4(&mat));
	}

	ContainmentType BBox::Contains(const BBox& box) const
	{
		return bounding_box.Contains(box.GetBoundingBox());
	}

	bool BBox::Intersects(const BBox& box) const
	{
		return bounding_box.Intersects(box.GetBoundingBox());
	}

	ContainmentType BBox::Contains(const BOrientedBox& obox) const
	{
		return bounding_box.Contains(obox.GetBoundingOrientedBox());
	}

	bool BBox::Intersects(const BOrientedBox& obox) const
	{
		return bounding_box.Intersects(obox.GetBoundingOrientedBox());
	}

	ContainmentType BBox::Contains(const BFrustum& frustum) const
	{
		return bounding_box.Contains(frustum.GetBoundingFrustum());
	}

	bool BBox::Intersects(const BFrustum& frustum) const
	{
		return bounding_box.Intersects(frustum.GetBoundingFrustum());
	}
	
	ContainmentType BBox::Contains(const BSphere& sphere) const
	{
		return ContainmentType();
	}

	bool BBox::Intersects(const BSphere& sphere) const
	{
		return false;
	}

	BOrientedBox::BOrientedBox(Vec3 center, Vec3 extents)
	{
		SetCenter(center);
		SetExtents(extents);
	}
	
	void BOrientedBox::Transform(const Mat4& mat)
	{
		bounding_oriented_box.Transform(bounding_oriented_box, XMLoadFloat4x4(&mat));
	}

	void BOrientedBox::Transform(const BOrientedBox& in, const Mat4& mat)
	{
		in.GetBoundingOrientedBox().Transform(bounding_oriented_box, XMLoadFloat4x4(&mat));
	}

	ContainmentType BOrientedBox::Contains(const BBox& box) const
	{
		return bounding_oriented_box.Contains(box.GetBoundingBox());
	}

	bool BOrientedBox::Intersects(const BBox& box) const
	{
		return bounding_oriented_box.Intersects(box.GetBoundingBox());
	}

	ContainmentType BOrientedBox::Contains(const BOrientedBox& obox) const
	{
		return bounding_oriented_box.Contains(obox.GetBoundingOrientedBox());
	}

	bool BOrientedBox::Intersects(const BOrientedBox& obox) const
	{
		return bounding_oriented_box.Intersects(obox.GetBoundingOrientedBox());
	}

	ContainmentType BOrientedBox::Contains(const BFrustum& frustum) const
	{
		return bounding_oriented_box.Contains(frustum.GetBoundingFrustum());
	}

	bool BOrientedBox::Intersects(const BFrustum& frustum) const
	{
		return bounding_oriented_box.Intersects(frustum.GetBoundingFrustum());
	}

	ContainmentType BOrientedBox::Contains(const BSphere& sphere) const
	{
		return ContainmentType();
	}

	bool BOrientedBox::Intersects(const BSphere& sphere) const
	{
		return false;
	}

	BFrustum::BFrustum(Mat4 mat)
	{
		BoundingFrustum::CreateFromMatrix(bounding_frustum, XMLoadFloat4x4(&mat));
	}

	void BFrustum::Transform(const Mat4& mat)
	{
		bounding_frustum.Transform(bounding_frustum, XMLoadFloat4x4(&mat));
	}

	void BFrustum::Transform(const BFrustum& in, const Mat4& mat)
	{
		in.GetBoundingFrustum().Transform(bounding_frustum, XMLoadFloat4x4(&mat));
	}

	ContainmentType BFrustum::Contains(const BBox& box) const
	{
		return bounding_frustum.Contains(box.GetBoundingBox());
	}

	bool BFrustum::Intersects(const BBox& box) const
	{
		return bounding_frustum.Intersects(box.GetBoundingBox());
	}

	ContainmentType BFrustum::Contains(const BOrientedBox& obox) const
	{
		return bounding_frustum.Contains(obox.GetBoundingOrientedBox());
	}

	bool BFrustum::Intersects(const BOrientedBox& obox) const
	{
		return bounding_frustum.Intersects(obox.GetBoundingOrientedBox());
	}

	ContainmentType BFrustum::Contains(const BFrustum& frustum) const
	{
		return bounding_frustum.Contains(frustum.GetBoundingFrustum());
	}

	bool BFrustum::Intersects(const BFrustum& frustum) const
	{
		return bounding_frustum.Intersects(frustum.GetBoundingFrustum());
	}

	ContainmentType BFrustum::Contains(const BSphere& sphere) const
	{
		return ContainmentType();
	}

	bool BFrustum::Intersects(const BSphere& sphere) const
	{
		return false;
	}
}