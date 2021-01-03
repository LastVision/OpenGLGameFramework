
project "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Framework/vendor/spdlog/include",
        "%{wks.location}/Framework/src",
        "%{wks.location}/Framework/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}"
    }

    links
    {
        "Framework"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "GF_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "GF_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "GF_DIST"
        runtime "Release"
        optimize "on"