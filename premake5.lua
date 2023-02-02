workspace "Explorer"		--�����������
	architecture "x64"	--��ϵ�ṹ

	configurations		--����
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"	--���Ŀ¼

project "Explorer"			--��Ŀ
	location "Explorer"	--���·��
	kind "SharedLib"	--��̬��
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
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"	--windowsϵͳ
		cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"	--sdk�汾

		defines			--��
		{
			"EXP_PLATFORM_WINDOWS",
			"EXP_BUILD_DLL"
		}

		postbuildcommands -- build����Զ�������
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox") --��������dll�⵽sanbox.exe��ͬһĿ¼��
        }

	filter "configurations:Debug"	--Debug����
        defines "EXP_DEBUG"
        symbols "on"

    filter "configurations:Release"	--Release����
        defines "EXP_RELEASE"
        optimize "on"

    filter "configurations:Dist"	--Dist����
        defines "EXP_DIST"
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
		"Explorer/src"
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
        symbols "on"

    filter "configurations:Release"	--Release����
        defines "EXP_RELEASE"
        optimize "on"

    filter "configurations:Dist"	--Dist����
        defines "EXP_DIST"
        optimize "on"