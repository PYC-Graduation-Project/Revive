#include "Opaque.hlsl"

#ifndef __BILLBOARD_HLSL__
#define __BILLBOARD_HLSL__

struct VS_BILLBOARD_INPUT
{
    float3 position : POSITION;
    float2 size : SIZE;
    uint resource_index : RSINDEX;
    float2 coordinate : TEXCOORD;
};

struct GS_BILLBOARD_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 normal : NORMAL;
    uint resource_index : RSINDEX;
    float2 uv : TEXCOORD;
};

VS_BILLBOARD_INPUT VSBillboard(VS_BILLBOARD_INPUT input)
{
    return input;
}

static float2 s_billboard_uvs[4] = { float2(0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

void GetBillboardConrers(float3 position, float2 size, out float4 vertices[4], out float3 look, out float3 right, out float3 up)
{
    look = g_view._13_23_33 * -1.0f;
    right = normalize(cross(g_view._12_22_32, look));
    up = normalize(cross(look, right));
    
    vertices[0] = float4(position + size.x * right - size.y * up, 1.0f);
    vertices[1] = float4(position + size.x * right + size.y * up, 1.0f);
    vertices[2] = float4(position - size.x * right - size.y * up, 1.0f);
    vertices[3] = float4(position - size.x * right + size.y * up, 1.0f);
}

[maxvertexcount(4)]
void GSBillboard(point VS_BILLBOARD_INPUT input[1], inout TriangleStream<GS_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_BILLBOARD_OUTPUT output;
    
    output.resource_index = input[0].resource_index;
    
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(mul(vertices[i], g_view), g_projection);
        output.normal = look;
        output.uv = s_billboard_uvs[i] + input[0].coordinate;
        
        out_stream.Append(output);
    }
}

void GetFixUpBillboardConrers(float3 position, float2 size, out float4 vertices[4], out float3 look, out float3 right, out float3 up)
{
    up = float3(0.0f, 1.0f, 0.0f);
    look = g_view._13_23_33 * -1.0f;
    right = normalize(cross(up, look));
    
    vertices[0] = float4(position + size.x * right - size.y * up, 1.0f);
    vertices[1] = float4(position + size.x * right + size.y * up, 1.0f);
    vertices[2] = float4(position - size.x * right - size.y * up, 1.0f);
    vertices[3] = float4(position - size.x * right + size.y * up, 1.0f);
}

[maxvertexcount(4)]
void GSFixUpBillboard(point VS_BILLBOARD_INPUT input[1], inout TriangleStream<GS_BILLBOARD_OUTPUT> out_stream)
{
    float3 look, right, up;
    float4 vertices[4];
    
    GetFixUpBillboardConrers(input[0].position, input[0].size * 0.5f, vertices, look, right, up);
    
    GS_BILLBOARD_OUTPUT output;
    
    output.resource_index = input[0].resource_index;
    
    for (int i = 0; i < 4; ++i)
    {
        output.sv_position = mul(mul(vertices[i], g_view), g_projection);
        output.normal = look;
        output.uv = s_billboard_uvs[i] + input[0].coordinate;
        
        out_stream.Append(output);
    }
}

PS_GBUFFER_OUTPUT PSBillboard(GS_BILLBOARD_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    float4 base_color = g_texture_data[input.resource_index].Sample(g_sampler_point_wrap, input.uv);
    
    //clip(base_color.a - 0.33f);
    output.base_color = base_color;
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    
    return output;
}

#endif // __BILLBOARD_HLSL__