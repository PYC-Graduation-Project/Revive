#pragma once
#include <DirectXCollision.h>

namespace client_fw
{
	struct BBox;
	struct BOrientedBox;
	struct BFrustum;
	struct BSphere;

	struct BBox
	{
	public:
		BBox(Vec3 center = vec3::ZERO, Vec3 extents = Vec3(1.f, 1.f, 1.f));

		virtual void Transform(const Mat4& mat);
		void Transform(const BBox& in, const Mat4& mat);

		ContainmentType Contains(const BBox& box) const;
		bool Intersects(const BBox& box) const;
		ContainmentType Contains(const BOrientedBox& obox) const;
		bool Intersects(const BOrientedBox& obox) const;
		ContainmentType Contains(const BFrustum& frustum) const;
		bool Intersects(const BFrustum& frustum) const;
		ContainmentType Contains(const BSphere& sphere) const;
		bool Intersects(const BSphere& sphere) const;

	private:
		BoundingBox bounding_box;

	public:
		const BoundingBox& GetBoundingBox() const { return bounding_box; }
		BoundingBox& GetBoundingBox() { return bounding_box; }
		Vec3 GetCenter() const { return Vec3(bounding_box.Center); }
		Vec3 GetExtents() const { return Vec3(bounding_box.Extents); }
		void SetCenter(const Vec3& center) { bounding_box.Center = center; }
		void SetExtents(const Vec3& extents) { bounding_box.Extents = extents; }
	};

	struct BOrientedBox
	{
	public:
		BOrientedBox(Vec3 center = vec3::ZERO, Vec3 extents = Vec3(1.f, 1.f, 1.f));

		virtual void Transform(const Mat4& mat);
		void Transform(const BOrientedBox& in, const Mat4& mat);

		ContainmentType Contains(const BBox& box) const;
		bool Intersects(const BBox& box) const;
		ContainmentType Contains(const BOrientedBox& obox) const;
		bool Intersects(const BOrientedBox& obox) const;
		ContainmentType Contains(const BFrustum& frustum) const;
		bool Intersects(const BFrustum& frustum) const;
		ContainmentType Contains(const BSphere& sphere) const;
		bool Intersects(const BSphere& sphere) const;

	private:
		BoundingOrientedBox bounding_oriented_box;

	public:
		const BoundingOrientedBox& GetBoundingOrientedBox() const { return bounding_oriented_box; }
		BoundingOrientedBox& GetBoundingOrientedBox() { return bounding_oriented_box; }
		Vec3 GetCenter() const { return Vec3(bounding_oriented_box.Center); }
		Vec3 GetExtents() const { return Vec3(bounding_oriented_box.Extents); }
		void SetCenter(const Vec3& center) { bounding_oriented_box.Center = center; }
		void SetExtents(const Vec3& extents) { bounding_oriented_box.Extents = extents; }
	};

	struct BFrustum
	{
	public:
		BFrustum(Mat4 mat = mat4::IDENTITY);

		virtual void Transform(const Mat4& mat);
		void Transform(const BFrustum& in, const Mat4& mat);

		ContainmentType Contains(const BBox& box) const;
		bool Intersects(const BBox& box) const;
		ContainmentType Contains(const BOrientedBox& obox) const;
		bool Intersects(const BOrientedBox& obox) const;
		ContainmentType Contains(const BFrustum& frustum) const;
		bool Intersects(const BFrustum& frustum) const;
		ContainmentType Contains(const BSphere& sphere) const;
		bool Intersects(const BSphere& sphere) const;

	private:
		BoundingFrustum bounding_frustum;


	public:
		const BoundingFrustum& GetBoundingFrustum() const { return bounding_frustum; }
		BoundingFrustum& GetBoundingFrustum() { return bounding_frustum; }

	};

	struct BSphere
	{

	};
}



