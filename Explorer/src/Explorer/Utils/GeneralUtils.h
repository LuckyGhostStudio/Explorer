#pragma once

namespace Explorer
{
	class GeneralUtils
	{
	public:
		/// <summary>
		/// 返回文件类型（扩展名.xxx）
		/// </summary>
		/// <param name="filename">文件名</param>
		/// <returns>文件类型</returns>
		static std::string GetFileType(const std::string& filename);
	};
}