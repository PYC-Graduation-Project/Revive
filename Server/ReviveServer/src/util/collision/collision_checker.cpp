#include "collision_checker.h"
#include"util/collisioner.h"
bool CollisionChecker::CheckCollisions(BoxCollision& a, BoxCollision& b)
{
    return (a.GetMinPos().x <= b.GetMaxPos().x && a.GetMaxPos().x >= b.GetMinPos().x) &&
        (a.GetMinPos().y <= b.GetMaxPos().y && a.GetMaxPos().y >= b.GetMinPos().y) &&
        (a.GetMinPos().z <= b.GetMaxPos().z && a.GetMaxPos().z >= b.GetMinPos().z);
}

bool CollisionChecker::CheckInRange(float x, float z,const Vector3&min_pos, const Vector3& max_pos)
{
    return (min_pos.x<=x&&x<=max_pos.x&& min_pos.z <= z && z <= max_pos.z);
}

bool CollisionChecker::CheckCollisions(BoxCollision& a,  SphereCollision& b)
{
    return false;
}

bool CollisionChecker::CheckInRange( BoxCollision& range_obj,  SphereCollision& move_obj)
{
    return false;
}
