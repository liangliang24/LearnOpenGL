workspace "LearnOpenGL"
    architecture "x64"
    startproject "LearnOpenGL"

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir ={}
IncludeDir["GLFW"]="vendor/glfw/include"
IncludeDir["GLAD"]="vendor/glad/include"
IncludeDir["glm"]="vendor/glm"
IncludeDir["spdlog"]="vendor/spdlog/include"
IncludeDir["ImGui"]="vendor/imgui"

include "vendor/glfw"
include "vendor/glad"
include "vendor/imgui"

project "LearnOpenGL"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "GLFW",
        "opengl32.lib",
        "GLAD",
        "ImGui"
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