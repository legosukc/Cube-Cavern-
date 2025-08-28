#ifndef _ERROR_HEADER
#define _ERROR_HEADER

#include <SDL2/SDL_error.h>

#include <iostream>
#include <cstdlib>

namespace _private_ErrorFuncs {

	static void _Error(const char* FailedFunctionName, const char* ErrorCode, size_t LINE, const char* FILE) {
		std::cerr << ":ERROR: " << FailedFunctionName << " failure" << std::endl;
		if (ErrorCode != nullptr) {
			std::cerr << "Error Code: " << SDL_GetError() << std::endl;
		}
		std::cerr << "At " << LINE << " in " << FILE << std::endl;
	}
	
	static inline void _FatalError(const char* FailedFunctionName, const char* ErrorCode, size_t LINE, const char* FILE) {
		_private_ErrorFuncs::_Error(FailedFunctionName, ErrorCode, LINE, FILE);
		exit(EXIT_FAILURE);
	}

#define FunctionFailedError(FailedFunctionName, ErrorCode)(_private_ErrorFuncs::_Error(FailedFunctionName, ErrorCode, __LINE__, __FILE__))
#define FatalFunctionFailedError(FailedFunctionName, ErrorCode)(_private_ErrorFuncs::_FatalError(FailedFunctionName, ErrorCode, __LINE__, __FILE__))
}

#endif