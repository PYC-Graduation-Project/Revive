#include "Resource.hlsl"
#include "Common.hlsl"

#ifndef __WIDGET_HLSL__
#define __WIDGET_HLSL__

struct VS_SKYCUBE_INPUT
{
    float3 position : POSITION;
};

struct VS_SKYCUBE_OUTPUT
{
    float4 sv_position : SV_POSITION;
    float3 position : POSITION;
};

VS_SKYCUBE_OUTPUT VSSkyCube(VS_SKYCUBE_INPUT input)
{
    VS_SKYCUBE_OUTPUT output;
    
    output.position = input.position;
    
    float4 pos = float4(input.position, 1.0f);
    pos.xyz += g_camera_pos;
    
    output.sv_position = mul(pos, g_view_projection).xyww;
    
    return output;
}

PS_GBUFFER_OUTPUT PSSkyCube(VS_SKYCUBE_OUTPUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    //MaterialData material_data = g_material_data[g_material_index];
    
    //output.base_color = g_texture_cube_data[g_material_index]
    //output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    //output.additional_info = float4(material_data.roughness, material_data.metallic, 1.0f, 1.0f);
    
    return output;
}

#endif // __WIDGET_HLSL__