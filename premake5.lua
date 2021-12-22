workspace "Revive"
	architecture "x64"
	startproject "Revive"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}

group "Dependencies"
group ""

include "ClientFW"
include "Revive"