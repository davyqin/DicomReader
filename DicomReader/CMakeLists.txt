cmake_minimum_required(VERSION 3.4.0)

project(DicomReader)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#set(CMAKE_CXX_FLAGS "-std=c++11 -o3 -Wall -lGL -lGLU -lglut -fpermissive")
#set(CMAKE_CXX_FLAGS "-std=c++11 -o3 -Wall")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND DCMTK
#########################################################
find_package(DCMTK REQUIRED)
include_directories(${DCMTK_INCLUDE_DIRS})
link_directories(${DCMTK_LIBRARY_DIRS})
add_definitions(${DCMTK_DEFINITIONS})
if(NOT DCMTK_FOUND)
    message(ERROR " DCMTK not found!")
endif(NOT DCMTK_FOUND)

#########################################################
# FIND Boost
#########################################################
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})
if(NOT Boost_FOUND)
    message(ERROR " Boost not found!")
endif(NOT Boost_FOUND)

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5OpenGL)


FILE(GLOB SRC_FILES "./*.cpp")
FILE(GLOB HEAD_FILES "./*.h")
FILE(GLOB UI_FILES "./*.ui")

# qt5_wrap_ui(WRAP_FILES ${UI_FILES})

# set(RCC_FILES rcc.qrc)

source_group("UI" FILES ${UI_FILES} ${WRAP_FILES})

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${DCMTK_LIBRARIES} ${Boost_LIBRARIES})

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
