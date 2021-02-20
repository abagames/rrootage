#ifndef bulletmlcommon_h_
#define bulletmlcommon_h_

#ifndef DECLSPEC
	#ifdef WIN32_DLL_EXPORT
	# define DECLSPEC __declspec(dllexport)
	#else
	# define DECLSPEC
	#endif
#endif

#include <string.h>

#endif // ! bulletmlcommon_h_

