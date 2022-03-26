#include "resource.hlsl"
#include "light.hlsl"
#include "ui.hlsl"

#ifndef __DEFERRED_HLSL__
#define __DEFERRED_HLSL__

//임시
//아직 조명적용을 하지 않기 떄문에 그냥 Texture전달로 끝

float ConvertToLinearDepth(float depth)
{
    return g_perspective_values.z / (depth + g_perspective_values.w);
}

float3 GetWolrdPoisition(float2 uv, float depth)
{
    float3 position;
    uv.x = uv.x * 2.0f - 1.0f;
    uv.y = (1.0f - uv.y) * 2.0f - 1.0f;
        
    position.xy = uv.xy * g_perspective_values.xy * depth;
    position.z = depth;
    
    return mul(float4(position, 1.0f), g_inverse_view).xyz;
}


float4 PSRenderTexture(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    return float4(g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, input.uv).xyz, 1.0f);
}

float4 PSRenderTextureWithDirectionalLight(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    float depth = g_texture_data[g_gbuffer_texture_indices[3]].Sample(g_sampler_point_wrap, input.uv).x;
    depth = ConvertToLinearDepth(depth);
    float3 position = GetWolrdPoisition(input.uv, depth);
    float3 normal = g_texture_data[g_gbuffer_texture_indices[1]].Sample(g_sampler_point_wrap, input.uv).xyz;
    normal = normalize(normal * 2.0f - 1.0f);
    float3 base_color = g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, input.uv).xyz;
    
    float4 additional_info = g_texture_data[g_gbuffer_texture_indices[2]].Sample(g_sampler_point_wrap, input.uv);
    
    Material material;
    material.base_color = base_color;
    material.normal = normal;
    material.roughness = additional_info.x;
    material.metalic = additional_info.y;
    
    float3 color = float3(0.f, 0.f, 0.f);
    [unroll(4)]
    for (uint i = 0; i < g_num_of_directional_light; ++i)
    {
        Light light;
        light.light_color = g_light_data[i].light_color;
        light.direction = g_light_data[i].light_direction;

        color += CalcDiretionalLight(position, material, light);
    }
    
     
    float3 ambient = 0.03f * material.base_color;
    color += ambient;
    color = color / (color + 1.0f);
    color = pow(color, (1.0f / 2.2f));
    
    return float4(color, 1.0f);
}

#endif // __DEFERRED_HLSL__