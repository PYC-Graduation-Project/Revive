#include "stdafx.h"
#include "client/asset/mesh/mesh_loader.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/mesh/vertex.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/mesh/material.h"
#include "client/physics/core/bounding_mesh.h"
#include "client/physics/collision/mesh_bounding_tree.h"

namespace client_fw
{
	SPtr<Mesh> MeshLoader::LoadMesh(const std::string& path, const std::string& extension)
	{
		SPtr<Mesh> mesh = nullptr;

		switch (HashCode(extension.c_str()))
		{
		case HashCode(".obj"):
			mesh = LoadObj(path, extension);
			break;
		default:
			LOG_ERROR("Files in {0} format cannot be supported", extension);
			break;
		}

		return mesh;
	}

	struct CombineData
	{
		std::string mtl_name;
		std::vector<UINT> pos_indices;
		std::vector<UINT> tex_indices;
		std::vector<UINT> normal_indices;
	};

	SPtr<StaticMesh> MeshLoader::LoadObj(const std::string& path, const std::string& extension)
	{
		std::ifstream obj_file(path);
		
		if (obj_file.is_open() == false)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
			return nullptr;
		}

		std::string parent_path = file_help::GetParentPathFromPath(path);
		std::string stem = file_help::GetStemFromPath(path);

		UINT lod = 0;
		SPtr<StaticMesh> mesh = CreateSPtr<StaticMesh>();

		while (lod < 4)
		{
			std::vector<Vec3> positions;
			std::vector<Vec3> normals;
			std::vector<Vec2> tex_coords;
			std::map<std::string, SPtr<Material>> materials;
			std::vector<CombineData> combine_data;
			UINT combine_data_index = 0;

			std::stringstream ss;
			std::string line;
			std::string prefix;

			Vec3 temp_vec;
			UINT temp_uint = 0;
			std::string temp_string;

			while (std::getline(obj_file, line))
			{
				ss.clear();
				prefix.clear();
				ss.str(line);
				ss >> prefix;

				switch (HashCode(prefix.c_str()))
				{
				case HashCode("mtllib"):
				{
					ss >> temp_string;
					materials = AssetStore::LoadMaterials(file_help::GetParentPathFromPath(path) + "\\" + temp_string);
					break;
				}
				case HashCode("v"):
					ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
					temp_vec.z *= -1.0f;
					positions.emplace_back(std::move(temp_vec));
					break;
				case HashCode("vt"):
					ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
					temp_vec.y = 1.0f - temp_vec.y;
					tex_coords.emplace_back(Vec2{ temp_vec.x, temp_vec.y });
					break;
				case HashCode("vn"):
					ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
					temp_vec.z *= -1.0f;
					normals.emplace_back(std::move(temp_vec));
					break;
				case HashCode("usemtl"):
				{
					ss >> temp_string;
					auto iter = std::find_if(combine_data.cbegin(), combine_data.cend(),
						[temp_string](const CombineData& data) {
							return data.mtl_name == temp_string;
						});
					if (iter == combine_data.cend())
					{
						combine_data.push_back({ temp_string });
						combine_data_index = static_cast<UINT>(combine_data.size()) - 1;
					}
					else
					{
						combine_data_index = static_cast<UINT>(std::distance(combine_data.cbegin(), iter));
					}
				}
				break;
				case HashCode("f"):
				{
					int count = 0;

					while (ss >> temp_uint)
					{
						if (count == 0)
							combine_data[combine_data_index].pos_indices.emplace_back(temp_uint - 1);
						else if (count == 1)
							combine_data[combine_data_index].tex_indices.emplace_back(temp_uint - 1);
						else if (count == 2)
							combine_data[combine_data_index].normal_indices.emplace_back(temp_uint - 1);

						if (ss.peek() == '/')
						{
							++count;
							ss.ignore();
						}
						else if (ss.peek() == ' ')
						{
							ss.ignore(1, ' ');
							count = 0;
						}
					}
				}
				break;
				default:
					break;
				}
			}

			mesh->CreateDataForLodMesh(lod);

			UINT vertex_count = 0;
			for (const auto& data : combine_data)
				vertex_count += static_cast<UINT>(data.pos_indices.size());

			std::vector<TextureLightVertex> vertices(vertex_count);
			std::vector<Triangle> triangles(vertex_count / 3);
			size_t tri_index = 0;

			vertex_count = 0;
			for (const auto& data : combine_data)
			{
				UINT count = static_cast<UINT>(data.pos_indices.size());
				mesh->AddInstanceInfo(lod, { count, vertex_count });
				mesh->AddMaterial(lod, std::move(materials[data.mtl_name]));

				for (UINT i = 0; i < count; ++i)
				{
					UINT index = i + vertex_count;
					if (index % 3 == 0)
						index += 2;
					else if (index % 3 == 2)
						index -= 2;

					vertices[index].SetPosition(positions[data.pos_indices[i]]);
					vertices[index].SetTexCoord(tex_coords[data.tex_indices[i]]);
					vertices[index].SetNormal(normals[data.normal_indices[i]]);

					switch (index % 3)
					{
					case 0:	triangles[tri_index].v1 = vertices[index].GetPosition(); break;
					case 1:	triangles[tri_index].v2 = vertices[index].GetPosition(); break;
					case 2:	triangles[tri_index].v3 = vertices[index].GetPosition(); break;
					}

					if (index % 3 == 0)
					{
						const auto& triangle = triangles[tri_index];
						Vec3 normal = vec3::Cross(triangle.v3 - triangle.v1, triangle.v2 - triangle.v1, true);

						if (normal == vec3::ZERO)
							triangles.pop_back();
						else
							++tri_index;
					}
				}

				vertex_count += count;
			}

			const UINT vertices_size = vertex_count * sizeof(TextureLightVertex);
			if (mesh->CreateVertexBufferBlob(lod, vertices_size) == false)
			{
				LOG_ERROR("Could not create blob for vertex");
				return nullptr;
			}
			CopyMemory(mesh->GetVertexBufferBlob(lod)->GetBufferPointer(), vertices.data(), vertices_size);

			if (lod == 0)
			{
#ifdef SHOW_TREE_INFO
				LOG_INFO("Triangle Tree : {0}", path);
#endif // SHOW_TREE_INFO
				mesh->SetOrientBox(BOrientedBox(std::move(positions)));
				auto bounding_tree = CreateSPtr<StaticMeshBoundingTree>();
				bounding_tree->Initialize(std::move(triangles));
				mesh->SetBoundingTree(std::move(bounding_tree));
			}

			++lod;
			std::string lod_path = parent_path + "/" + stem + "_lod" + std::to_string(lod) + extension;
			obj_file = std::ifstream(lod_path);

			if (obj_file.is_open() == false)
				break;
		}

		return mesh;
	}
}
