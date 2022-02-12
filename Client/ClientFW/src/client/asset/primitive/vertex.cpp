#include "stdafx.h"
#include "client/asset/primitive/vertex.h"

namespace client_fw
{
	Vertex::Vertex()
		: m_position(vec3::ZERO)
	{
	}

	Vertex::Vertex(const Vec3& position)
		: m_position(position)
	{
	}

	TextureVertex::TextureVertex()
		: Vertex(), m_tex_coord(vec2::ZERO)
	{
	}

	TextureVertex::TextureVertex(const Vec3& position, const Vec2& tex_coord)
		: Vertex(position), m_tex_coord(tex_coord)
	{
	}

	TextureLightVertex::TextureLightVertex()
		: TextureVertex(), m_normal(vec3::AXIS_Y)
	{
	}

	TextureLightVertex::TextureLightVertex(const Vec3& position, const Vec3& normal, const Vec2& tex_coord)
		: TextureVertex(position, tex_coord), m_normal(normal)
	{
	}
}