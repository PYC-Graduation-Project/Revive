#pragma once
const float EPSILON = 0.0001;

class Vector2 {
public:
	float x, z; //생성자 
	Vector2(){} Vector2(float _x, float _z){ x = _x, z = _z; }
	//외적 
	float cross(const Vector2& other) const{ 
		return (x*other.z)-(z*other.x); 
	} 
	/* 연산자 오버로딩을 통해 실제 벡터의 연산을 구현합니다. */ 
	//벡터의 실수배 
	Vector2 operator * (double r) const{ return Vector2(x*r, z*r); }
	float operator * (Vector2 r) const { return (x * r.x)+ (z * r.z); }
	//벡터의 덧셈 
	Vector2 operator + (Vector2 other) const{ 
		return Vector2(x + other.x, z + other.z); } 
	//벡터의 뺄셈 
	Vector2 operator - (Vector2 other) const{
		return Vector2(x - other.x, z - other.z); 
	} 
	float Dist2d(const Vector2& other)
	{
		float temp_x = (x - other.x) * (x - other.x);
		float temp_z = (z - other.z) * (z - other.z);
		return sqrt(temp_x + temp_z);
	}
	//두 벡터의 비교 
	bool operator == (Vector2 other) const{
		return x == other.x && z == other.z; 
	} bool operator < (Vector2 other) const{
		return x < other.x && z < other.z; } 
};


