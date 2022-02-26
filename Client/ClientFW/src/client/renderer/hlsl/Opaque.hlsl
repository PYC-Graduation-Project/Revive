
struct InstanceData
{
	matrix world;
	matrix world_inverse_transpose;
};

struct MaterialData
{
	float4 base_color;
	int diffuse_texture_index;
};

StructuredBuffer<InstanceData> g_instance_data : register(t0, space0);
StructuredBuffer<MaterialData> g_material_data : register(t1, space0);
Texture2D g_texture_data[] : register(t2, space0);

SamplerState g_sampler_point_wrap : register(s0);

cbuffer cbMaterialIndexData : register(b0, space0)
{
	uint g_material_index;
};

cbuffer cbCameraData : register(b1, space0)
{
	matrix g_view;
	matrix g_projection;
	float3 g_camera_pos;
}

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
	//float4 position = float4(input.position, 1.0f);
	output.position = position.xyz;
	output.sv_position = mul(mul(position, g_view), g_projection);
	output.normal = mul(input.normal, (float3x3)i_data.world);
	output.normal = normalize(output.normal);
	output.uv = input.uv;

	return output;
}

[earlydepthstencil]
float4 PSOpaqueMesh(VS_OPAQUE_MESH_OUT input) : SV_TARGET
{
	MaterialData material_data = g_material_data[g_material_index];
	if (material_data.diffuse_texture_index >= 0)
	{
		return g_texture_data[material_data.diffuse_texture_index].Sample(g_sampler_point_wrap, input.uv);
		//return g_material_data[g_material_index].base_color;

	}
	else
	{
		return g_material_data[g_material_index].base_color;
	}
}
#define ANIMATION_BONES 128

cbuffer cbBoneOffsetData : register(b2, space0)
{
	float4x4 g_bone_offsets[ANIMATION_BONES];
}

cbuffer cbBoneTransData : register(b3, space0)
{
	float4x4 g_bone_trans[ANIMATION_BONES];
}

struct VS_DIFFUSE_IN
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	int4 indices : BONEINDEX;
	float4 weights : BONEWEIGHT;
};

struct VS_DIFFUSE_OUT
{
	float4 sv_position : SV_POSITION;
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};


VS_DIFFUSE_OUT VSDiffuse(VS_DIFFUSE_IN input, uint instance_id : SV_InstanceID)
{
	VS_DIFFUSE_OUT output;

	InstanceData i_data = g_instance_data[instance_id];
	float4x4 vertex_to_bone_world = (float4x4)0.0f;
	float4 weight = float4(1.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 4; i++)
	{
		vertex_to_bone_world +=  weight[i] * mul(g_bone_offsets[input.indices[i]],g_bone_trans[input.indices[i]]);
	}
	float4 position = mul(float4(input.position, 1.0f), i_data.world);
	//float4 position = mul(mul(float4(input.position, 1.0f), i_data.world), vertex_to_bone_world);
	//float4 position = mul(float4(input.position, 1.0f), vertex_to_bone_world);
	//float4 position = mul(mul(float4(input.position, 1.0f), vertex_to_bone_world), i_data.world);
	//float4 position = float4(input.position, 1.0f);
	output.position = position.xyz;
	output.sv_position = mul(mul(position, g_view), g_projection);
	//.normal = mul(input.normal, (float3x3)i_data.world);
	//output.normal = normalize(output.normal);
	output.normal = float3(input.weights.xyz);
	//output.position = mul(float4(input.position, 1.0f), g_instance_data[instance_id].world);
	//output.position.z = instance_id / 100.0f + 0.01f;
	output.uv = input.uv;

	return output;
}

float4 PSDiffuse(VS_DIFFUSE_OUT input) : SV_TARGET
{
	MaterialData material_data = g_material_data[g_material_index];
	//return float4(1.0f,0.0f,0.0f, 1.0f);
	if (material_data.diffuse_texture_index >= 0)
	{
		return g_texture_data[material_data.diffuse_texture_index].Sample(g_sampler_point_wrap, input.uv);

	}
	else
	{
		return g_material_data[g_material_index].base_color;
	}
}