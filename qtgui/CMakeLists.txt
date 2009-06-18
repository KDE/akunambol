
include_directories(${QT_INCLUDES})

set(akunambol_SRCS
   config.cpp
   main.cpp
   mainwindow.cpp
 )

set (akunambol_RCCS
   resource.qrc
)

kde4_add_ui_files(akunambol_SRCS config.ui mainwindow.ui)

#kde4_add_kcfg_files(akunambol_SRCS settings.kcfgc )

Qt4_add_resources(akunambol_SRCS ${akunambol_RCCS})

kde4_add_executable(akunambol ${akunambol_SRCS})

target_link_libraries(akunambol ${KDE4_KDEUI_LIBS} )

install(TARGETS akunambol DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

