project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "%{IncludeDir.stb_image}/**.h",
        "%{IncludeDir.stb_image}/**.cpp",
    }

    includedirs
    {
        "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.assimp}",
    }

    links
    {
        "GLFW",
        "opengl32.lib",
        "GLAD",
        "ImGui",
        "vendor/assimp/bin/assimp-vc143-mtd.dll"
    }

    defines
    {
        "FMT_UNICODE=0"
    }

    filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"