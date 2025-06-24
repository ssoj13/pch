cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RULE_MESSAGES OFF)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(DEFINED VCPKG_ROOT)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
elseif(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
    message(FATAL_ERROR "VCPKG_ROOT not set")
endif()
set(VCPKG_APPLOCAL_DEPS ON)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)
if(NOT DEFINED GEN_APP)
	set(GEN_APP OFF)
endif()
project(pch)

#==========================================

include (CMakeFindDependencyMacro)
set(CUR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(PNAME ${PROJECT_NAME})
message("!!! PNAME: ${PNAME}")

#==========================================
#	Set env: 
#==========================================
# Set PNAME_UPPERCASE and PNAME_LOWERCASE variables
string(TOUPPER ${PNAME} PNAME_UPPERCASE)
string(TOLOWER ${PNAME} PNAME_LOWERCASE)

# Version variables
set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)
set(PROJECT_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

# INSTALL_LIB_DIR
set(INSTALL_LIB_DIR lib CACHE PATH "Relative installation path for libraries")
# INSTALL_BIN_DIR
set(INSTALL_BIN_DIR bin CACHE PATH "Relative installation path for executables")
# INSTALL_INCLUDE_DIR
set(INSTALL_INCLUDE_DIR include CACHE PATH "Relative installation path for header files")

# INSTALL_CMAKE_DIR
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR cmake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/${PNAME})
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Relative instalation path for CMake files")

# Convert relative path to absolute path (needed later on)
foreach(substring LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${substring}_DIR)
  if(NOT IS_ABSOLUTE ${${var}})
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
  message(STATUS "${var}: "  "${${var}}")
endforeach()

# Set up include-directories
include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")

# Library name (by default is the project name in lowercase)
# Example: libfastnoise.so
if(NOT LIBRARY_NAME)
  set(LIBRARY_NAME ${PNAME_LOWERCASE})
endif()

# Library folder name (by default is the project name in lowercase)
# Example: #include <fastnoise/fastnoise.h>
if(NOT LIBRARY_FOLDER)
  set(LIBRARY_FOLDER ${PNAME_LOWERCASE})
endif()

# The export set for all the targets
set(PROJECT_EXPORT ${PNAME}EXPORT)

# Path of the CMake files generated
set(PROJECT_CMAKE_FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})

# The RPATH to be used when installing
set(CMAKE_INSTALL_RPATH ${INSTALL_LIB_DIR})


#==========================================
# Library configuration
#==========================================
# Select library type
set(_PN ${PNAME})
option(BUILD_SHARED_LIBS "Build ${_PN} as a shared library." OFF)
if(BUILD_SHARED_LIBS)
  set(LIBRARY_TYPE SHARED)
else()
  set(LIBRARY_TYPE STATIC)
endif()

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release'.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


#==========================================
# Dependencies
#==========================================
# Threads::Threads is to make it work on Linux
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(PKGS "Threads;fmt;simplexnoise")
foreach(P ${PKGS})
    find_package(${P} REQUIRED)
    if (${P}_FOUND)
        message( "#### ${P} found!")
        message( "#### ${P}_INCLUDE_DIRS: ${${P}_INCLUDE_DIRS}")
        message( "#### ${P}_LIBRARIES: ${${P}_LIBRARIES}")
    else()
        message( "#### !!! ${P} NOT found !!!")
    endif()
endforeach()
#find_package(Boost 1.73.0 REQUIRED system)

find_package(Maya 2024 REQUIRED)
if (Maya_FOUND)
    if (NOT Maya_FIND_QUIETLY)
        message(STATUS "Maya version: ${Maya_MAJOR_VERSION}.${Maya_MINOR_VERSION}.${Maya_SUBMINOR_VERSION}")
    endif()
    if (NOT Maya_FIND_QUIETLY)
        message(STATUS "Found the following Maya libraries:")
    endif()
    foreach(COMPONENT ${Maya_FIND_COMPONENTS})
        string(TOUPPER ${COMPONENT} UPPERCOMPONENT)
        if(Maya_${UPPERCOMPONENT}_FOUND)
            if(NOT Maya_FIND_QUIETLY)
                message(STATUS "  ${COMPONENT}")
            endif()
            set(Maya_LIBRARIES ${Maya_LIBRARIES} ${Maya_${UPPERCOMPONENT}_LIBRARY})
        endif()
    endforeach()
	# set(MAYA_PLUGINS_DIR "~/maya/2023/plug-ins")
else()
    if(Maya_FIND_REQUIRED)
        message(SEND_ERROR "Unable to find the requested Maya libraries.\n${Maya_ERROR_REASON}")
    endif()
endif()
message(STATUS "Maya executable: ${MAYA_EXECUTABLE}")
message(STATUS "Maya_include_dir: ${MAYA_INCLUDE_DIR}")
message(STATUS "Maya_library_dir: ${MAYA_LIBRARY_DIR}")
message(STATUS "Maya_libs: ${MAYA_LIBRARIES}")
###################################




#==========================================
# Set SOURCES / HEADERS
#==========================================
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
# Set HEADERS variable
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

#==========================================
# Library configuration-2: after deps
#==========================================
# Target
if(GEN_APP)
	message("### GEN_APP is set, creating executable")
	add_executable(${LIBRARY_NAME} ${SOURCES} ${HEADERS})
else()
	message("### GEN_APP is not set, creating library")
	add_library(${LIBRARY_NAME} ${LIBRARY_TYPE} ${SOURCES} ${HEADERS})
endif()

target_include_directories(${LIBRARY_NAME} PRIVATE ${MAYA_INCLUDE_DIR} ${JFW_INCLUDE_DIRS})
target_link_libraries(${LIBRARY_NAME}
	INTERFACE
	Threads::Threads
	fmt::fmt-header-only
	simplexnoise
)


# Install library
install(TARGETS ${LIBRARY_NAME}
  EXPORT ${PROJECT_EXPORT}
  RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT stlib
  COMPONENT dev)

# Create 'version.h'
configure_file(version.h.in		"${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY)
set(HEADERS ${HEADERS}			"${CMAKE_CURRENT_BINARY_DIR}/version.h")
# Install headers
install(FILES ${HEADERS} DESTINATION "${INSTALL_INCLUDE_DIR}/${LIBRARY_FOLDER}" )


#==========================================
# Installation configuration:
#==========================================
# This "exports" all targets which have been put into the export set
install(EXPORT ${PROJECT_EXPORT}							DESTINATION ${INSTALL_CMAKE_DIR}		FILE ${PNAME}Targets.cmake)
# Create the <package>Config.cmake.in                       	
configure_file(${CUR_ROOT}/cmake/Config.cmake.in			"${PROJECT_CMAKE_FILES}/${PNAME}Config.cmake" @ONLY)
# Create the <package>ConfigVersion.cmake.in
configure_file(${CUR_ROOT}/cmake/ConfigVersion.cmake.in		"${PROJECT_CMAKE_FILES}/${PNAME}ConfigVersion.cmake" @ONLY)
# Install <package>Config.cmake and <package>ConfigVersion.cmake files
install(FILES		"${PROJECT_CMAKE_FILES}/${PNAME}Config.cmake"		"${PROJECT_CMAKE_FILES}/${PNAME}ConfigVersion.cmake"		DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
# Uninstall targets
configure_file("${CUR_ROOT}/cmake/Uninstall.cmake.in"		"${PROJECT_CMAKE_FILES}/Uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(${LIBRARY_NAME}_uninstall 	COMMAND ${CMAKE_COMMAND} -P ${PROJECT_CMAKE_FILES}/Uninstall.cmake)
#########################

