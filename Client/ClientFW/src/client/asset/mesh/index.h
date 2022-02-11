#pragma once

namespace client_fw
{
	class Index
	{
	public:
		Index();
		Index(const UINT& index);

		const UINT& GetPosition() const { return m_index; }
		void SetIndex(const UINT& index) { m_index = index; }

	protected:
		UINT m_index;

	};
}
