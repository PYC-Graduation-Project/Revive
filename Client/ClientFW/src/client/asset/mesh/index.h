#pragma once

namespace client_fw
{
	class Index
	{
	public:
		Index();
		Index(UINT index);

		UINT GetPosition() const { return m_index; }
		void SetIndex(UINT index) { m_index = index; }


	protected:
		UINT m_index;

	};
}
