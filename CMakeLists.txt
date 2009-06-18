project(akunabmol)

# Search for Funambol
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package (Funambol REQUIRED)

# search packages used by KDE
find_package (KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)
include(CheckIncludeFiles)
include(FindKdepimLibs)

#find_program(XSLTPROC_EXECUTABLE xsltproc)
#macro_log_feature(XSLTPROC_EXECUTABLE "xsltproc" "The command line XSLT processor from libxslt" "http://xmlsoft.org/XSLT/" FALSE "" "Needed for building Akonadi resources. Recommended.")

add_subdirectory(qtgui)
#add_subdirectory(resource)

