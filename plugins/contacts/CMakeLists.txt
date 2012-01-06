
include_directories("${CMAKE_SOURCE_DIR}/library")

set(contacts_SRCS
    contactssource.cpp
    contactsbackend.cpp
)

kde4_add_plugin(akunambol_syncsource_contacts ${contacts_SRCS})
target_link_libraries(akunambol_syncsource_contacts
                      ${Funambol_LIBRARY}
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_KIO_LIBS}
                      ${akunambol_LINK_LIBS}
                      akunambolprivate
                      )

install(TARGETS akunambol_syncsource_contacts DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES akunambol-syncsource-contacts.desktop DESTINATION ${SERVICES_INSTALL_DIR})
