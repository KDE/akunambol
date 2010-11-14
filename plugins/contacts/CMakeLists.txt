
# Search KDE packages
find_package(KDE4 REQUIRED)
# find_package(Akonadi REQUIRED)
# find_package(KdepimLibs REQUIRED)

# Search for Funambol
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(Funambol REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)
include(CheckIncludeFiles)

include_directories(${QT_INCLUDES} ${KDE4_INCLUDES} ${Akonadi_INCLUDES} ${Funambol_INCLUDE_DIR} "${CMAKE_SOURCE_DIR}/library")

# Test application
kde4_add_executable(test-plugins ../test/main.cpp)

target_link_libraries(test-plugins ${KDE4_KDEUI_LIBS} kdecore ${Funambol_LIBRARY} ${akunambol_LINK_LIBS} ${akunambolprivate_LINK_LIBS} ${Kdepimlibs_LIBS})

######

set(contacts_SRCS
contactssource.cpp)

kde4_add_plugin(akunambol_syncsource_contacts ${contacts_SRCS}) 
target_link_libraries(akunambol_syncsource_contacts ${akunambol_LINK_LIBS} kdecore akunambolprivate ${Funambol_LIBRARY} ${KDE4_KIO_LIBS})

install(TARGETS akunambol_syncsource_contacts DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES akunambol-syncsource-contacts.desktop DESTINATION ${SERVICES_INSTALL_DIR})
