--[[
	Premake script to create the makefiles for Erolf
--]]

dofile "premake4_arm.lua"


-- homa solution config
solution "homa"
	configurations { "Debug", "Release", "Processor1", "Processor2" }
	platforms { "native", "arm" }

	project "homa"
		kind "ConsoleApp"
		language "c"

		includedirs {"src/", "src/framework/include/", "src/target/include"}

		-- global compiler options
		flags {
			
		}
		
		-- extra options
		newoption {
		   trigger     = "component",
		   value       = "nr",
		   description = "Choose target component",
		   allowed = {
		      { "application",  "application" },
		      { "vector",  "VectorTableLib" }
		   }
		}

		buildoptions { "-Wall", "-Wextra", "-Werror", "-std=c99", "-O2"}
		linkoptions {
			"-L."
		}

		local targetDir        = "build/".._OPTIONS.platform.."/"
		local targetDirRelease = targetDir.."release/".._OPTIONS["component"].."/"
		local targetDirDebug   = targetDir.."debug/".._OPTIONS["component"].."/"
		local targetName       = "homa"
		local targetSuffix     = ".elf"

	
		targetname(targetName..targetSuffix)
		
			
		--[[
			global config
		--]]

		prebuildcommands { '@echo "\\n\\n--- Starting to build: `date` ---\\n\\n"' }
		postbuildcommands { '@echo "\\n\\n--- Finished build ---\\n\\n"' }

		configuration "DEBUG"
			--[[
			--  valid DEBUG SYMBOLS:
			--    DEBUG
			--]]
			defines { "DEBUG", "STM32F4" }
			flags { "Symbols"}
			objdir(targetDirDebug.."/obj/")
			buildoptions {"-g3", "-O0"}
			
			
		configuration "RELEASE"
			flags { "Symbols" }
			defines { "RELEASE", "STM32F0" }
			objdir(targetDirRelease.."/obj/")
			buildoptions {"-g3", "-O2"}
			
		--[[
			processor location 
		--]]
		
		configuration "vector"
			kind "StaticLib"
			files {
				"src/target/stm32f0/vectorISR.c"
			}
			linkoptions {
			}
			targetname("vector")
			
		configuration "application"
			files {
				"src/**.c"
			}
			excludes {
				"src/target/stm32f0/vectorISR.c"
			}
			linkoptions {
				"-Tsrc/target/stm32f0/stm32.ld",
				"-lvector",
				"-lm",
			--	"-lc",
				"-Wl,-Map=homa.map,--cref"
			}
			targetname("homa"..targetSuffix)
			

		--[[
			ARM specific config	
		--]]
		configuration "arm"
			defines { "STM32F0"
				--   ,"NO_LOG_OUTPUT" 
					}
			
			buildoptions { "-mthumb", "-mcpu=cortex-m0", "-msoft-float" }
			linkoptions { "-mthumb -mcpu=cortex-m0 -msoft-float" }
			linkoptions {
				"-mfix-cortex-m3-ldrd",
				"-nostartfiles",
			}

