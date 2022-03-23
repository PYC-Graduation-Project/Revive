#include "Resource.hlsl"
#include "Common.hlsl"

#ifndef __OPAQUE_HLSL__
#define __OPAQUE_HLSL__

struct PS_GBUFFER_OUTPUT
{
    float4 base_color : SV_TARGET0;
    float4 normal : SV_TARGET1;
};

struct VS_OPAQUE_MESH_IN
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

struct VS_OPAQUE_MESH_OUT
{
    float4 sv_position : SV_POSITION;
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

VS_OPAQUE_MESH_OUT VSOpaqueMesh(VS_OPAQUE_MESH_IN input, uint instance_id : SV_InstanceID)
{
    VS_OPAQUE_MESH_OUT output;
    
    InstanceData i_data = g_instance_data[instance_id];
    
    float4 position = mul(float4(input.position, 1.0f), i_data.world);
    output.position = position.xyz;
    output.sv_position = mul(position, g_view_projection);
    output.normal = normalize(mul(input.normal, (float3x3) i_data.world_inverse_transpose));
    output.tangent = normalize(mul(input.tangent, (float3x3) i_data.world_inverse_transpose));
    output.bitangent = normalize(mul(input.bitangent, (float3x3) i_data.world_inverse_transpose));
    output.uv = input.uv;
    
    return output;
}

[earlydepthstencil]
PS_GBUFFER_OUTPUT PSOpaqueMesh(VS_OPAQUE_MESH_OUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    MaterialData material_data = g_material_data[g_material_index];
    if(material_data.diffuse_texture_index >= 0)
        output.base_color = g_texture_data[material_data.diffuse_texture_index].Sample(g_sampler_point_wrap, input.uv);
    else
        output.base_color = g_material_data[g_material_index].base_color;
    
    float3 normal;
    if(material_data.normal_texture_index >= 0)
        normal = GetNormalFromNormalMap(g_texture_data[material_data.normal_texture_index].Sample(g_sampler_point_wrap, input.uv).xyz,
        input.normal, input.tangent, input.bitangent);
    else
        normal = input.normal.xyz;

    output.normal = float4(normal + 1.0f * 0.5f, 1.0f);
    
    return output;
}

#endif // __OPAQUE_HLSL__