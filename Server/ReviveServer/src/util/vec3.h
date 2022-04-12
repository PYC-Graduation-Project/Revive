#pragma once
#include<iostream>
#include<ostream>
class Vector3
{
public:
	Vector3() { x = 0; y = 0; z = 0; }
	Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
	float x, y, z;
#ifndef �����ڿ����ε�
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
	friend Vector3 operator *(Vector3& a, const float b)
	{

		return Vector3(a.x * b, a.y * b, a.z * b);

	}
	friend Vector3 operator *(Vector3& a, Vector3& b)
	{

		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);

	}
	friend Vector3 operator /(Vector3& a, const float b)
	{

		return Vector3(a.x / b, a.y / b, a.z / b);

	}
#endif // ������ �����ε�
	float VectorScale()
	{
		float temp_x = abs(x);
		float temp_y = abs(y);
		float temp_z = abs(z);
		float scale = sqrt(pow(temp_x, 2) + pow(temp_y, 2) + pow(temp_z, 2));
		
		return scale;
	}
	Vector3 Normalrize()
	{
		float scale = VectorScale();
		Vector3 nor{ x / scale, y / scale, z / scale };
		return nor;
	}
};