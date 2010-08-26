
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

include_directories(${QT_INCLUDES} ${KDE4_INCLUDES} ${Akonadi_INCLUDES} ${Funambol_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/library")

set(common_SRCS src/funambolsyncsource.cpp
                src/sourcemanager.cpp
                src/appsyncsource.cpp
                src/appsyncsourceconfig.cpp
                src/collectionsfetcher.cpp
                src/akonadisource.cpp
                src/KFunSyncConfig.cpp)

kde4_add_library(akunambol_plugin_funambol_common SHARED ${common_SRCS})

set(common_LINK_LIBS 
    ${KDE4_KDEUI_LIBS}
    ${Kdepimlibs_LIBS}
    ${KDE4_KMIME_LIBS}
    ${KDE4_AKONADI_KMIME_LIBS}
    ${KDE4_KABC_LIBS}
    ${KDE4_KPIMUTILS_LIBS}
    ${KDE4_AKONADI_LIBS}
    ${Funambol_LIBRARY}
    ${KDE4_KIO_LIBS}
    kdecore
    kcal
    akunambolprivate
)


target_link_libraries(akunambol_plugin_funambol_common ${common_LINK_LIBS})

