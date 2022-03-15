#include "Opaque.hlsl"

#ifndef __WIDGET_HLSL__
#define __WIDGET_HLSL__

struct VS_WORLD_WIDGET_INPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    int texture_index : TEXINDEX;
    float4 color : COLOR;
    float2 coordinate : TEXCOORD;
    float2 tilling : TILLING;
    float3 right : RIGHT;
    float3 up : UP;
};

struct GS_WORLD_WIDGET_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    int texture_index : TEXINDEX;
    float4 color : COLOR;
};

VS_WORLD_WIDGET_INPUT VSWorldWidget(VS_WORLD_WIDGET_INPUT input)
{
    return input;
}

static float2 s_ui_uvs[4] = { float2(0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

[maxvertexcount(4)]
void GSWorldWidget(point VS_WORLD_WIDGET_INPUT input[1], inout TriangleStream<GS_WORLD_WIDGET_OUTPUT> out_stream)
{
    float3 vertices[4];
    
    float3 pos = input[0].position;
    float3 right = input[0].right * input[0].size.x;
    float3 up = input[0].up * input[0].size.y;
    vertices[0] = pos - up;
    vertices[1] = pos;
    vertices[2] = pos - up + right;
    vertices[3] = pos + right;
    
    GS_WORLD_WIDGET_OUTPUT output;
    
    output.normal = normalize(cross(input[0].right, input[0].up));
    output.texture_index = input[0].texture_index;
    output.color = input[0].color;

    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(float4(vertices[i], 1.0f), g_view_projection);
        output.uv = s_ui_uvs[i] * input[0].tilling + input[0].coordinate;
        
        out_stream.Append(output);
    }
}

PS_GBUFFER_OUTPUT PSMaskedWidget(GS_WORLD_WIDGET_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    float4 base_color;
    
    if (input.texture_index >= 0)
        base_color = g_texture_data[input.texture_index].Sample(g_sampler_point_wrap, input.uv) * input.color;
    else
        base_color = input.color;
    
    clip(base_color.a - 0.333333f);
    output.base_color = base_color;
    
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    
    return output;
}

#endif // __WIDGET_HLSL__