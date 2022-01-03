#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	enum class eLevelState
	{
		kInGame, kPaused, kDead
	};

	class Level : public IBaseObject
	{
	public:
		Level(std::string_view name = "level");
		virtual ~Level() = default;

		void InitializeLevel();
		virtual void Initialize() override {}

		void ShutdownLevel();
		virtual void Shutdown() override {}

		void UpdateLevel(float delta_time);
		virtual void Update(float delta_time) override {}


	protected:
		std::string_view m_name;
		eLevelState m_level_state;

	public:
		std::string_view GetName() const { return m_name; }
		eLevelState GetLevelState() const { return m_level_state; }
		void SetLevelState(eLevelState level_state) { m_level_state = level_state; }
	};
}

