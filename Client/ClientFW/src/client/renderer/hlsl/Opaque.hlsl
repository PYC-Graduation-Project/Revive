#ifndef __OPAQUE_HLSL__
#define __OPAQUE_HLSL__

#define MASKED_ALPHA        0.333333f

struct InstanceData
{
    matrix world;
    matrix world_inverse_transpose;
    uint bone_start_index;
};

struct MaterialData
{
    float4 base_color;
    int diffuse_texture_index;
};

struct SkeletalData
{
    matrix bone_transform;
};

StructuredBuffer<InstanceData> g_instance_data : register(t0, space0);
StructuredBuffer<MaterialData> g_material_data : register(t1, space0);
StructuredBuffer<SkeletalData> g_bone_transform_data : register(t2, space0);
Texture2D g_texture_data[] : register(t0, space1);

SamplerState g_sampler_point_wrap : register(s0);

cbuffer cbMaterialIndexData : register(b0, space0)
{
    uint g_material_index;
};

cbuffer cbCameraData : register(b1, space0)
{
    matrix g_view;
    matrix g_projection;
    matrix g_view_projection;
    float3 g_camera_pos;
    uint g_final_texture_index;
    uint4 g_gbuffer_texture_indices;
}

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
};

struct VS_OPAQUE_MESH_OUT
{
    float4 sv_position : SV_POSITION;
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OPAQUE_MESH_OUT VSOpaqueMesh(VS_OPAQUE_MESH_IN input, uint instance_id : SV_InstanceID)
{
    VS_OPAQUE_MESH_OUT output;
    
    InstanceData i_data = g_instance_data[instance_id];
    
    float4 position = mul(float4(input.position, 1.0f), i_data.world);
    output.position = position.xyz;
    output.sv_position = mul(position, g_view_projection);
    output.normal = mul(input.normal, (float3x3)i_data.world_inverse_transpose);
    output.normal = normalize(output.normal);
    output.uv = input.uv;
    
    return output;
}

[earlydepthstencil]
PS_GBUFFER_OUTPUT PSOpaqueMesh(VS_OPAQUE_MESH_OUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    MaterialData material_data = g_material_data[g_material_index];
    if(material_data.diffuse_texture_index >= 0)
    {
        output.base_color = g_texture_data[material_data.diffuse_texture_index].Sample(g_sampler_point_wrap, input.uv);
    }
    else
    {
        output.base_color = g_material_data[g_material_index].base_color;
    }
    
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    
    return output;
}

#endif // __OPAQUE_HLSL__