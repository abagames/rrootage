#ifndef bulletmlcommon_h_
#define bulletmlcommon_h_

#ifdef _WIN32
	#ifdef WIN32_DLL_EXPORT
	# define DECLSPEC __declspec(dllexport)
	#else
	# define DECLSPEC
	#endif
#endif

#include <string.h>

#endif // ! bulletmlcommon_h_

