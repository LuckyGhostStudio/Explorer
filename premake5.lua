workspace "Explorer"	--解决方案名称
	architecture "x64"	--体系结构

	configurations		--配置
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "Sandbox"	--将Sandbox设为启动项目

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"	--输出目录

--包含目录列表
IncludeDir = {}
IncludeDir["GLFW"] = "Explorer/vendor/GLFW/include"
IncludeDir["GLAD"] = "Explorer/vendor/GLAD/include"
IncludeDir["ImGui"] = "Explorer/vendor/imgui"
IncludeDir["glm"] = "Explorer/vendor/glm"

include "Explorer/vendor/GLFW"		--包含GLFW目录
include "Explorer/vendor/GLAD"		--包含GLAD目录
include "Explorer/vendor/imgui"		--包含imgui目录

project "Explorer"		--项目
	location "Explorer"	--相对路径
	kind "SharedLib"	--动态库
	language "C++"		--语言
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	--目标目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")	--中间目录

	pchheader "exppch.h"	--预编译头文件
	pchsource "Explorer/src/exppch.cpp"

	files				--文件
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs			--附加包含目录
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",			--引用GLFW
		"GLAD",			--引用GLAD
		"ImGui",		--引用imgui
		"opengl32.lib"
	}

	filter "system:windows"	--windows系统
		cppdialect "C++17"
        systemversion "latest"	--sdk版本

		defines			--宏
		{
			"EXP_PLATFORM_WINDOWS",
			"EXP_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands --build后的自定义命令
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox") --拷贝引擎dll库到sanbox.exe的同一目录下
        }

	filter "configurations:Debug"	--Debug
        defines "EXP_DEBUG"
		runtime "Debug"
        symbols "on"

    filter "configurations:Release"	--Release
        defines "EXP_RELEASE"
		runtime "Release"
        optimize "on"

    filter "configurations:Dist"	--Dist
        defines "EXP_DIST"
		runtime "Release"
        optimize "on"

project "Sandbox"		--项目
	location "Sandbox"	--相对路径
	kind "ConsoleApp"	--控制台应用
	language "C++"		--语言
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files				
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs			
	{
		"Explorer/vendor/spdlog/include",
		"Explorer/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Explorer"			
	}

	filter "system:windows"	--windows
		cppdialect "C++17"
        systemversion "latest"	

		defines			
		{
			"EXP_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"	
        defines "EXP_DEBUG"
		runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "EXP_RELEASE"
		runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "EXP_DIST"
		runtime "Release"
        optimize "on"