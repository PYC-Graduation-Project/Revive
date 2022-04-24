#include<algorithm>
#include "collision_checker.h"

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

bool CollisionChecker::CheckCollisions(SphereCollison& a,BoxCollision& b  )
{
    float x = std::fmaxf(b.GetMinPos().x, fminf(a.GetCenterPos().x, b.GetCenterPos().x));
    float y = std::fmaxf(b.GetMinPos().y, fminf(a.GetCenterPos().y, b.GetCenterPos().y));
    float z = std::fmaxf(b.GetMinPos().z, fminf(a.GetCenterPos().z, b.GetCenterPos().z));
    float distance = sqrt((x - a.GetCenterPos().x) * (x - a.GetCenterPos().x) +
        (y - a.GetCenterPos().y) * (y - a.GetCenterPos().y) +
        (z - a.GetCenterPos().z) * (z - a.GetCenterPos().z));
    return distance < a.GetRadius();
}

bool CollisionChecker::CheckInRange( BoxCollision& range_obj, SphereCollison& move_obj)
{
    return false;
}
