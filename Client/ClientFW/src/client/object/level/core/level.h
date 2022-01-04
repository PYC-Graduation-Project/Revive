#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	struct EventKeyInfo;

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
		void RegisterPressedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		void RegisterReleasedEvent(std::string_view name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);

	private:
		void RegisterInputEvent(std::string_view name);

	protected:
		std::string_view m_name;
		eLevelState m_level_state;

	private:
		std::vector<std::string_view> m_registered_input_event;

	public:
		std::string_view GetName() const { return m_name; }
		eLevelState GetLevelState() const { return m_level_state; }
		void SetLevelState(eLevelState level_state) { m_level_state = level_state; }
	};
}

