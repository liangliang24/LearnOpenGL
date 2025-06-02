workspace "LearnOpenGL"
    architecture "x64"
    startproject "Example"

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
IncludeDir["stb_image"]="vendor/stb_image"

include "core/vendor/glfw"
include "core/vendor/glad"
include "core/vendor/imgui"

include "core"
include "example"
