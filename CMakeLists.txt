cmake_minimum_required(VERSION 2.6)
PROJECT(3PCHM)

FIND_PACKAGE(VXL)
IF(VXL_FOUND)
  INCLUDE(${VXL_CMAKE_DIR}/UseVXL.cmake)
ENDIF(VXL_FOUND)
#IF( VXL_VNL_FOUND )
INCLUDE_DIRECTORIES( ${VXL_VNL_INCLUDE_DIR} )
#IF( VXL_MUL_FOUND )
INCLUDE_DIRECTORIES( ${VXL_MUL_INCLUDE_DIR} )

IF(NOT VTK_BINARY_DIR)
FIND_PACKAGE(VTK)
IF(NOT VTK_DIR)
  MESSAGE(FATAL_ERROR "Please set VTK_DIR.")
ENDIF(NOT VTK_DIR)
INCLUDE(${VTK_USE_FILE})
ENDIF(NOT VTK_BINARY_DIR)

#find_package(ITK REQUIRED)
#include(${ITK_USE_FILE})

FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES( ${OPENCV_INCLUDE_DIR})

# use what QVTK built with
SET(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
SET(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})



# Use the include path and library for Qt that is used by VTK.
INCLUDE_DIRECTORIES(
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)


# Set your files and resources here  
SET(3PCHMSrcs main.cxx SimpleView.cxx CHM.cxx ProjMatch.cxx MeshGMM.cxx SamplingTools.cxx gmmreg_utils.cpp gmmreg_base.cpp gmmreg_ctrl_pts.cpp gmmreg_cpd.cpp gmmreg_tps.cpp gmmreg_tps_func.cpp gmmreg_rigid.cpp gmmreg_rigid_func.cpp gmmreg_grbf.cpp gmmreg_grbf_func.cpp)
SET(3PCHMUI SimpleView.ui)
SET(3PCHMHeaders SimpleView.h CHM.h ProjMatch.h MeshGMM.h SamplingTools.h gmmreg_utils.h gmmreg_base.h gmmreg_ctrl_pts.h gmmreg_cpd.h gmmreg_tps.h gmmreg_tps_func.h gmmreg_rigid.h gmmreg_rigid_func.h gmmreg_grbf.h gmmreg_grbf_func.h)
SET(3PCHMResources Icons/icons.qrc)

# The rest should just work (sure...)
QT4_WRAP_UI(UISrcs ${3PCHMUI})
QT4_WRAP_CPP(MOCSrcs ${3PCHMHeaders} )
QT4_ADD_RESOURCES(ResourceSrcs ${3PCHMResources})

SOURCE_GROUP("Resources" FILES
  ${3PCHMUI}
  ${3PCHMResources}
  ${EXE_ICON}
)

SOURCE_GROUP("Generated" FILES
  ${UISrcs}
  ${MOCSrcs}
  ${ResourceSrcs}
  ${RCS_SOURCES}
)

ADD_DEFINITIONS(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)
SET_SOURCE_FILES_PROPERTIES(${3PCHMSrcs} PROPERTIES OBJECT_DEPENDS "${UISrcs}")

# It's nice to have the ui in the windows project file...just double click on it
# and designer comes up on that ui file :)
IF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
  SET (3PCHMSrcs ${3PCHMSrcs} ${3PCHMUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "msdev")
IF (${CMAKE_BUILD_TOOL} MATCHES "devenv")
  SET (3PCHMSrcs ${3PCHMSrcs} ${3PCHMUI})
ENDIF (${CMAKE_BUILD_TOOL} MATCHES "devenv")


ADD_EXECUTABLE(3PCHM MACOSX_BUNDLE ${3PCHMSrcs} ${UISrcs} ${MOCSrcs} ${ResourceSrcs})

TARGET_LINK_LIBRARIES(3PCHM
  QVTK
  QVTKWidgetPlugin
  ${QT_LIBRARIES}
  ${VTK_LIBRARIES}
  #${ITK_LIBRARIES}
  ${OpenCV_LIBS}
  vnl_algo
  vnl
  vcl
  mbl
  vgl
  vnl_io
  vbl
  vsl
  vul
  vpl
  testlib
)



