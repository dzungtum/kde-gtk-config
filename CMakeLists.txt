project(chakragtkconfig)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_PREFIX_PATH /usr)

message("PREFIX : ${CMAKE_PREFIX_PATH}")
find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES})

# Codigos Fuente
set(kcm_SRCS installer.cpp aparienciagtk.cpp modulo.cpp dialog_installer.cpp dialog_uninstaller.cpp thread.cpp)

# Uis 
message("CARPETA ACTUAL : ${chakragtkconfig_SOURCE_DIR}")
kde4_add_ui_files(kcm_SRCS gui.ui dialog_installer.ui dialog_uninstaller.ui)
kde4_add_plugin(kcm_cgc ${kcm_SRCS})

target_link_libraries(kcm_cgc ${KDE4_KIO_LIBS} )

message("AQUI SE INSTALARA EL PLUGIN = ${PLUGIN_INSTALL_DIR}")

install(TARGETS kcm_cgc DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES chakra-gtk-config.desktop DESTINATION ${KDE4_SERVICES_INSTALL_DIR})