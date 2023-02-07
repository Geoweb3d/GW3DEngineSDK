# Install script for directory: C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine

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
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release" TYPE STATIC_LIBRARY FILES "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/vs2005/lib/Release/MyGUIEngineStatic.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo" TYPE STATIC_LIBRARY FILES "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/vs2005/lib/RelWithDebInfo/MyGUIEngineStatic.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel" TYPE STATIC_LIBRARY FILES "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/vs2005/lib/MinSizeRel/MyGUIEngineStatic.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug" TYPE STATIC_LIBRARY FILES "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/vs2005/lib/Debug/MyGUIEngineStatic_d.lib")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/MYGUI" TYPE FILE FILES
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Allocator.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_CustomAllocator.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DDItemInfo.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IBItemInfo.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_MenuItemType.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResizingPolicy.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_WidgetToolTip.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Button.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Canvas.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ComboBox.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DDContainer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_EditBox.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ImageBox.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ItemBox.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ListBox.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_MenuBar.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_MenuControl.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_MenuItem.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_MultiListBox.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_PopupMenu.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ProgressBar.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ScrollBar.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ScrollView.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TextBox.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TabControl.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TabItem.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_MultiListItem.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Widget.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Window.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Bitwise.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Constants.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_CoordConverter.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DataFileStream.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DataStream.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Delegate.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DelegateImplement.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DynLib.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Enumerator.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_EventPair.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_GeometryUtility.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_StringUtility.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TextChangeHistory.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TextIterator.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TextureUtility.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TextView.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TextViewData.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Timer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_UString.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_WidgetTranslate.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_XmlDocument.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ConsoleLogListener.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_FileLogListener.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ILogFilter.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ILogListener.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LevelLogFilter.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LogLevel.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LogManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LogSource.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LogStream.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_BackwardCompatibility.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Common.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DeprecatedTypes.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DeprecatedWidgets.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Diagnostic.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Exception.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_GenericFactory.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Macros.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Platform.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Precompiled.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Prerequest.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_RenderFormat.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_WidgetDefines.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ICroppedRectangle.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IDataStream.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IItem.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IItemContainer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IObject.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ISerializable.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ITexture.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IUnlinkWidget.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IVertexBuffer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Plugin.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_RTTI.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Singleton.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_BiIndexBase.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ScrollViewBase.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_WidgetInput.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_WidgetUserData.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_EditText.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_MainSkin.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_PolygonalSkin.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_RotatingSkin.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_SimpleText.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_SubSkin.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TileRect.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IStateInfo.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ISubWidget.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ISubWidgetRect.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ISubWidgetText.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LayerItem.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LayerNode.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_OverlappedLayer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_RenderItem.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_SharedLayer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_SharedLayerNode.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_SkinItem.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ILayer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ILayerItem.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ILayerNode.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IRenderTarget.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ActionController.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ControllerEdgeHide.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ControllerFadeAlpha.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ControllerItem.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ControllerPosition.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResourceImageSet.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResourceImageSetPointer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResourceLayout.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResourceManualFont.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResourceManualPointer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResourceSkin.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResourceTrueTypeFont.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IFont.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IPointer.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_IResource.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ChildSkinInfo.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_FontData.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LayoutData.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_MaskPickInfo.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResourceImageSetData.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_SubWidgetBinding.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_SubWidgetInfo.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Align.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Any.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Colour.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_CommonStateInfo.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_FlowDirection.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ImageInfo.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_KeyCode.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_MouseButton.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_RenderTargetInfo.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TCoord.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TPoint.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TRect.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_TSize.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Types.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Version.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_VertexData.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_WidgetStyle.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ClipboardManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ControllerManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DataManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_DynLibManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_FactoryManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_FontManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_Gui.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_InputManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LanguageManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LayerManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_LayoutManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_PluginManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_PointerManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_RenderManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ResourceManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_SkinManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_SubWidgetManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_ToolTipManager.h"
    "C:/dev/sourcecode/trunk/geoweb3d/distribution/examples/ThirdParty/MyGUI_3.2.0/MyGUIEngine/include/MyGUI_WidgetManager.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

