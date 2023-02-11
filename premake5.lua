workspace "Explorer"		--解决方案名称
	architecture "x64"	--体系结构

	configurations		--配置
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"	--输出目录

--包含目录列表
IncludeDir = {}
IncludeDir["GLFW"] = "Explorer/vendor/GLFW/include"

include "Explorer/vendor/GLFW"		--包含GLFW目录

project "Explorer"			--项目
	location "Explorer"	--相对路径
	kind "SharedLib"	--动态库
	language "C++"		--语言

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	--目标目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")	--中间目录

	pchheader "exppch.h"	--预编译头文件
	pchsource "Explorer/src/exppch.cpp"

	files				--文件
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs			--附加包含目录
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",			--引用GLFW
		"opengl32.lib"
	}

	filter "system:windows"	--windows系统
		cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"	--sdk版本

		defines			--宏
		{
			"EXP_PLATFORM_WINDOWS",
			"EXP_BUILD_DLL",
			"EXP_ENABLE_ASSERTS"
		}

		postbuildcommands -- build后的自定义命令
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox") --拷贝引擎dll库到sanbox.exe的同一目录下
        }

	filter "configurations:Debug"	--Debug配置
        defines "EXP_DEBUG"
		buildoptions "/MDd"			--多线程调试dll
        symbols "on"

    filter "configurations:Release"	--Release配置
        defines "EXP_RELEASE"
		buildoptions "/MD"			--多线程dll
        optimize "on"

    filter "configurations:Dist"	--Dist配置
        defines "EXP_DIST"
		buildoptions "/MD"
        optimize "on"

project "Sandbox"		--项目
	location "Sandbox"	--相对路径
	kind "ConsoleApp"	--控制台应用
	language "C++"		--语言

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	--目标目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")	--中间目录

	files				--文件
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs			--附加包含目录
	{
		"Explorer/vendor/spdlog/include",
		"Explorer/src",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"Explorer"			--引用Explorer
	}

	filter "system:windows"	--windows系统
		cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"	--sdk版本

		defines			--宏
		{
			"EXP_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"	--Debug配置
        defines "EXP_DEBUG"
		buildoptions "/MDd"			--多线程调试dll
        symbols "on"

    filter "configurations:Release"	--Release配置
        defines "EXP_RELEASE"
		buildoptions "/MD"			--多线程dll
        optimize "on"

    filter "configurations:Dist"	--Dist配置
        defines "EXP_DIST"
		buildoptions "/MD"			--多线程dll
        optimize "on"