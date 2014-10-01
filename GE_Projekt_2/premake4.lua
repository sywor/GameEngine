

solution "q_ge_project_2"
    configurations { "Debug", "Release" }
        flags{ "Unicode", "NoPCH" } 
        libdirs { "lib" }
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
        flags { "Symbols" }
        
    configuration { "Release" }
        defines { "NDEBUG", "RELEASE" }
        flags { "Optimize", "FloatFast" }

		
    configuration { "Debug" }
        targetdir ( "bin/" .. "/debug" )

    configuration { "Release" } 
        targetdir ( "bin/" .. "/release" )   

		
	project "q_ge_project_2"
		targetname "q_ge_project_2" 
		debugdir ""
		location ( location_path )
		language "C++"
		kind "ConsoleApp"
		files { "src/core/**.cpp", "src/core/**.hpp", "src/core/**.h", "src/shared/**.cpp", "src/shared/**.hpp", "src/shared/**.h"}
		includedirs { "src/core", "include", "src/shared" }
		links { "SDL2", "SDL2main", "SDL2test", "glew32", "opengl32", "gfx", "logger", "assimp", "lua51", "zlib1" }
		
	
	project "gfx"
		location ( location_path )
		language "C++"
		kind "SharedLib"
		files { "src/gfx/**.hpp", "src/gfx/**.h", "src/gfx/**.cpp", "src/shared/**.cpp", "src/shared/**.hpp", "src/shared/**.h", "shaders/**.glsl" }
		includedirs { "src/gfx", "include", "src/shared" }
		defines { "GFX_DLL_EXPORT" }
		links { "SDL2", "SDL2main", "SDL2test", "glew32", "opengl32", "logger" }
		
	project "logger"
		location ( location_path )
		language "C++"
		kind "SharedLib"
		files { "src/logger/**.hpp", "src/logger/**.h", "src/logger/**.cpp", "src/shared/logger/**.cpp", "src/shared/logger/**.hpp", "src/shared/logger/**.h" }
		includedirs { "src/logger", "include", "src/shared/logger" }
		defines { "LOGGER_DLL_EXPORT" }
        
    
