#pragma once

namespace client_fw
{
	class MessageEventInfo
	{
	public:
		MessageEventInfo(const std::string& name)
			: m_name(name) {}
		
	protected:
		std::string m_name;

	public:
		const std::string& GetName() const { return m_name; }
	};

	bool operator<(const MessageEventInfo& msg1, const MessageEventInfo& msg2);
}


