#pragma once

#ifdef _WIN32
	#ifdef _WIN64
		#define AU_WINDOWS_PLATFORM
	#else
		#error x86 builds are not supported
	#endif
#endif