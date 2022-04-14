#include "opaque.hlsl"

struct VS_SKELETAL_MESH_IN
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float4 weights : BONEWEIGHT;
	int4 indices : BONEINDEX;
};

struct VS_SKELETAL_MESH_OUT
{
	float4 sv_position : SV_POSITION;
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};


VS_SKELETAL_MESH_OUT VSSkeletalMesh(VS_SKELETAL_MESH_IN input, uint instance_id : SV_InstanceID)
{
    VS_SKELETAL_MESH_OUT output;

	InstanceData i_data = g_instance_data[instance_id];
	
    float3 pos = float3(0.0f, 0.0f, 0.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);
	
	[unroll]
    for (uint i = 0; i < 4; ++i)
    {
        SkeletalData skeletal_data = g_bone_transform_data[i_data.additional_info + input.indices[i]];
        pos += mul(float4(input.position, 1.0f), skeletal_data.bone_transform).xyz * input.weights[i];
        normal += mul(input.normal, (float3x3)skeletal_data.bone_transform) * input.weights[i];
    }
	
    float4 final_position = mul(float4(pos, 1.0f), i_data.world);
	
    output.position = pos;
    output.sv_position = mul(mul(final_position, g_view), g_projection);
    output.normal = normalize(mul(normal, (float3x3) i_data.world_inverse_transpose));

	output.uv = input.uv;

	return output;
}

PS_GBUFFER_OUTPUT PSSkeletalMesh(VS_SKELETAL_MESH_OUT input)
{
    PS_GBUFFER_OUTPUT output;
    
    MaterialData material_data = g_material_data[g_material_index];
    
    output.base_color = g_texture_data[material_data.diffuse_texture_index].Sample(g_sampler_point_wrap, input.uv);
    output.normal = float4(input.normal.xyz + 1.0f * 0.5f, 1.0f);
    output.additional_info = float4(material_data.roughness, material_data.metallic, 1.0f, 1.0f);
    
    return output;
}

//
// Skeletal Mesh For Shadow
//

struct VS_SKELETAL_MESH_FOR_SHADOW_IN
{
    float3 position : POSITION;
    float4 weights : BONEWEIGHT;
    int4 indices : BONEINDEX;
};

VS_SHADOW_OUT VSSkeletalMeshForShadow(VS_SKELETAL_MESH_FOR_SHADOW_IN input, uint instance_id : SV_InstanceID)
{
    VS_SHADOW_OUT output;

    InstanceData i_data = g_instance_data[instance_id];
	
    float3 pos = 0.0f;
	
	[unroll]
    for (uint i = 0; i < 4; ++i)
    {
        SkeletalData skeletal_data = g_bone_transform_data[i_data.additional_info + input.indices[i]];
        pos += mul(float4(input.position, 1.0f), skeletal_data.bone_transform).xyz * input.weights[i];
    }
	
    float4 final_position = mul(float4(pos, 1.0f), i_data.world);
    output.sv_position = mul(final_position, g_view_projection);

    return output;
}

//
// Skeletal Mesh For Cube Shadow
//

float4 VSSkeletalMeshForShadowCube(VS_SKELETAL_MESH_FOR_SHADOW_IN input, uint instance_id : SV_InstanceID) : SV_POSITION
{
    InstanceData i_data = g_instance_data[instance_id];
	
    float3 pos = 0.0f;
	
	[unroll]
    for (uint i = 0; i < 4; ++i)
    {
        SkeletalData skeletal_data = g_bone_transform_data[i_data.additional_info + input.indices[i]];
        pos += mul(float4(input.position, 1.0f), skeletal_data.bone_transform).xyz * input.weights[i];
    }
	
    return mul(float4(pos, 1.0f), i_data.world);
}

[maxvertexcount(18)]
void GSSkeletalMeshForShadowCube(triangle float4 input[3] : SV_POSITION, inout TriangleStream<GS_SHADOW_CUBE_OUT> out_stream)
{
    GS_SHADOW_CUBE_OUT output;
    
    [unroll]
    for (int face = 0; face < 6; ++face)
    {
        output.render_target_index = face;
        
        [unroll]
        for (int i = 0; i < 3; ++i)
        {
            output.sv_position = mul(input[i], g_cube_view_projection[face]);
            out_stream.Append(output);
        }

        out_stream.RestartStrip();
    }
}