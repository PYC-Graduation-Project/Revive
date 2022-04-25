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

bool CollisionChecker::lineIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& x)
{
    float det = (b - a).cross(d - c); 
    //두선이 평행인 경우 
    if (fabs(det) < EPSILON) return false; 
    x = a + (b - a) * ((c - a).cross(d - c) / det);
    return true;
}

float CollisionChecker::ccw(Vector2 a, Vector2 b)
{
    return a.cross(b);
}

float CollisionChecker::ccw(Vector2 p, Vector2 a, Vector2 b)
{
    return ccw(a - p, b - p);
}

bool CollisionChecker::paralleSegments(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& p)
{
    if (b < a) std::swap(a, b); if (d < c) std::swap(c, d); 
    //한 직선위에 없거나 두 선분이 겹치지 않는 경우를 우선 걸러낸다. 본문의 1번 관계인 경우이다. 
    if(ccw(a, b, c) != 0 || b < c || d < a) 
        return false; 
    //두 선분이 확실히 겹친다면 교차점 하나를 찾는다. 
    a<c ? p = c: p = a; 
    
    return true;


}

bool CollisionChecker::inBoundingRectangle(Vector2 p, Vector2 a, Vector2 b)
{
    if (b < a) std::swap(a, b); 
    return p == a || p == b || (a < p&& p < b);
}

bool CollisionChecker::segmentIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& p)
{
    if (!lineIntersection(a, b, c, d, p)) 
        return paralleSegments(a, b, c, d, p); 
    //p가 두 선분에 포함되어 있는 경우에만 참을 반환한다. 
    return inBoundingRectangle(p, a, b) && inBoundingRectangle(p, c, d);


}
