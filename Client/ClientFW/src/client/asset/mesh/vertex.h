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
}
