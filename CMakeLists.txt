CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
PROJECT (RemoteDisplayView)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
ADD_COMPILE_OPTIONS (-g -O3 -Wall -Wextra -fPIC)

#-------------------------------------------------------------------------------
# Set Third party Library
#-------------------------------------------------------------------------------
find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
SET(TARGET_NAME "remote-display-view-test")

#-------------------------------------------------------------------------------
# Set Build Variable
#-------------------------------------------------------------------------------
# SET(SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src)
SET(SCREEN_CAPTURE_DIR ${PROJECT_SOURCE_DIR}/screen_capture)
SET(TEST_BIN_FILE_DIR ${PROJECT_SOURCE_DIR}/cmd/remote_display_server)

SET(SCREEN_CAPTURE_INCLUDE_FILES
  ${SCREEN_CAPTURE_DIR}/geometry.h
  ${SCREEN_CAPTURE_DIR}/screen_capture_bridge.h
  ${SCREEN_CAPTURE_DIR}/screen_capture.h
)

SET(SCREEN_CAPTURE_SOURCE_FILES
  ${SCREEN_CAPTURE_DIR}/screen_capture_bridge.cc
  ${SCREEN_CAPTURE_DIR}/screen_capture.cc
)

SET(RDV_SCREEN_CAPTURE
  ${SCREEN_CAPTURE_INCLUDE_FILES}
  ${SCREEN_CAPTURE_SOURCE_FILES}
)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${GTK3_INCLUDE_DIRS}
)

link_directories (
  ${GTK3_LIBRARY_DIRS}
)

#-------------------------------------------------------------------------------
# Build
#-------------------------------------------------------------------------------
add_library(rdv_screencapture SHARED ${RDV_SCREEN_CAPTURE})
target_link_libraries(rdv_screencapture
 PRIVATE
  ${GTK3_LIBRARIES}
)

add_executable(${TARGET_NAME} ${TEST_BIN_FILE_DIR}/remote_display_view_test.cc)
target_link_libraries(${TARGET_NAME}
 PRIVATE
  rdv_screencapture
  ${GTK3_LIBRARIES}
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