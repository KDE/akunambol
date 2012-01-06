
include_directories(${CMAKE_SOURCE_DIR}/library)

set(common_SRCS src/funambolsyncsource.cpp
                src/sourcemanager.cpp
                src/appsyncsource.cpp
                src/appsyncsourceconfig.cpp
                src/collectionsfetcher.cpp
                src/akonadisource.cpp
                src/KFunSyncConfig.cpp)

kde4_add_library(akunambol_plugin_funambol_common SHARED ${common_SRCS})

set(common_LINK_LIBS
    ${Funambol_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDEPIMLIBS_AKONADI_KMIME_LIBS}
    ${KDEPIMLIBS_AKONADI_LIBS}
    ${KDEPIMLIBS_KABC_LIBS}
    ${KDEPIMLIBS_KCAL_LIBS}
    ${KDEPIMLIBS_KMIME_LIBS}
    ${KDEPIMLIBS_KPIMUTILS_LIBS}
    ${Kdepimlibs_LIBS}
    akunambolprivate
)

target_link_libraries(akunambol_plugin_funambol_common ${common_LINK_LIBS})

