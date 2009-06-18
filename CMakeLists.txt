project(akunabmol)

# Search KDE packages
find_package(KDE4 REQUIRED)
find_package(Akonadi REQUIRED)
find_package(KdepimLibs REQUIRED)

# Search for Funambol
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(Funambol REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)
include(CheckIncludeFiles)

include_directories(${QT_INCLUDES} ${Funambol_INCLUDE_DIR})

set(akunambol_SRCS
    qtgui/config.cpp
    qtgui/main.cpp
    qtgui/mainwindow.cpp
    syncsource/akonadisource.cpp
    syncsource/sourcemanager.cpp
    )

set(akunambol_RCCS qtgui/resource.qrc)

kde4_add_ui_files(akunambol_SRCS qtgui/config.ui qtgui/mainwindow.ui)

Qt4_add_resources(akunambol_SRCS ${akunambol_RCCS})

kde4_add_executable(akunambol ${akunambol_SRCS})

target_link_libraries(akunambol ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY})

install(TARGETS akunambol DESTINATION ${BIN_INSTALL_DIR} )

