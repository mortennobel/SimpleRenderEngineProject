
add_executable(SRE-Project project.cpp)
add_executable(SRE-Box2d box2d-example.cpp)
add_executable(SRE-SDL2Mixer sdl2_mixer-example.cpp)

set(all_libs ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${Box2D_LIBRARY} ${SDL2MIXER_LIBRARY} SRE)


target_link_libraries(SRE-Project ${all_libs})
target_link_libraries(SRE-Box2d ${all_libs})
target_link_libraries(SRE-SDL2Mixer ${all_libs})

message("Copy files from ${CMAKE_SOURCE_DIR}/project/data to ${Debug}")
file(COPY ${CMAKE_SOURCE_DIR}/project/data DESTINATION .)
file(COPY ${CMAKE_SOURCE_DIR}/project/data DESTINATION .)

IF (WIN32)
    SET(SDL2_dll "" CACHE STRING "Where the SDL2.dll can be found")
    SET(SDL2_image_lib_path "" CACHE PATH "Where the SDL2_image.dll can be found")
    SET(DLLFileList "${SDL2_image_lib_path}/SDL2_image.dll" "${SDL2_image_lib_path}/libjpeg-9.dll" "${SDL2_image_lib_path}/libpng16-16.dll" "${SDL2_image_lib_path}/libtiff-5.dll" "${SDL2_image_lib_path}/libwebp-4.dll" "${SDL2_image_lib_path}/zlib1.dll"  ${SDL2_dll})
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
ENDIF(WIN32)
