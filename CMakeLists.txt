# Author: Lucas Vilas-Boas
# Year: 2024
# Repo: https://github.com/lucoiso/luGUI

# ----------- Global Definitions -----------
SET(LIBRARY_NAME luGUI)

CMAKE_MINIMUM_REQUIRED(VERSION 3.28)
PROJECT(${LIBRARY_NAME} VERSION 0.0.1 LANGUAGES CXX)

IF (POLICY CMP0048)
    CMAKE_POLICY(SET CMP0048 NEW)
ENDIF ()

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_SCAN_FOR_MODULES ON)

IF (UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts")
ENDIF (UNIX)

SET(Boost_NO_WARN_NEW_VERSIONS ON)
IF (WIN32)
    ADD_DEFINITIONS(-DBOOST_USE_WINAPI_VERSION=0x0A00)
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x0A00)
ENDIF (WIN32)

# ------------- Library Setup --------------
SET(PRIVATE_MODULES_BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Source/Private)
SET(PUBLIC_MODULES_BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Source/Public)

SET(PRIVATE_MODULES
        "${PRIVATE_MODULES_BASE_DIRECTORY}/Integrations/GLFWCallbacks.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/Integrations/GLFWHandler.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/Integrations/ImGuiGLFWBackend.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/Integrations/ImGuiOverlay.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/Integrations/ImGuiVulkanBackend.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Controls/Control.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Controls/Viewport.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Controls/Window.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Items/Item.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Items/Text.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Items/Image.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Items/Button.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Items/Input.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Items/ComboBox.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Panels/Panel.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Panels/Stack.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/UserInterface/Panels/Grid.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/Singletons/ImageManager.cxx"
        "${PRIVATE_MODULES_BASE_DIRECTORY}/Singletons/FontManager.cxx"
)

SET(PUBLIC_MODULES
        "${PUBLIC_MODULES_BASE_DIRECTORY}/Integrations/GLFWCallbacks.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/Integrations/GLFWHandler.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/Integrations/ImGuiGLFWBackend.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/Integrations/ImGuiOverlay.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/Integrations/ImGuiVulkanBackend.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Controls/Control.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Controls/Viewport.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Controls/Window.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Controls/WindowFlags.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Items/Item.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Items/Text.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Items/Image.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Items/Button.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Items/Input.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Items/ComboBox.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Panels/Panel.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Panels/Stack.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/UserInterface/Panels/Grid.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/Singletons/ImageManager.ixx"
        "${PUBLIC_MODULES_BASE_DIRECTORY}/Singletons/FontManager.ixx"
)

SET(PUBLIC_HEADERS
        "${PUBLIC_MODULES_BASE_DIRECTORY}/luGUIModule.hpp"
)

ADD_LIBRARY(${LIBRARY_NAME} SHARED ${PRIVATE_MODULES})
SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

TARGET_SOURCES(${LIBRARY_NAME}
        PUBLIC
        FILE_SET cxx_public_modules
        TYPE CXX_MODULES
        BASE_DIRS ${PUBLIC_MODULES_BASE_DIRECTORY}
        FILES ${PUBLIC_MODULES}
)

TARGET_SOURCES(${LIBRARY_NAME}
        PUBLIC
        FILE_SET cxx_public_headers
        TYPE HEADERS
        BASE_DIRS ${PUBLIC_MODULES_BASE_DIRECTORY}
        FILES ${PUBLIC_HEADERS}
)

TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE ${PRIVATE_MODULES_BASE_DIRECTORY})
TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PUBLIC ${PUBLIC_MODULES_BASE_DIRECTORY})
TARGET_COMPILE_DEFINITIONS(${LIBRARY_NAME} PRIVATE BUILD_DLL)

ADD_SUBDIRECTORY(Renderer)
FIND_PACKAGE(imgui CONFIG REQUIRED)

SET(BUILD_SHARED_LIBS   ON)
SET(GLFW_BUILD_EXAMPLES OFF)
SET(GLFW_BUILD_TESTS    OFF)
SET(GLFW_BUILD_DOCS     OFF)
SET(GLFW_INSTALL        OFF)

INCLUDE(FetchContent)

FETCHCONTENT_DECLARE(
        glfw
        GIT_REPOSITORY https://github.com/lucoiso/glfw
        GIT_TAG attach-win32-window
)
FETCHCONTENT_MAKEAVAILABLE(glfw)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} PUBLIC
        RenderCore
        imgui::imgui
        glfw
)
TARGET_INCLUDE_DIRECTORIES(${LIBRARY_NAME} PRIVATE ${glfw_SOURCE_DIR}/include)

INCLUDE(GNUInstallDirs)
INSTALL(TARGETS ${LIBRARY_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)