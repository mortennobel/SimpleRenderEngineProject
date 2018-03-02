SET(scr_files spinning-cube spinning-primitives-tex spinning-primitives-openvr hello-engine gui spheres benchmark64k picking render-to-texture obj-viewer hello-engine-raw sprite-example custom-mesh-layout)

# Create custom build targets
FOREACH(scr_file ${scr_files})
    add_executable(SRE-Example-${scr_file} ${scr_file}.cpp)
    target_link_libraries(SRE-Example-${scr_file} ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${OPENVR_LIB} SRE)
ENDFOREACH()

file(COPY ../examples_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ../examples_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

IF (WIN32)
    IF (USE_OPENVR)	
		SET(DLLFileList "${DLLFileList}" "${OPENVR_ROOT_DIR}/bin/${OPENVR_PLATFORM}/openvr_api.dll")
	ENDIF (USE_OPENVR)
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
ENDIF(WIN32)
