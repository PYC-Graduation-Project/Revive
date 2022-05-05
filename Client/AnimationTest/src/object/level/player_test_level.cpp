#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/input/input.h>
#include <client/renderer/core/render.h>
#include <client/renderer/core/render_system.h>
#include <client/renderer/renderlevel/core/render_level.h>
#include <client/object/actor/default_pawn.h>
#include <client/object/actor/player_controller.h>
#include <client/object/actor/static_mesh_actor.h>
#include <client/object/component/mesh/skeletal_mesh_component.h>
#include <client/util/octree/octree.h>
#include "object/level/player_test_level.h"
#include "object/actor/test_actor.h"

namespace anim_test
{
	PlayerTestLevel::PlayerTestLevel()
		: Level("player test level")
	{
	}

	bool PlayerTestLevel::Initialize()
	{
		/*auto cube = CreateSPtr<StaticMeshActor>(eMobilityState::kMovable, "../Contents/Cube.obj");
		SpawnActor(cube);
		cube->SetPosition(Vec3{ 0,0,1000.0f });*/
		/*auto violet_idle = CreateSPtr<TestActor>(eMobilityState::kMovable, "../Contents/Revive/skel.rev","idle");
		SpawnActor(violet_idle);
		violet_idle->SetPosition(Vec3{ 10.0f, -35.0f, 1100.0f });
		violet_idle->SetScale(50.f);
		violet_idle->SetRotation(80.0f, 0.0f, 0.0f);*/
		auto ground = CreateSPtr<StaticMeshActor>(eMobilityState::kStatic,"../Revive/Contents/cliff_stone_01.obj");
		SpawnActor(ground);
		ground->SetPosition(Vec3{ 0.0f,-900.0f,1000.0f });
		ground->SetScale(12000.f);

		std::array<std::string, 7> animation_name = { "appear","attack", "death","hit","idle","run","Null" };
		//std::array<std::string, 7> animation_name = { "attack_first","attack_second", "death","hit","idle","run","Null" };
		auto skel_run = CreateSPtr<TestActor>(eMobilityState::kMovable, "../Revive/Contents/skeleton_king.rev","Null");
		SpawnActor(skel_run);
		skel_run->SetPosition(Vec3{ 0.0f, 300.0f, 1000.0f });
		skel_run->SetScale(100.f);
		skel_run->SetRotation(80.0f, 0.0f, 0.0f);

		RegisterPressedEvent("box view", { {eKey::kC} }, [this, skel_run]()->bool
		{
			LOG_INFO(skel_run->GetSkeletalMeshComponent()->GetOrientedBox()->GetExtents());
			return true;
		});
		RegisterPressedEvent("animation view", { {eKey::kN} }, [this,skel_run,animation_name]()->bool 
		{
			LOG_INFO(animation_name[m_animation_set_index]);
			skel_run->SetAnimation(animation_name[m_animation_set_index++]); 
			if (m_animation_set_index >= 7)
				m_animation_set_index = 0;
			return true;
		});
		//skel_run->SetWeapon("../Contents/Cube.obj","mount0");
		//skel_run->SetWeaponOffset(Vec3{50.f,-10.f,0.f});
		

		Input::SetInputMode(eInputMode::kGameOnly);
		Input::SetHideCursor(true);

		return true;
	}

	void PlayerTestLevel::Shutdown()
	{
		Input::SetHideCursor(false);
		Input::SetClipCursor(false);
		Input::SetInputMode(eInputMode::kUIOnly);
	}

	void PlayerTestLevel::Update(float delta_time)
	{
		/*
		static float x = -300.0f, y = 0.0f, z = 1100.0f;
		static float time = 0.0f;

		static UINT count = 1;

		time += delta_time;

		if (time >= 0.016f && count <= 1000)
		{
			for (int i = 0; i < 10; ++i)
			{
				if (i % 2 == 0)
				{
					auto police = CreateSPtr<TestActor>(eMobilityState::kMovable, "../Contents/Revive/skel.rev","run");
					police->SetPosition(Vec3{ x, y- 35.0f, z });
					SpawnActor(police);
					police->SetScale(Vec3(50.0f, 50.0f, 50.0f));
					police->SetRotation(80.0f, 0.0f, 0.0f);
					++count;
				}
				else
				{
					auto police = CreateSPtr<TestActor>(eMobilityState::kMovable, "../Contents/Revive/skel.rev","idle");
					SpawnActor(police);
					police->SetPosition(Vec3{ x, y, z });
					police->SetScale(50.f);
					police->SetScale(Vec3(50.0f, 50.0f, 50.0f));
					police->SetRotation(80.0f, 0.0f, 0.0f);
					++count;
				}
				

				z += 100.0f;
			}

			if (z > 4000.0f)
			{
				x -= 100.0f;
				z = 1000.0f;
				if (x < -3300.0f)
				{
					x = -300.0f;
					y += 100.0f;
				}
			}

			time -= 0.016f;
		}*/
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
