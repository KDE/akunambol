project(akunambol)

# Search for Funambol
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

# Search KDE packages
find_package(KDE4 REQUIRED)

# include(KDE4Defaults)
include(KDE4Defaults)

find_package(KdepimLibs)
macro_log_feature(KdepimLibs_FOUND "KDE PIM Libs" "KDE Libraries for PIM" "http://pim.kde.org/" TRUE "" "Required to build Akunambol")

# Set Akonadi_MIN_VERSION if needed
find_package(Akonadi QUIET NO_MODULE)
macro_log_feature(Akonadi_FOUND "Akonadi server libraries" "Access to PIM storage and services" "http://pim.kde.org/akonadi" TRUE "${Akonadi_MIN_VERSION}" "")

macro_optional_find_package(LikeBack)

if( NOT LIKEBACK_FOUND )
    add_definitions("-DNO_LIKEBACK")
endif( NOT LIKEBACK_FOUND )

find_package(Funambol)
macro_log_feature(FUNAMBOL_FOUND "LibFunambol" "SyncML client engine" "http://funambol.com" TRUE "" "")

add_subdirectory(images/icons)

set(VERSION "0.2.70")
configure_file(${CMAKE_SOURCE_DIR}/config.h.cmake  ${CMAKE_CURRENT_BINARY_DIR}/aku-auto-config.h)

add_definitions(${Funambol_DEFINITIONS} -DQT_STRICT_ITERATORS)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )


include_directories(${QT_INCLUDES} ${KDE4_INCLUDES} ${Akonadi_INCLUDES} ${KDEPIMLIBS_INCLUDE_DIRS} ${Funambol_INCLUDE_DIR})

if (LIKEBACK_FOUND)
 include_directories(${LIKEBACK_INCLUDE_DIR})
endif(LIKEBACK_FOUND)

######### libakunambol

set(akunambolprivate_SRCS
    library/syncsource.cpp
    library/syncjob.cpp
    library/syncitem.cpp
    library/funambolsyncsource.cpp
    library/funambolconfig.cpp
    library/funambolbackend.cpp
    library/cachedfunambolbackend.cpp
    library/funambolsyncitem.cpp
    library/funambolsyncjob.cpp
    library/syncsourceloader.cpp
    library/synccredentials.cpp
    )

# kde4_add_ui_files(kcmsync_SRCS kcmsync/kcmsync.ui kcmsync/serverproperties.ui)

## FIXME STATIC??? WTF???
kde4_add_library(akunambolprivate SHARED ${akunambolprivate_SRCS})

target_link_libraries(akunambolprivate ${QT_QTSQL_LIBRARY} ${KDE4_KDEUI_LIBS} ${Funambol_LIBRARY} ${akunambolprivate_LINK_LIBS})
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
    ${KDEPIMLIBS_KMIME_LIBS}
    ${KDEPIMLIBS_AKONADI_KMIME_LIBS}
    ${KDEPIMLIBS_KABC_LIBS}
    ${KDEPIMLIBS_KPIMUTILS_LIBS}
    ${KDEPIMLIBS_AKONADI_LIBS}
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


target_link_libraries(akunambol
                      ${Funambol_LIBRARY}
                      ${KDE4_KDEUI_LIBS}
                      ${KDE4_KUTILS_LIBS}
                      ${KDEPIMLIBS_KCAL_LIBS}
                      ${akunambol_LINK_LIBS}
                      akunambolprivate
                      )

install(FILES akunambol.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )

install(FILES akunambol-syncsource.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR} )

install(TARGETS akunambol DESTINATION ${BIN_INSTALL_DIR} )

add_subdirectory(plugins/contacts)

macro_display_feature_log()
