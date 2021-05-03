CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
PROJECT (RemoteDisplayView)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
ADD_COMPILE_OPTIONS (-g -Wall)

#-------------------------------------------------------------------------------
# Set Third party Library
#-------------------------------------------------------------------------------
find_package(X11 REQUIRED)
find_package(xinerama REQUIRED)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0)

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
SET(TARGET_NAME "remote-display-view")

#-------------------------------------------------------------------------------
# Set Build Variable
#-------------------------------------------------------------------------------
SET(SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${X11_INCLUDE_DIR}
  ${GTK2_INCLUDE_DIRS}
)

link_directories (
  ${GTK2_LIBRARY_DIRS}
)

#-------------------------------------------------------------------------------
# Set SubDirectory
#-------------------------------------------------------------------------------
add_executable(${TARGET_NAME} ${SOURCE_DIRECTORY}/remote_display_view.cc)

target_link_libraries(${TARGET_NAME}
PRIVATE
  ${X11_LIBRARIES}
  ${GTK2_LIBRARIES}
)

# # -------------------------------------------------------------------------------
# # Set gTest
# # -------------------------------------------------------------------------------
# include(cmake/googletest.cmake)

# fetch_googletest(
#     ${PROJECT_SOURCE_DIR}/cmake
#     ${PROJECT_BINARY_DIR}/googletest
# )

# enable_testing()

# add_subdirectory(test)