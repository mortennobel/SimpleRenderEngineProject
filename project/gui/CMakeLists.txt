file(GLOB GuiSrc
        "*.hpp"
        "*.h"
        "*.cpp"
        )

# Compile cpp files
add_executable(SRE-Gui ${GuiSrc})
target_link_libraries(SRE-Gui ${all_libs})

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
    file(COPY ${DLLFileList} DESTINATION .)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(SRE-Gui PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)
