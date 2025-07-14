# CMakeLists.txt
# Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
# Licensed under the MIT licence. See LICENCE file in the project root for detailed information.

project(mmpcli)

include(FetchContent)


# Add the Windows Implementation Library (WIL) as a dependency.
if (WIN32)
    FetchContent_Declare(wil
        URL "https://github.com/microsoft/wil/archive/refs/tags/v1.0.250325.1.zip"
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    option(FAST_BUILD "" OFF)
    option(WIL_BUILD_PACKAGING "" OFF)
    option(WIL_BUILD_TESTS "" OFF)
    FetchContent_GetProperties(wil)
    if (NOT wil_POPULATED)
        FetchContent_Populate(wil)
        add_subdirectory(${wil_SOURCE_DIR} ${wil_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif ()
    mark_as_advanced(FORCE
        FETCHCONTENT_SOURCE_DIR_WIL
        FETCHCONTENT_UPDATES_DISCONNECTED_WIL
        FAST_BUILD
        WIL_BUILD_PACKAGING
        WIL_BUILD_TESTS)
endif ()


# Grab all the files the target depends on.
set(IncludeDirectory "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SourceDirectory "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE PublicHeaderFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${IncludeDirectory}" "*.h" "*.inl")
file(GLOB_RECURSE PrivateHeaderFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${SourceDirectory}" "*.h" "*.inl")
set (HeaderFiles ${PublicHeaderFiles} ${PrivateHeaderFiles})
file(GLOB_RECURSE SourceFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${SourceDirectory}" "*.cpp")
if (WIN32)
    file(GLOB_RECURSE ResourceFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" "*.rc")
else ()
    set(ResourceFiles "")
endif ()

# Group resource scripts into a new filter.
source_group("Resource Files" FILES ${ResourceFiles})


# Define the target
add_library(${PROJECT_NAME} SHARED ${HeaderFiles} ${SourceFiles} ${ResourceFiles})
target_compile_definitions(${PROJECT_NAME} PRIVATE MMPCLI_EXPORTS)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${IncludeDirectory}>
    PRIVATE
        $<BUILD_INTERFACE:${SourceDirectory}>)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE WIL)
endif ()


# Install the library
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Config.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
