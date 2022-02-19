#include "Opaque.hlsl"

#ifndef __UI_HLSL__
#define __UI_HLSL__

cbuffer cbUITextureIndexData : register(b10, space0)
{
    uint g_ui_texture_index;
};

struct VS_RENDER_TEXTURE_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_RENDER_TEXTURE_OUTPUT VSRenderTexture(uint vertex_id : SV_VertexID) 
{
    VS_RENDER_TEXTURE_OUTPUT output;
    
    if (vertex_id == 0) { output.sv_position = float4(-1.0f, 1.0f, 0.0f, 1.0f); output.uv = float2(0.0f, 0.0f); }
    else if (vertex_id == 1) { output.sv_position = float4(1.0f, 1.0f, 0.0f, 1.0f); output.uv = float2(1.0f, 0.0f); }
    else if (vertex_id == 2) { output.sv_position = float4(1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(1.0f, 1.0f); }
    else if (vertex_id == 3) { output.sv_position = float4(-1.0f, 1.0f, 0.0f, 1.0f); output.uv = float2(0.0f, 0.0f); }
    else if (vertex_id == 4) { output.sv_position = float4(1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(1.0f, 1.0f); }
    else if (vertex_id == 5) { output.sv_position = float4(-1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(0.0f, 1.0f); }
    else { output.sv_position = float4(0.0f, 0.0f, 0.0f, 1.0f); output.uv = float2(0.0f, 0.0f); }
   
    
    return output;
}

float4 PSRenderMainUI(VS_RENDER_TEXTURE_OUTPUT input) : SV_TARGET
{
    //юс╫ц
    return g_texture_data[g_ui_texture_index].Sample(g_sampler_point_wrap, input.uv);

}

#endif // __UI_HLSL__