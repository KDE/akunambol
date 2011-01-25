project(akunambol)

# Search KDE packages
find_package(KDE4 REQUIRED)
find_package(Akonadi REQUIRED)
find_package(KdepimLibs REQUIRED)
find_package(LikeBack REQUIRED)

# Search for Funambol
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(Funambol REQUIRED)

# include(KDE4Defaults)

include(KDE4Defaults)
include(MacroLibrary)
include(CheckIncludeFiles)

add_subdirectory(images/icons)

set(VERSION "0.2.70")
configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake  ${CMAKE_CURRENT_BINARY_DIR}/aku-auto-config.h)


add_definitions(-DENABLE_FUNAMBOL_NAMESPACE)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )


include_directories(${QT_INCLUDES} ${KDE4_INCLUDES} ${Akonadi_INCLUDES} ${KDEPIMLIBS_INCLUDE_DIRS} ${Funambol_INCLUDE_DIR} ${LIKEBACK_INCLUDE_DIR})

######### libakunambol

set(akunambolprivate_SRCS
    library/syncsource.cpp
    library/funambolsyncsource.cpp
    library/funambolbackend.cpp
    library/syncsourceloader.cpp
    library/synccredentials.cpp
    )

# kde4_add_ui_files(kcmsync_SRCS kcmsync/kcmsync.ui kcmsync/serverproperties.ui)

## FIXME STATIC??? WTF???
kde4_add_library(akunambolprivate SHARED ${akunambolprivate_SRCS})

target_link_libraries(akunambolprivate ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambolprivate_LINK_LIBS})
set_target_properties(akunambolprivate PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS akunambolprivate ${INSTALL_TARGETS_DEFAULT_ARGS} )

# install(TARGETS akunambolprivate DESTINATION ${LIBRARY_OUTPUT_PATH})

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
#     akonadi/contacts.cpp
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

set(akunambol_RCCS qtgui/resource.qrc)

kde4_add_ui_files(akunambol_SRCS qtgui/mainwindow.ui qtgui/standardsourcesettings.ui
                  qtgui/settings.ui)

Qt4_add_resources(akunambol_SRCS ${akunambol_RCCS})

kde4_add_executable(akunambol ${akunambol_SRCS})


target_link_libraries(akunambol ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambol_LINK_LIBS} akunambolprivate -lkcal)

install(FILES akunambol.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )

install(FILES akunambol-syncsource.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR} )

install(TARGETS akunambol DESTINATION ${BIN_INSTALL_DIR} )

add_subdirectory(plugins/contacts)

