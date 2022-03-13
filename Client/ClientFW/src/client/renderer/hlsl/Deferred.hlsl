#include "Opaque.hlsl"
#include "UI.hlsl"

#ifndef __DEFERRED_HLSL__
#define __DEFERRED_HLSL__

//임시
//아직 조명적용을 하지 않기 떄문에 그냥 Texture전달로 끝
float4 PSRenderTexture(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    return g_texture_data[g_gbuffer_texture_indices[0]].Sample(g_sampler_point_wrap, input.uv);
}

#endif // __DEFERRED_HLSL__