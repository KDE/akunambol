# - Try to find Funambol
# Once done, this will define
#
#  Funambol_FOUND - system has Funambol
#  Funambol_INCLUDE_DIRS - the Funambol include directories
#  Funambol_LIBRARIES - link these to use Funambol

include(LibFindMacros)

# Include dir
find_path(Funambol_INCLUDE_DIR
  # TODO: change that to a file that will always be there?
  NAMES funambol/common/client/SyncClient.h
)

# Hack?
set(Funambol_INCLUDE_DIR ${Funambol_INCLUDE_DIR}/funambol/posix ${Funambol_INCLUDE_DIR}/funambol/common)

# Finally the library itself
find_library(Funambol_LIBRARY
  NAMES funambol
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
set(Funambol_PROCESS_INCLUDES Funambol_INCLUDE_DIR Funambol_INCLUDE_DIRS)
set(Funambol_PROCESS_LIBS Funambol_LIBRARY Funambol_LIBRARIES)
libfind_process(Funambol)

