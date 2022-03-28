#ifndef __COMMON_HLSL__
#define __COMMON_HLSL__

float3 GetNormalFromNormalMap(float3 normal_map, float3 world_normal, float3 world_tangent, float3 world_bitangent)
{
    float3 normal = 2.0f * normal_map - 1.0f;
    
    float3 N = world_normal;
    float3 T = normalize(world_tangent - dot(world_tangent, N) * N);
    float3 B = cross(N, T);
    
    float3x3 TBN = float3x3(T, B, N);
    
    return mul(normal, TBN);
}

#endif //__COMMON_HLSL__