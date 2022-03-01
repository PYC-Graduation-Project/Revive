#pragma once

namespace client_fw
{
	class Vertex
	{
	public:
		Vertex();
		Vertex(const Vec3& position);

		const Vec3& GetPosition() const { return m_position; }
		void SetPosition(const Vec3& position) { m_position = position; }
			
	protected:
		Vec3 m_position;
	
	};

	class TextureVertex : public Vertex
	{
	public:
		TextureVertex();
		TextureVertex(const Vec3& position, const Vec2& tex_coord);

		const Vec2& GetTexCoord() const { return m_tex_coord; }
		void SetTexCoord(const Vec2& tex_coord) { m_tex_coord = tex_coord; }

	protected:
		Vec2 m_tex_coord;
	};

	class TextureLightVertex : public TextureVertex
	{
	public:
		TextureLightVertex();
		TextureLightVertex(const Vec3& position, const Vec3& normal, const Vec2& tex_coord);

		const Vec3& GetNormal() const { return m_normal; }
		void SetNormal(const Vec3& normal) { m_normal = normal; }

	protected:
		Vec3 m_normal;
	};

	class ShapeVertex : public Vertex
	{
	public:
		ShapeVertex();
		ShapeVertex(const Vec3& position, const Vec3& size,
			const Vec3& forward, const Vec3& right, const Vec3& up);

		const Vec3& GetSize() const { return m_size; }
		void SetSize(const Vec3& size) { m_size = size; }
		const Vec3& GetForward() const { return m_forward; }
		const Vec3& GetRight() const { return m_right; }
		const Vec3& GetUp() const { return m_up; }
		void SetForward(const Vec3& forward) { m_forward = forward; }
		void SetRight(const Vec3& right) { m_right = right; }
		void SetUp(const Vec3& up) { m_up = up; }

	protected:
		Vec3 m_size;
		Vec3 m_forward;
		Vec3 m_right;
		Vec3 m_up;
	};

	class BoneVertex : public TextureLightVertex
	{
	public:
		BoneVertex();
		BoneVertex(const Vec3& position, const Vec3& normal, const Vec2& tex_coord, const Vec4& bone_weight, const IVec4& bone_index);

		const Vec4& GetBoneWeight() const { return m_bone_weight; }
		void SetBoneWeight(const Vec4& bone_weight) { m_bone_weight = bone_weight; }
		const IVec4& GetBoneIndex() const { return m_bone_index; }
		void SetBoneIndex(const IVec4& bone_index) { m_bone_index = bone_index; }

	protected:
		Vec4 m_bone_weight;
		IVec4 m_bone_index;
	};
}
