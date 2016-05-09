# get all the *.cxx filenames, to compile them into a lib
#file(GLOB SOME_PROJ_LIB_SRCS "${PROJECT_SOURCE_DIR}/src/*.cc")
## include ROOT library and include files
#include_directories(${PROJECT_SOURCE_DIR}/include)
#include_directories(${ROOTSYS}/include)
#link_directories(${ROOTSYS}/lib)
## and compile src into a library
#add_library(Proj_lib_name ${SOME_PROJ_LIB_SRCS})
## here, list the ROOT libraries you require
#target_link_libraries(Proj_lib_name dl Core Cint RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread Gui pthread m)


# find ROOT
FIND_PACKAGE(ROOT REQUIRED)
INCLUDE_DIRECTORIES(SYSTEM ${ROOT_INCLUDE_DIR})

SET(LIB_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/EventAlignment.cc)
SET(LIB_HEADER_FILES ${PROJECT_SOURCE_DIR}/include/EventAlignment.hh)
set(EVAL_DICT ${CMAKE_CURRENT_BINARY_DIR}/evalDict.cxx)
ROOT_GENERATE_DICTIONARY_OLD("${LIB_HEADER_FILES}" "${ROOT_INCLUDE_DIR}" "${EVAL_DICT}")
ADD_LIBRARY(${PROJECT_NAME} SHARED ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})
# Link necessary libraries:
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${ROOT_LIBRARIES})

#INSTALL(TARGETS EvAl
#        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin
#        LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
#        ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib)