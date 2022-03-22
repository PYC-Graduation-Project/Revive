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
    SkeletalData skeletal_data = g_transform_data[instance_id];
	
    float3 pos = float3(0.0f, 0.0f, 0.0f);
    float3 normal = float3(0.0f, 0.0f, 0.0f);
	
	[unroll]
    for (uint i = 0; i < 4; ++i)
    {
        pos += mul(float4(input.position, 1.0f), skeletal_data.bone_trans[input.indices[i]]).xyz * input.weights[i];
		normal += mul(input.normal, (float3x3) skeletal_data.bone_trans[input.indices[i]]) * input.weights[i];
    }
    
	output.position = pos;
    output.sv_position = mul(mul(float4(pos, 1.0f), g_view), g_projection);
    output.normal = normal;

	output.uv = input.uv;

	return output;
}

float4 PSSkeletalMesh(VS_SKELETAL_MESH_OUT input) : SV_TARGET
{
 //return float4(input.normal, 1.0f);
    input.normal = normalize(input.normal);
	
	MaterialData material_data = g_material_data[g_material_index];
if (material_data.diffuse_texture_index >= 0)
{
	return g_texture_data[material_data.diffuse_texture_index].Sample(g_sampler_point_wrap, input.uv);

}
else
{
	return g_material_data[g_material_index].base_color;
}
}