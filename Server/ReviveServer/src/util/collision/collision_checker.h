#pragma once
#include"util/vec3.h"
#include"util/collision/collisioner.h"
class CollisionChecker
{
public:
	CollisionChecker()=default;
	CollisionChecker(const CollisionChecker&) = delete;
	CollisionChecker& operator=(const CollisionChecker&) = delete;
	
	static bool CheckCollisions( BoxCollision&a,BoxCollision& b);
	static bool CheckInRange(float x, float z, const Vector3& min_pos, const Vector3& max_pos);
	
	static bool CheckCollisions(SphereCollison& a, BoxCollision& b );
	static bool CheckInRange(BoxCollision& range_obj, SphereCollison& move_obj);
};

