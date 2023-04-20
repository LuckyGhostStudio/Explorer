workspace "Explorer"	--解决方案名称
	architecture "x64"	--体系结构
	startproject "Editor"	--启动项目

	configurations		--配置
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"	--输出目录

--包含目录列表
IncludeDir = {}
IncludeDir["Box2D"] = "Explorer/vendor/Box2D/include"
IncludeDir["GLFW"] = "Explorer/vendor/GLFW/include"
IncludeDir["GLAD"] = "Explorer/vendor/GLAD/include"
IncludeDir["ImGui"] = "Explorer/vendor/imgui"
IncludeDir["glm"] = "Explorer/vendor/glm"
IncludeDir["stb_image"] = "Explorer/vendor/stb_image"
IncludeDir["entt"] = "Explorer/vendor/entt/include"
IncludeDir["yaml_cpp"] = "Explorer/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Explorer/vendor/ImGuizmo"
IncludeDir["Assimp"] = "packages/Assimp.3.0.0/build/native/include"

--库目录列表
LibDir = {}
LibDir["Assimp"] = "packages/Assimp.3.0.0/build/native/lib/x64"

group "Dependencies"
	include "Explorer/vendor/Box2D"		--包含Box2D目录
	include "Explorer/vendor/GLFW"		--包含GLFW目录
	include "Explorer/vendor/GLAD"		--包含GLAD目录
	include "Explorer/vendor/imgui"		--包含imgui目录
	include "Explorer/vendor/yaml-cpp"

group ""

project "Explorer"		--项目
	location "Explorer"	--相对路径
	kind "StaticLib"	--静态库
	language "C++"		--语言
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	--目标目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")	--中间目录

	pchheader "exppch.h"	--预编译头文件
	pchsource "Explorer/src/exppch.cpp"

	files				--文件
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs			--附加包含目录
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Assimp}"
	}

	libdirs		--附加库目录
	{
		"%{LibDir.Assimp}"
	}

	links
	{
		"Box2D",		--引用Box2D
		"GLFW",			--引用GLFW
		"GLAD",			--引用GLAD
		"ImGui",		--引用imgui
		"yaml-cpp",		--引用yaml-cpp
		"assimp.lib",	--引用assimp
		"opengl32.lib"
	}

	filter "files:Explorer/vendor/ImGuizmo/**.cpp"
	flags {"NoPCH"}		--该cpp文件不使用预编译头文件

	filter "system:windows"	--windows系统
        systemversion "latest"	--sdk版本

		defines			--宏
		{
			--"EXP_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
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
	cppdialect "C++17"
	staticruntime "on"

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
		"Explorer/vendor",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Explorer"			
	}

	filter "system:windows"	--windows
        systemversion "latest"	

		defines			
		{
			--"EXP_PLATFORM_WINDOWS"
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

project "Editor"		--项目
	location "Editor"	--相对路径
	kind "ConsoleApp"	--控制台应用
	language "C++"		--语言
	cppdialect "C++17"
	staticruntime "on"

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
		"Explorer/vendor",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Assimp}"
	}

	links
	{
		"Explorer"			
	}

	filter "system:windows"	--windows
        systemversion "latest"	

		defines			
		{
			--"EXP_PLATFORM_WINDOWS"
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