#include "stdafx.h"
#include "client/math/math.h"

namespace client_fw
{
	IVec4& IVec4::operator+=(const IVec4& rhs)
	{
		XMStoreSInt4(this, XMVectorAdd(XMLoadSInt4(this), XMLoadSInt4(&rhs)));
		return *this;
	}

	IVec4& IVec4::operator-=(const IVec4& rhs)
	{
		XMStoreSInt4(this, XMVectorSubtract(XMLoadSInt4(this), XMLoadSInt4(&rhs)));
		return *this;
	}

	IVec4& IVec4::operator*=(const IVec4& rhs)
	{
		XMStoreSInt4(this, XMVectorMultiply(XMLoadSInt4(this), XMLoadSInt4(&rhs)));
		return *this;
	}

	IVec4& IVec4::operator*=(int scalar)
	{
		XMStoreSInt4(this, XMVectorScale(XMLoadSInt4(this), static_cast<float>(scalar)));
		return *this;
	}

	IVec4& IVec4::operator/=(const IVec4& rhs)
	{
		XMStoreSInt4(this, XMVectorDivide(XMLoadSInt4(this), XMLoadSInt4(&rhs)));
		return *this;
	}

	IVec4& IVec4::operator/=(int scalar)
	{
		XMStoreSInt4(this, XMVectorScale(XMLoadSInt4(this), (1.0f / static_cast<float>(scalar))));
		return *this;
	}

	std::string IVec4::ToString() const
	{
		std::stringstream ret;
		ret << "Vec2 : {" << x << ", " << y << "}";
		return ret.str();
	}

	IVec4 operator+(const IVec4& v1, const IVec4& v2)
	{
		IVec4 ret(v1);
		ret += v2;
		return ret;
	}

	IVec4 operator-(const IVec4& v1, const IVec4& v2)
	{
		IVec4 ret(v1);
		ret -= v2;
		return ret;
	}

	IVec4 operator*(const IVec4& v1, const IVec4& v2)
	{
		IVec4 ret(v1);
		ret *= v2;
		return ret;
	}

	IVec4 operator*(const IVec4& v, int scalar)
	{
		IVec4 ret(v);
		ret *= scalar;
		return ret;
	}

	IVec4 operator*(int scalar, const IVec4& v)
	{
		return v * scalar;
	}

	IVec4 operator/(const IVec4& v1, const IVec4& v2)
	{
		IVec4 ret(v1);
		ret /= v2;
		return ret;
	}

	IVec4 operator/(const IVec4& v, int scalar)
	{
		IVec4 ret(v);
		ret /= scalar;
		return ret;
	}

	IVec4 operator/(int scalar, const IVec4& v)
	{
		return v / scalar;
	}

	bool operator==(const IVec4& v1, const IVec4& v2)
	{
		return XMVector2Equal(XMLoadSInt4(&v1), XMLoadSInt4(&v2));
	}

	bool operator!=(const IVec4& v1, const IVec4& v2)
	{
		return !(v1 == v2);
	}

	bool operator<(const IVec4& v1, const IVec4& v2)
	{
		return XMVector2Less(XMLoadSInt4(&v1), XMLoadSInt4(&v2));
	}

	bool operator<=(const IVec4& v1, const IVec4& v2)
	{
		return XMVector2LessOrEqual(XMLoadSInt4(&v1), XMLoadSInt4(&v2));
	}

	bool operator>(const IVec4& v1, const IVec4& v2)
	{
		return XMVector2Greater(XMLoadSInt4(&v1), XMLoadSInt4(&v2));
	}
	bool operator>=(const IVec4& v1, const IVec4& v2)
	{
		return XMVector2GreaterOrEqual(XMLoadSInt4(&v1), XMLoadSInt4(&v2));
	}

	std::ostream& operator<<(std::ostream& os, const IVec4& v)
	{
		return os << v.ToString();
	}
}