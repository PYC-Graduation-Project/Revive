workspace "ReviveServer"
	architecture "x64"
	startproject "ReviveServer"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

IncludeDir = {}

group "Dependencies"
group ""

include "ReviveServer"
