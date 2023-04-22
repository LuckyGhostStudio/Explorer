#pragma once

#include <xhash>

namespace Explorer
{
	/// <summary>
	/// ����Ψһ��ʶ
	/// </summary>
	class UUID
	{
	private:
		uint64_t m_UUID;
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	};
}

namespace std
{
	/// <summary>
	/// UUID���͵Ĺ�ϣ
	/// </summary>
	template<>
	struct hash<Explorer::UUID>
	{
		std::size_t operator()(const Explorer::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}