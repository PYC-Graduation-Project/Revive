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

struct VS_UI_INPUT
{
    float2 position : POSITION;
    float2 size : SIZE;
    uint texture_index : TEXINDEX;
};

struct GS_UI_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float2 uv : TEXCOORD;
    uint texture_index : TEXINDEX;
};

VS_UI_INPUT VSRenderUI(VS_UI_INPUT input)
{
    return input;
}

static float2 s_ui_uvs[4] = { float2(0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

[maxvertexcount(4)]
void GSRenderUI(point VS_UI_INPUT input[1], inout TriangleStream<GS_UI_OUTPUT> out_stream)
{
    float2 vertices[4];
    
    vertices[0] = float2(input[0].position.x,  input[0].position.y - input[0].size.y);
    vertices[1] = float2(input[0].position.x, input[0].position.y);
    vertices[2] = float2(input[0].position.x + input[0].size.x, input[0].position.y - input[0].size.y);
    vertices[3] = float2(input[0].position.x + input[0].size.x, input[0].position.y);
    
    GS_UI_OUTPUT output;
    
    output.texture_index = input[0].texture_index;
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(float4(vertices[i], 0.0f, 1.0f), g_projection);
        output.sv_position.z = 0.0f;
        output.uv = s_ui_uvs[i];
        
        out_stream.Append(output);
    }
}

float4 PSRenderUI(GS_UI_OUTPUT input) : SV_TARGET
{
    return g_texture_data[input.texture_index].Sample(g_sampler_point_wrap, input.uv);
}

#endif // __UI_HLSL__