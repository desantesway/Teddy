workspace "Teddy"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Teddy"

    location "Teddy"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "teddy_pch.h"
    pchsource "Teddy/src/teddy_pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include", 
        "%{prj.name}/src"
	}

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "TED_PLATFORM_WINDOWS",
            "TED_BUILD_DLL"
        }

    postbuildcommands
    { 
        ("{COPY} ../bin/" .. outputdir .. "/Teddy/*.dll ../bin/" .. outputdir .. "/KeyStar")
    }

    filter "configurations:Debug"
        defines { "TED_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "TED_RELEASE" }
        optimize "On"

    filter "configurations:Dist"
        defines { "TED_DIST" }
        optimize "On"

    filter { "action:vs*" }
        buildoptions { "/utf-8" }

project "KeyStar"

    location "KeyStar"
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

    includedirs 
    {
        "Teddy/vendor/spdlog/include", 
        "Teddy/src"
    } 

    links{ "Teddy" }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "TED_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "TED_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "TED_RELEASE" }
        optimize "On"

    filter "configurations:Dist"
        defines { "TED_DIST" }
        optimize "On"

    filter { "action:vs*" }
        buildoptions { "/utf-8" }