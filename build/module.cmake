message (STATUS "Configuring ${MODULE}")

if (NOT PROJECT_ROOT_DIR)
    set (PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR})
endif ()

add_library (${MODULE})

if (MODULE_SET_LANGUAGE)
    set_target_properties (${MODULE} PROPERTIES LINKER_LANGUAGE ${MODULE_LANGUAGE})
endif (MODULE_SET_LANGUAGE)

if (BUILD_SHARED_LIBS)
    install (TARGETS ${MODULE} DESTINATION ${SHARED_LIBS_INSTALL_DESTINATION})

    if (NOT MSVC)
        set_target_properties (${MODULE} PROPERTIES COMPILE_FLAGS "-fPIC")
    endif (NOT MSVC)
endif ()

target_sources (${MODULE} PRIVATE ${MODULE_SRC})

target_include_directories (${MODULE} PUBLIC
    ${PROJECT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_ROOT_DIR}
    ${PROJECT_ROOT_DIR}/src
    ${PROJECT_ROOT_DIR}/thirdparty/googletest/googletest/include
    ${MODULE_INCLUDE}
)

target_link_libraries (${MODULE} PRIVATE ${MODULE_LINK})