#include "resource.hlsl"

#ifndef __SHADOW_HLSL__
#define __SHADOW_HLSL__

struct VS_SHADOW_OUT
{
    float4 sv_position : SV_POSITION;
};

float CalcShadowFactor(float3 position, ShadowTextureData shadow_data)
{
    float4 shadow_pos = mul(float4(position, 1.0f), shadow_data.uv_from_ndc);
    
    shadow_pos.xyz /= shadow_pos.w;
    
    float percent_lit = 0.0f;
    
    float dx = shadow_data.inverse_texture_size.x;
    float dy = shadow_data.inverse_texture_size.y;
    
    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, dy), float2(0.0f, dy), float2(dx, dy)
    };
    
    Texture2D shadow_texture = g_texture_data[shadow_data.shadow_texture_index];
    
    [unroll]
    for (int i = 0; i < 9; ++i)
    {
        percent_lit += shadow_texture.SampleCmpLevelZero(g_sampler_comparison_pcf_shadow, shadow_pos.xy + offsets[i], shadow_pos.z).r;
    }
    
    return percent_lit / 9.0f;
}

#endif //__SHADOW_HLSL__