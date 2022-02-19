#include "Opaque.hlsl"

cbuffer cbUITextureIndexData : register(b10, space0)
{
    uint g_ui_texture_index;
};

struct VS_UI_TEXTURE_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_UI_TEXTURE_OUTPUT VSRenderMainUI(uint vertex_id : SV_VertexID) 
{
    VS_UI_TEXTURE_OUTPUT output;
    
    if (vertex_id == 0) { output.sv_position = float4(-1.0f, 1.0f, 0.0f, 1.0f); output.uv = float2(0.0f, 0.0f); }
    else if (vertex_id == 1) { output.sv_position = float4(1.0f, 1.0f, 0.0f, 1.0f); output.uv = float2(1.0f, 0.0f); }
    else if (vertex_id == 2) { output.sv_position = float4(1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(1.0f, 1.0f); }
    else if (vertex_id == 3) { output.sv_position = float4(-1.0f, 1.0f, 0.0f, 1.0f); output.uv = float2(0.0f, 0.0f); }
    else if (vertex_id == 4) { output.sv_position = float4(1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(1.0f, 1.0f); }
    else if (vertex_id == 5) { output.sv_position = float4(-1.0f, -1.0f, 0.0f, 1.0f); output.uv = float2(0.0f, 1.0f); }
    else { output.sv_position = float4(0.0f, 0.0f, 0.0f, 1.0f); output.uv = float2(0.0f, 0.0f); }
   
    
    return output;
}

float4 PSRenderMainUI(VS_UI_TEXTURE_OUTPUT input) : SV_TARGET
{
    //юс╫ц
    return g_texture_data[g_ui_texture_index].Sample(g_sampler_point_wrap, input.uv);

}