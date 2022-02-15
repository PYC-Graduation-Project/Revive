#include <include/client_core.h>
#include <client/input/input.h>
#include <client/renderer/core/render.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/static_mesh_actor.h>
//#include "render/shader/anim_test_actor_shader.h"
#include "object/actor/test_actor.h"
#include <client/util/octree/octree.h>
#include "object/level/player_test_level.h"

namespace anim_test
{
	PlayerTestLevel::PlayerTestLevel()
		: Level("player test level")
	{
	}

	bool PlayerTestLevel::Initialize()
	{
		m_player = CreateSPtr<DefaultPawn>();
		m_player_controller = CreateSPtr<PlayerController>();

		m_player_controller->Possess(m_player);

		SpawnActor(m_player);
		SpawnActor(m_player_controller);


		auto skel = CreateSPtr<TestActor>(eMobilityState::kMovable, "../Contents/s.rev");
		SpawnActor(skel);
		skel->SetPosition(Vec3{ 0.0f, 0.0f, 1000.0f });
		skel->SetScale(Vec3(50.0f, 50.0f, 50.0f));
		skel->SetRotation(-1.5f, 3.0f, 0.0f);
		//gunner->IsUpdatedWorldMatrix();
		//gunner->GetWorldMatrix
		//Render::RegisterGraphicsShader<RenderAnimShader>("render anim shader", eRenderLevelType::kOpaque);

		Input::SetInputMode(eInputMode::kUIAndGame);
		Input::SetHideCursor(true);

		return true;
	}

	void PlayerTestLevel::Shutdown()
	{
		//Render::UnregisterGraphicsShader("render anim shader", eRenderLevelType::kOpaque);

		Input::SetHideCursor(false);
		Input::SetClipCursor(false);
		Input::SetInputMode(eInputMode::kUIOnly);
	}

	void PlayerTestLevel::Update(float delta_time)
	{
	}
	std::vector<SPtr<VisualOctree>> PlayerTestLevel::CreateVisualOctrees() const
	{
		std::vector<SPtr<VisualOctree>> visual_octrees;
		visual_octrees.emplace_back(CreateSPtr<VisualOctree>(10000.0f));
		return visual_octrees;
	}

	std::vector<SPtr<CollisionOctree>> PlayerTestLevel::CreateCollisionOctrees() const
	{
		std::vector<SPtr<CollisionOctree>> collision_octrees;
		collision_octrees.emplace_back(CreateSPtr<CollisionOctree>(10000.0f));
		return collision_octrees;
	}
}
