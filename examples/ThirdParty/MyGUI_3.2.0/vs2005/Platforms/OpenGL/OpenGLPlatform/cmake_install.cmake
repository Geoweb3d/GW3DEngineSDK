# Install script for directory: C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/Platforms/OpenGL/OpenGLPlatform

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/vs2005/sdk")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MYGUI" TYPE FILE FILES
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/Platforms/OpenGL/OpenGLPlatform/include/MyGUI_OpenGLDataManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/Platforms/OpenGL/OpenGLPlatform/include/MyGUI_OpenGLDiagnostic.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/Platforms/OpenGL/OpenGLPlatform/include/MyGUI_OpenGLPlatform.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/Platforms/OpenGL/OpenGLPlatform/include/MyGUI_OpenGLRenderManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/Platforms/OpenGL/OpenGLPlatform/include/MyGUI_OpenGLTexture.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/Platforms/OpenGL/OpenGLPlatform/include/MyGUI_OpenGLRTTexture.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/Platforms/OpenGL/OpenGLPlatform/include/MyGUI_OpenGLVertexBuffer.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release" TYPE STATIC_LIBRARY FILES "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/vs2005/lib/Release/MyGUI.OpenGLPlatform.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo" TYPE STATIC_LIBRARY FILES "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/vs2005/lib/RelWithDebInfo/MyGUI.OpenGLPlatform.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel" TYPE STATIC_LIBRARY FILES "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/vs2005/lib/MinSizeRel/MyGUI.OpenGLPlatform.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug" TYPE STATIC_LIBRARY FILES "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/vs2005/lib/Debug/MyGUI.OpenGLPlatform_d.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

