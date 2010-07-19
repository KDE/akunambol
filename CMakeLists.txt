project(akunabmol)

set(VERSION 0.1)
configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_SOURCE_DIR}/config.h)

#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

# Search KDE packages
find_package(KDE4 REQUIRED)
find_package(Akonadi REQUIRED)
find_package(KdepimLibs REQUIRED)

# Search for Funambol
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(Funambol REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)
include(CheckIncludeFiles)

include_directories(${QT_INCLUDES} ${KDE4_INCLUDES} ${Akonadi_INCLUDES} ${Funambol_INCLUDE_DIR})

set(syncsource_SRCS
    syncsource/syncconfig.cpp
    syncsource/ContactsSource.cpp
    syncsource/sourcemanager.cpp
    syncsource/statuslistener.cpp
    )

set(akonadi_extractor_SRCS
    akonadi/akonadiwalker.cpp
)

## Akonadi retreiver test application

set(akonadi_SRCS
#     akonadi/main.cpp
    akonadi/akonadi-dialog.cpp
    akonadi/contacts.cpp
    ${akonadi_extractor_SRCS}
    )


kde4_add_ui_files(akonadi_SRCS akonadi/akonadi-dialog.ui)

# kde4_add_executable(akonadi-test ${akonadi_SRCS})


set(akunambol_LINK_LIBS
    ${KDE4_KDEUI_LIBS}
    ${Kdepimlibs_LIBS}
    ${KDE4_KMIME_LIBS}
    ${KDE4_AKONADI_KMIME_LIBS}
    ${KDE4_KABC_LIBS}
    ${KDE4_KPIMUTILS_LIBS}
    ${KDE4_AKONADI_LIBS}
    microblog
)

# target_link_libraries(akonadi-test ${akunambol_LINK_LIBS})

set(akunambol_SRCS
    qtgui/configdialog.cpp
    qtgui/main.cpp
    qtgui/mainwindow.cpp
    qtgui/KDELog.cpp
    ${syncsource_SRCS}
    ${akonadi_SRCS}
    )


set_source_files_properties(${akunambol_SRCS} PROPRIETIES COMPILE_FLAGS -fexceptions)

set(akunambol_RCCS qtgui/resource.qrc)

kde4_add_ui_files(akunambol_SRCS qtgui/configdialog.ui qtgui/mainwindow.ui)

Qt4_add_resources(akunambol_SRCS ${akunambol_RCCS})

kde4_add_executable(akunambol ${akunambol_SRCS})

target_link_libraries(akunambol ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambol_LINK_LIBS})

install(TARGETS akunambol DESTINATION ${BIN_INSTALL_DIR} )

########### KCModule ####################

set(kcmsync_SRCS
    kcmsync/kcmsync.cpp
    kcmsync/syncserver.cpp
    kcmsync/syncjob.cpp
    kcmsync/serverproperties.cpp
    ${syncsource_SRCS}
    ${akonadi_SRCS}
    )

kde4_add_ui_files(kcmsync_SRCS kcmsync/kcmsync.ui kcmsync/serverproperties.ui)

kde4_add_plugin(kcm_sync ${kcmsync_SRCS})

target_link_libraries(kcm_sync ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambol_LINK_LIBS})

install(TARGETS kcm_sync DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES kcmsync/kcm_sync.desktop  DESTINATION ${SERVICES_INSTALL_DIR})

########### KDEDModule ####################

set(kded_akunambolsync_SRCS
    kded/akunambolsyncdaemon.cpp
    kcmsync/syncserver.cpp
    kcmsync/syncjob.cpp
    ${syncsource_SRCS}
    ${akonadi_SRCS}
    )


qt4_add_dbus_adaptor(kded_akunambolsync_SRCS kded/org.kde.akunambolsync.xml kded/akunambolsyncdaemon.h AkunambolSyncDaemon)

kde4_add_plugin(kded_akunambolsync ${kded_akunambolsync_SRCS})

target_link_libraries(kded_akunambolsync ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambol_LINK_LIBS} ${KDE4_SOLID_LIBS})

install(TARGETS kded_akunambolsync DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES kded/akunambolsync.desktop  DESTINATION ${SERVICES_INSTALL_DIR}/kded)


## Funambol SDK test application

set(test_SRCS
	#    test/main.cpp
	#    test/dialog.cpp
    ${syncsource_SRCS}
    )

#kde4_add_ui_files(test_SRCS test/dialog.ui)

#kde4_add_executable(test-source ${test_SRCS})

#target_link_libraries(test-source ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY})



