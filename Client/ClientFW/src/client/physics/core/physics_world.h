#pragma once

namespace client_fw
{
	class CollisionChecker;

	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();
	
		PhysicsWorld(const PhysicsWorld&) = delete;
		PhysicsWorld& operator=(const PhysicsWorld&) = delete;

		bool Initialize();
		void Shutdown();
		void Update(float delta_time);
		
	private:
		UPtr<CollisionChecker> m_collision_checker;
	};
}



