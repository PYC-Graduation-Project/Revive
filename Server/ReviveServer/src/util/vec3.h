#pragma once
#include<iostream>
#include<ostream>
using Vec2 = std::pair<short, short>;
class Vector3
{
public:
	Vector3() { x = 0; y = 0; z = 0; }
	Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
	float x, y, z;
#ifndef 연산자오버로딩
	friend std::ostream& operator<< (std::ostream & os, const Vector3 & a)
	{
		return os << "{" << a.x << ", " << a.y << ", " << a.z << "}";
	}
	friend Vector3 operator +(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	friend Vector3  operator -(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}
	friend void operator +=(Vector3& a, const Vector3& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;

	}
	friend bool operator ==(const Vector3& a, const Vector3& b)
	{
		return (a.x == b.x && a.y == b.y && a.z == b.z);
	}
	friend bool operator !=(const Vector3& a, const Vector3& b)
	{
		return (a.x != b.x || a.y != b.y || a.z != b.z);
	}
	friend Vector3 operator *(Vector3& a, const float b)
	{

		return Vector3(a.x * b, a.y * b, a.z * b);

	}
	friend Vector3 operator *(const Vector3& a, const float b)
	{

		return Vector3(a.x * b, a.y * b, a.z * b);

	}
	friend Vector3 operator *(const Vector3& a, const Vector3& b)
	{

		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);

	}
	friend Vector3 operator /(Vector3& a, const float b)
	{

		return Vector3(a.x / b, a.y / b, a.z / b);

	}
#endif // 연산자 오버로딩
	float VectorScale()
	{
		float temp_x = x*x;
		float temp_y = y*y;
		float temp_z = z*z;
		float scale = sqrt(temp_x + temp_y + temp_z);
		
		return scale;
	}
	Vector3 Normalrize()
	{
		float scale = VectorScale();
		Vector3 nor{ x / scale, y / scale, z / scale };
		return nor;
	}
	float Dist(const Vector3& other)
	{
		float temp_x = (x - other.x) * (x - other.x);
		float temp_y = (y - other.y) * (y - other.y);
		float temp_z = (z - other.z) * (z - other.z);
		return sqrt(temp_x + temp_y + temp_z);
	}
	float Dist2d(const Vector3& other)
	{
		float temp_x = (x - other.x) * (x - other.x);
		float temp_z = (z - other.z) * (z - other.z);
		return sqrt(temp_x +  temp_z);
	}
	float Dot(const Vector3& other)
	{
		return x * other.x + y * other.y + z * other.z;
	}
	Vector3 Cross(const Vector3& other)
	{
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
};