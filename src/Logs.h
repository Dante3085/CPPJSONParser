#pragma once

#define DEBUG_LOGS

#ifdef DEBUG_LOGS
	#define LOG(x) std::cout << x << std::endl
#else
	#define LOG(x)
#endif