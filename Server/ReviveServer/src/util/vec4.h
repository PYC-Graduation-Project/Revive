#pragma once
#pragma once
#include<iostream>
class Vector4
{
public:
	Vector4() { x = 0; y = 0; z = 0; w = 0; }
	Vector4(float x, float y, float z,float w) :x(x), y(y), z(z),w(w) {}
	float x, y, z,w;
#ifndef 연산자오버로딩

	friend Vector4 operator +(const Vector4& a, const Vector4& b)
	{
		return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}

	friend Vector4  operator -(const Vector4& a, const Vector4& b)
	{
		return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}
	friend void operator +=(Vector4& a, const Vector4& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
		a.w += b.w;

	}
	friend Vector4 operator *(Vector4& a, const float b)
	{

		return Vector4(a.x * b, a.y * b, a.z * b, a.w * b);

	}

#endif // 연산자 오버로딩
	
};