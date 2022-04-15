
include (CheckIncludeFileCXX)
include (CheckCXXSourceRuns)

find_path(BULLETML_INCLUDE_DIR bulletmlparser.h
  HINTS
  	ENV BULLETMLDIR
	PATH_SUFFIXES bulletml
                # path suffixes to search inside ENV{SDLDIR}
                include/bulletml include
)

find_library(BULLETML_LIBRARY
  NAMES bulletml
  HINTS
  ENV BULLETMLDIR
  ENV BULLETMLDIR
  PATH_SUFFIXES ${CMAKE_INSTALL_LIBDIR} ${VC_LIB_PATH_SUFFIX}
)

set(BULLETML_TEST_SOURCE [====[
#include <stdlib.h>
#include "bulletml/bulletmlparser.h"
#include "bulletml/bulletmlparser-tinyxml.h"
int main(void)
{
   BulletMLParserTinyXML parser("bulletmlparser-tinyxml.h");
   exit(EXIT_SUCCESS);
} 
]====])


set(BULLETML_LIBRARIES ${BULLETML_LIBRARY})
set(BULLETML_INCLUDE_DIRS ${BULLETML_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(BulletML
	REQUIRED_VARS BULLETML_LIBRARIES BULLETML_INCLUDE_DIRS
	VERSION_VAR BULLETML_VERSION_STRING)

set(CMAKE_REQUIRED_LIBRARIES ${BULLETML_LIBRARY})
CHECK_CXX_SOURCE_RUNS("${BULLETML_TEST_SOURCE}" CMAKE_BULLETML_HAVE_TINYXML)
unset(CMAKE_REQUIRED_LIBRARIES)

mark_as_advanced(BULLETML_LIBRARY BULLETML_INCLUDE_DIR)
