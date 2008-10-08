set(akunambol_SRCS
   akunambol.cpp
   main.cpp
 )

kde4_add_ui_files(akunambol_SRCS akunambolMainWindow.ui prefs_base.ui)

kde4_add_kcfg_files(akunambol_SRCS settings.kcfgc )

kde4_add_executable(akunambol ${akunambol_SRCS})

target_link_libraries(akunambol ${KDE4_KDEUI_LIBS} )

install(TARGETS akunambol DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES akunambol.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES akunambol.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES akunambolui.rc  DESTINATION  ${DATA_INSTALL_DIR}/akunambol )
