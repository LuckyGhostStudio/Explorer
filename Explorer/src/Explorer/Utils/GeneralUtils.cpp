#include "exppch.h"
#include "GeneralUtils.h"

namespace Explorer
{
	std::string GeneralUtils::GetFileType(const std::string& filename)
	{
		uint32_t dotIndex = filename.find_last_of('.');

		return filename.substr(dotIndex, filename.length() - dotIndex);	//文件后缀名
	}
}