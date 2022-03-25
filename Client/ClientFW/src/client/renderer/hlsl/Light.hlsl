#include "resource.hlsl"

#ifndef __LIGHT_HLSL__
#define __LIGHT_HLSL__

struct Material
{
    float3 base_color;
    float3 normal;
    float metalic;
    float roughness;
};

struct Light
{
    float3 light_color;
    float3 direction;
};

float3 FresnelSchlick(float cos_theta, float3 f0)
{
    return f0 + (1.0f - f0) * pow(clamp(1.0f - cos_theta, 0.0f, 1.0f), 5.0f);
}

float DistributionGGX(float3 normal, float3 half_way, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    
    float ndoth = max(dot(normal, half_way), 0.0f);
    float ndoth2 = ndoth * ndoth;
    
    float denom = (ndoth2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;
    
    return a2 / denom;
}

float GeometrySchlickGGX(float ndotv, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) * 0.125f;
 
    float denom = ndotv * (1.0f - k) + k;
    
    return ndotv / denom;
}

float GeometrySmith(float3 normal, float3 to_camera, float3 to_light, float roughness)
{
    float ndotv = max(dot(normal, to_camera), 0.0f);
    float ndotl = max(dot(normal, to_light), 0.0f);
    float ggx2 = GeometrySchlickGGX(ndotv, roughness);
    float ggx1 = GeometrySchlickGGX(ndotl, roughness);
    
    return ggx1 * ggx2;
}

float3 CalcDiretionalLight(float3 position, Material material, Light light)
{
    float3 to_camera = normalize(g_camera_pos - position);
    
    float3 f0 = 0.04f;
    f0 = lerp(f0, material.base_color, material.metalic);
    
    float3 to_light = -light.direction;
    float3 half_way = normalize(to_camera + to_light);
    
    float3 radiance = light.light_color;
    
    float ndf = DistributionGGX(material.normal, half_way, material.roughness);
    float g = GeometrySmith(material.normal, to_camera, to_light, material.roughness);
    float3 f = FresnelSchlick(max(dot(half_way, to_camera), 0.0f), f0);
    
    float3 k_specular = f;
    float3 k_diffuse = 1.0f - k_specular;
    k_diffuse *= 1.0f - material.metalic;
    
    float ndotl = max(dot(material.normal, to_light), 0.0f);
    float ndotv = max(dot(material.normal, to_camera), 0.0f);
    
    float3 numerator = ndf * g * f;
    float denominator = 4.0f * max(ndotv, 0.0f) * max(ndotl, 0.0f);
    float3 specular = numerator / max(denominator, 0.001f);
    
    float3 lo = (k_diffuse * material.base_color / PI + specular) * radiance * ndotl;
    
    return lo;
}

#endif //__LIGHT_HLSL__