#pragma once

namespace client_fw
{
	class MessageEventInfo;
	class Actor;

	class PacketHelper
	{
	public:
		static void RegisterPacketEventToActor(SPtr<MessageEventInfo>&& message, UINT id);
		static void RegisterPacketEventToLevel(SPtr<MessageEventInfo>&& message);
		static void RegisterPacketEventToServer(SPtr<MessageEventInfo>&& message);

		static void ConnectActorToServer(const SPtr<Actor>& actor, UINT id);
		static SPtr<Actor> DisconnectActorFromServer(UINT id);
	};

}

