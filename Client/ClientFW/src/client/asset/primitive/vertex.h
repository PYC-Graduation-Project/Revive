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

	class UIVertex
	{
	public:
		UIVertex();
		UIVertex(const Vec2& position, const Vec2& size, UINT index);

		const Vec2& GetPosition() const { return m_position; }
		void SetPosition(const Vec2& position) { m_position = position; }
		const Vec2& GetSize() const { return m_size; }
		void SetSize(const Vec2& size) { m_size = size; }
		UINT GetTextureResourceIndex() const { return m_texture_resource_index; }
		void SetTextureResourceIndex(UINT index) { m_texture_resource_index = index; }

	protected:
		Vec2 m_position;
		Vec2 m_size;
		UINT m_texture_resource_index;
	};
}
