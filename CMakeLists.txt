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
# find_package(xinerama REQUIRED)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0)

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
SET(TARGET_NAME "remote-display-view")

#-------------------------------------------------------------------------------
# Set Define
#-------------------------------------------------------------------------------
add_definitions(-DDEBUG_X11_WINDOW)

#-------------------------------------------------------------------------------
# Set Build Variable
#-------------------------------------------------------------------------------
SET(SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src)

SET(BASE_CAPTURE_DIR ${PROJECT_SOURCE_DIR}/src/screen_capture)
SET(NET_CAPTURE_DIR ${PROJECT_SOURCE_DIR}/src/screen_capture)
SET(SCREEN_CAPTURE_DIR ${PROJECT_SOURCE_DIR}/src/screen_capture)
SET(THIRD_PARTY_DIR ${PROJECT_SOURCE_DIR}/src/third_party)

SET(BASE_INCLUDE_FILES
  ${BASE_CAPTURE_DIR}/geometry.h
  ${BASE_CAPTURE_DIR}/time.h
)

SET(BASE_SOURCE_FILES

)

SET(NET_INCLUDE_FILES

)

SET(NET_SOURCE_FILES

)

SET(THIRD_PARTY_INCLUDE_FILES
)

SET(THIRD_PARTY_SOURCE_FILES
)

SET(SCREEN_CAPTURE_INCLUDE_FILES
  ${SCREEN_CAPTURE_DIR}/display_X11.h
  ${SCREEN_CAPTURE_DIR}/debug_window_X11.h
  ${SCREEN_CAPTURE_DIR}/screen_capture_X11.h
)

SET(SCREEN_CAPTURE_SOURCE_FILES
  ${SCREEN_CAPTURE_DIR}/display_X11.cc
  ${SCREEN_CAPTURE_DIR}/debug_window_X11.cc
  ${SCREEN_CAPTURE_DIR}/screen_capture_X11.cc
)

SET(RDV_BASE ${BASE_INCLUDE_FILES} ${BASE_SOURCE_FILES})
SET(RDV_NET ${NET_INCLUDE_FILES} ${NET_SOURCE_FILES})
SET(RDV_SCREEN_CAPTURE ${SCREEN_CAPTURE_INCLUDE_FILES} ${SCREEN_CAPTURE_SOURCE_FILES})
SET(RDV_THIRD_PARTY ${THIRD_PARTY_INCLUDE_FILES} ${THIRD_PARTY_SOURCE_FILES})

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${X11_INCLUDE_DIR}
  ${X11_Xinerama_INCLUDE_DIR}
  ${GTK2_INCLUDE_DIRS}
)

link_directories (
  ${GTK2_LIBRARY_DIRS}
)

#-------------------------------------------------------------------------------
# Build
#-------------------------------------------------------------------------------
# add_library(rdv_base STATIC ${RDV_BASE})
# add_library(rdv_net STATIC ${NET_SOURCE_FILES})

add_library(rdv_screencapture STATIC ${RDV_SCREEN_CAPTURE})
target_link_libraries(rdv_screencapture
 PRIVATE
  ${X11_LIBRARIES}
  ${X11_Xinerama_LIB}
  ${GTK2_LIBRARIES}
)

# add_library(rdv_thirdparty STATIC ${THIRD_PARTY_SOURCE_FILES})

add_executable(${TARGET_NAME} ${SOURCE_DIRECTORY}/remote_display_view.cc)
target_link_libraries(${TARGET_NAME}
 PRIVATE
  # rdv_base
  # rdv_net
  rdv_screencapture
  rdv_thirdparty
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