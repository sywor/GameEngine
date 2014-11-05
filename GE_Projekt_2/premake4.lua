

solution "q_ge_project_2"
    configurations { "Debug", "Release" }
        flags{  "NoPCH" } 
        libdirs { "lib" ,"$(LibraryPath);$(DXSDK_DIR)Lib\x86"}
        includedirs { "include" }
    
        local location_path = "solution"

        if _ACTION then
	        defines { "_CRT_SECURE_NO_WARNINGS", "NOMINMAX" }

            location_path = location_path .. "/" .. _ACTION
            location ( location_path )
            location_path = location_path .. "/projects"
        end
		
		defines { "GENERATE_LUA_API_FILE"}
	
    configuration { "Debug" }
        defines { "DEBUG" }
        flags { "Symbols", "WinMain" }
        
    configuration { "Release" }
        defines { "NDEBUG", "RELEASE" }
        flags { "Optimize", "FloatFast", "WinMain" }

		
    configuration { "Debug" }
        targetdir ( "bin/" .. "/debug" )

    configuration { "Release" } 
        targetdir ( "bin/" .. "/release" )   

		
	project "q_ge_project_2"
		targetname "q_ge_project_2" 
		debugdir ""
		location ( location_path )
		language "C++"
		kind "WindowedApp"
		files { "src/core/**.cpp", "src/core/**.hpp", "src/core/**.h", "src/shared/**.cpp", "src/shared/**.hpp", "src/shared/**.h"}
		includedirs { "src/core", "include", "src/shared" }
		links { "gfx", "logger", "assimp", "lua51", "zlib" }
		
	
	project "gfx"
		location ( location_path )
		language "C++"
		kind "SharedLib"
		files { "src/gfx/**.hpp", "src/gfx/**.h", "src/gfx/**.cpp", "src/shared/**.cpp", "src/shared/**.hpp", "src/shared/**.h", "shaders/**.hlsl", "include/WICTextureLoader/WICTextureLoader.cpp", "include/WICTextureLoader/WICTextureLoader.h"  }
		includedirs { "src/gfx", "include", "src/shared", "$(IncludePath);$(DXSDK_DIR)Include;$(SolutionDir)include", "include/WICTextureLoader/WICTextureLoader.*"}
		defines { "GFX_DLL_EXPORT" }
		links { "d3d11", "d3dcompiler", "dxguid", "winmm", "logger", "DirectXTex" }
		
		
	project "logger"
		location ( location_path )
		language "C++"
		kind "SharedLib"
		files { "src/logger/**.hpp", "src/logger/**.h", "src/logger/**.cpp", "src/shared/logger/**.cpp", "src/shared/logger/**.hpp", "src/shared/logger/**.h" }
		includedirs { "src/logger", "include", "src/shared/logger" }
		defines { "LOGGER_DLL_EXPORT" }
        
    
