#include "resource.hlsl"
#include "UI.hlsl"

#ifndef __DEFERRED_HLSL__
#define __DEFERRED_HLSL__

//임시
//아직 조명적용을 하지 않기 떄문에 그냥 Texture전달로 끝
float4 PSRenderTexture(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    return g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, input.uv);
}

float4 PSRenderTextureWithDirectionalLight(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    float3 light_color = float3(0.f, 0.f, 0.f);
    [unroll(4)]
    for (uint i = 0; i < g_num_of_directional_light; ++i)
    {
        light_color += g_light_data[i].light_color;
    }
    
    return g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, input.uv) * float4(light_color, 1.0f);
}

#endif // __DEFERRED_HLSL__