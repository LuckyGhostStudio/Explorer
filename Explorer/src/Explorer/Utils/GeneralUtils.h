#pragma once

namespace Explorer
{
	class GeneralUtils
	{
	public:
		/// <summary>
		/// �����ļ����ͣ���չ��.xxx��
		/// </summary>
		/// <param name="filename">�ļ���</param>
		/// <returns>�ļ�����</returns>
		static std::string GetFileType(const std::string& filename);
	};
}