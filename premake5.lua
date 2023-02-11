workspace "Explorer"		--�����������
	architecture "x64"	--��ϵ�ṹ

	configurations		--����
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"	--���Ŀ¼

--����Ŀ¼�б�
IncludeDir = {}
IncludeDir["GLFW"] = "Explorer/vendor/GLFW/include"
IncludeDir["GLAD"] = "Explorer/vendor/GLAD/include"
IncludeDir["ImGui"] = "Explorer/vendor/imgui"

include "Explorer/vendor/GLFW"		--����GLFWĿ¼
include "Explorer/vendor/GLAD"		--����GLADĿ¼
include "Explorer/vendor/imgui"		--����imguiĿ¼

project "Explorer"			--��Ŀ
	location "Explorer"	--���·��
	kind "SharedLib"	--��̬��
	language "C++"		--����

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	--Ŀ��Ŀ¼
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")	--�м�Ŀ¼

	pchheader "exppch.h"	--Ԥ����ͷ�ļ�
	pchsource "Explorer/src/exppch.cpp"

	files				--�ļ�
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs			--���Ӱ���Ŀ¼
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",			--����GLFW
		"GLAD",			--����GLAD
		"ImGui",		--����ImGui
		"opengl32.lib"
	}

	filter "system:windows"	--windowsϵͳ
		cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"	--sdk�汾

		defines			--��
		{
			"EXP_PLATFORM_WINDOWS",
			"EXP_BUILD_DLL",
			"EXP_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands -- build����Զ�������
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox") --��������dll�⵽sanbox.exe��ͬһĿ¼��
        }

	filter "configurations:Debug"	--Debug����
        defines "EXP_DEBUG"
		buildoptions "/MDd"			--���̵߳���dll
        symbols "on"

    filter "configurations:Release"	--Release����
        defines "EXP_RELEASE"
		buildoptions "/MD"			--���߳�dll
        optimize "on"

    filter "configurations:Dist"	--Dist����
        defines "EXP_DIST"
		buildoptions "/MD"
        optimize "on"

project "Sandbox"		--��Ŀ
	location "Sandbox"	--���·��
	kind "ConsoleApp"	--����̨Ӧ��
	language "C++"		--����

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")	--Ŀ��Ŀ¼
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")	--�м�Ŀ¼

	files				--�ļ�
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs			--���Ӱ���Ŀ¼
	{
		"Explorer/vendor/spdlog/include",
		"Explorer/src",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"Explorer"			--����Explorer
	}

	filter "system:windows"	--windowsϵͳ
		cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"	--sdk�汾

		defines			--��
		{
			"EXP_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"	--Debug����
        defines "EXP_DEBUG"
		buildoptions "/MDd"			--���̵߳���dll
        symbols "on"

    filter "configurations:Release"	--Release����
        defines "EXP_RELEASE"
		buildoptions "/MD"			--���߳�dll
        optimize "on"

    filter "configurations:Dist"	--Dist����
        defines "EXP_DIST"
		buildoptions "/MD"			--���߳�dll
        optimize "on"