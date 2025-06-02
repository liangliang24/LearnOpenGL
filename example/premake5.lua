project "Example"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "../core/%{IncludeDir.stb_image}/**.h",
        "../core/%{IncludeDir.stb_image}/**.cpp",
    }

    includedirs
    {
        "src",
        "../core/%{IncludeDir.GLFW}",
        "../core/%{IncludeDir.GLAD}",
        "../core/%{IncludeDir.glm}",
        "../core/%{IncludeDir.spdlog}",
        "../core/%{IncludeDir.ImGui}",
        "../core/%{IncludeDir.stb_image}",
        "../core/src"
    }

    links
    {
        "Core"
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