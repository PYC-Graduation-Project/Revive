project "ReviveServer"--프로젝트 이름
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"lib/**.lib",
		"src/**.lua",
	}

	includedirs--추가포함 디렉터리
	{
		--"%{wks.location}/ClientFW/src",
		--"%{wks.location}/ClientFW/vendor/spdlog/include",
		"%{wks.location}/ReviveServer/src",--필수아닌 선택
	}
	

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "ReviveServer_DEBUG"--프로젝트 이름 요거랑 맞추기
		symbols "On"

	filter "configurations:Release"
		defines "ReviveServer_RELEASE"
		optimize "On"