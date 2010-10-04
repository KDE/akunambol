
cmake_minimum_required(VERSION 2.8)

project(akunabmol)

add_subdirectory(images/icons)

set(VERSION "0.2.70")
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

######### libakunambol

set(akunambolprivate_SRCS
    library/syncsource.cpp
    library/syncconfig.cpp
    )

# kde4_add_ui_files(kcmsync_SRCS kcmsync/kcmsync.ui kcmsync/serverproperties.ui)

kde4_add_library(akunambolprivate SHARED ${akunambolprivate_SRCS})

target_link_libraries(akunambolprivate ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambolprivate_LINK_LIBS} ${akunambol_LINK_LIBS})

install(TARGETS akunambolprivate DESTINATION ${LIBRARY_OUTPUT_PATH})

# install( FILES kcmsync/kcm_sync.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
###############
set(syncsource_SRCS
    syncsource/akonadisource.cpp
    syncsource/KFunSyncConfig.cpp
    syncsource/contactssource.cpp
    syncsource/eventssource.cpp
    syncsource/taskssource.cpp
    syncsource/sourcemanager.cpp
    syncsource/statuslistener.cpp
    akonadi/contacts.cpp
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


target_link_libraries(akunambol ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambol_LINK_LIBS} ${akunambolprivate_LINK_LIBS} -lkcal)

########### KCModule ####################
# 
# set(kcmsync_SRCS
#     kcmsync/kcmsync.cpp
#     kcmsync/syncserver.cpp
#     kcmsync/syncjob.cpp
#     kcmsync/serverproperties.cpp
#     ${syncsource_SRCS}
#     ${akonadi_SRCS}
#     )
# 
# kde4_add_ui_files(kcmsync_SRCS kcmsync/kcmsync.ui kcmsync/serverproperties.ui)
# 
# kde4_add_plugin(kcm_sync ${kcmsync_SRCS})
# 
# target_link_libraries(kcm_sync ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambol_LINK_LIBS})
# 
# install(TARGETS kcm_sync DESTINATION ${PLUGIN_INSTALL_DIR})
# 
# install( FILES kcmsync/kcm_sync.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
# 
# ########### KDEDModule ####################
# 
# set(kded_akunambolsync_SRCS
#     kded/akunambolsyncdaemon.cpp
#     kcmsync/syncserver.cpp
#     kcmsync/syncjob.cpp
#     ${syncsource_SRCS}
#     ${akonadi_SRCS}
#     )
# 
# ######### PLUGINS ########################
# add_subdirectory(plugins)
# 
# qt4_add_dbus_adaptor(kded_akunambolsync_SRCS kded/org.kde.akunambolsync.xml kded/akunambolsyncdaemon.h AkunambolSyncDaemon)
# 
# kde4_add_plugin(kded_akunambolsync ${kded_akunambolsync_SRCS})
# 
# target_link_libraries(kded_akunambolsync ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambol_LINK_LIBS} ${KDE4_SOLID_LIBS})
# 
# install(TARGETS kded_akunambolsync DESTINATION ${PLUGIN_INSTALL_DIR})
# 
# install( FILES kded/akunambolsync.desktop  DESTINATION ${SERVICES_INSTALL_DIR}/kded)

install(FILES akunambol.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )

install(TARGETS akunambol DESTINATION ${BIN_INSTALL_DIR} )

