# - Try to find Funambol
# Once done, this will define
#
#  FUNAMBOL_FOUND - system has Funambol
#  Funambol_INCLUDE_DIR - the Funambol include directories
#  Funambol_LIBRARY - link these to use Funambol
#  Funambol_DEFINITIONS - Compiler flags required by Funambol

if(NOT WIN32)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(PC_Funambol libfunambol)
  set(Funambol_DEFINITIONS ${PC_Funambol_CFLAGS})
endif()

find_path(Funambol_INCLUDE_DIR NAMES funambol/common/client/SyncClient.h
  HINTS ${PC_Funambol_INCLUDEDIR}
)

find_library(Funambol_LIBRARY NAMES funambol
  HINTS ${PC_Funambol_LIBDIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Funambol DEFAULT_MSG Funambol_INCLUDE_DIR Funambol_LIBRARY Funambol_DEFINITIONS)

mark_as_advanced(Funambol_INCLUDE_DIR Funambol_LIBRARY Funambol_DEFINITIONS)
