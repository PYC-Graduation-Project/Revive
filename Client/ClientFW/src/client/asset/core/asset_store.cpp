#include "stdafx.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/component/mesh/core/mesh_component.h"

namespace client_fw
{
	SPtr<Mesh> AssetStore::LoadMesh(const std::string& path)
	{
		return s_asset_manager->LoadMesh(path);
	}
}
