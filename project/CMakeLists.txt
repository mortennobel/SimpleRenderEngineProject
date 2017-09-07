# Add executables (to different projects)
add_executable("SRE-box2d-example" "box2d-example.cpp")
add_executable("SRE-project" "project.cpp")
add_executable("SRE-sdl2_mixer-example" "sdl2_mixer-example.cpp")
add_executable("SRE-lua-example" "lua-example.cpp")

# Link projects with project libraries
target_link_libraries("SRE-box2d-example" ${all_libs})
target_link_libraries("SRE-project" ${all_libs})
target_link_libraries("SRE-sdl2_mixer-example" ${all_libs})
if (UNIX)
target_link_libraries("SRE-lua-example" ${all_libs}  -ldl)
else (UNIX)
target_link_libraries("SRE-lua-example" ${all_libs})
endif(UNIX)

# Copy project data to output path
file(COPY ../project_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ../project_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

IF (WIN32)
	## Copy dll's to output path
	file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
	file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
	# Set working folder to project folder (to find project data runtime)
	set_target_properties("SRE-box2d-example" PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
	set_target_properties("SRE-project" PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
	set_target_properties("SRE-sdl2_mixer-example" PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
	set_target_properties("SRE-lua-example" PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
ENDIF(WIN32)

