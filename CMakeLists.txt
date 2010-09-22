project(akunabmol)

add_subdirectory(images/icons)

set(VERSION "0.2.1")
configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake  ${CMAKE_CURRENT_BINARY_DIR}/aku-auto-config.h)

set( CMAKE_CXX_FLAGS "-O0 -g")

#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_FUNAMBOL_NAMESPACE" )

# Search KDE packages
find_package(KDE4 REQUIRED)
find_package(Akonadi REQUIRED)
find_package(KdepimLibs REQUIRED)
find_package(LikeBack REQUIRED)

# Search for Funambol
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(Funambol REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)
include(CheckIncludeFiles)

include_directories(${QT_INCLUDES} ${KDE4_INCLUDES} ${Akonadi_INCLUDES} ${Funambol_INCLUDE_DIR} ${LIKEBACK_INCLUDE_DIR})

set(syncsource_SRCS
    syncsource/akonadisource.cpp
    syncsource/KFunSyncConfig.cpp
    syncsource/contactssource.cpp
    syncsource/eventssource.cpp
    syncsource/taskssource.cpp
    syncsource/sourcemanager.cpp
    )

set(client_SRCS
    client/appsyncsource.cpp
    client/appsyncsourceconfig.cpp
    client/appsyncsourcemanager.cpp
    client/collectionsfetcher.cpp
    )

set(akunambol_LINK_LIBS
    ${KDE4_KDEUI_LIBS}
    ${Kdepimlibs_LIBS}
    ${KDE4_KMIME_LIBS}
    ${KDE4_AKONADI_KMIME_LIBS}
    ${KDE4_KABC_LIBS}
    ${KDE4_KPIMUTILS_LIBS}
    ${KDE4_AKONADI_LIBS}
    ${LIKEBACK_LIBRARIES}
    microblog
)

# target_link_libraries(akonadi-test ${akunambol_LINK_LIBS})

set(akunambol_SRCS
    qtgui/settings.cpp
    qtgui/standardsourcesettings.cpp
    qtgui/main.cpp
    qtgui/mainwindow.cpp
    qtgui/KDELog.cpp
    qtgui/sourcepushbutton.cpp
    ${syncsource_SRCS}
    ${client_SRCS}
    )


set_source_files_properties(${akunambol_SRCS} PROPRIETIES COMPILE_FLAGS -fexceptions)

set(akunambol_RCCS qtgui/resource.qrc)

kde4_add_ui_files(akunambol_SRCS qtgui/mainwindow.ui qtgui/standardsourcesettings.ui
                  qtgui/settings.ui)

Qt4_add_resources(akunambol_SRCS ${akunambol_RCCS})

kde4_add_executable(akunambol ${akunambol_SRCS})

target_link_libraries(akunambol ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambol_LINK_LIBS} -lkcal)

install(FILES akunambol.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )

install(TARGETS akunambol DESTINATION ${BIN_INSTALL_DIR} )
