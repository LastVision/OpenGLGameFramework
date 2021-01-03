include "./vendor/premake/premake_customization/solution_items.lua"

workspace "GameFramework"
	architecture "x86_64"
	startproject "Game"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Framework/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Framework/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Framework/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Framework/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Framework/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Framework/vendor/entt/include"

group "Dependencies"
	include "vendor/premake"
	include "Framework/vendor/GLFW"
	include "Framework/vendor/Glad"
	include "Framework/vendor/imgui"
group ""

include "Framework"
include "Game"