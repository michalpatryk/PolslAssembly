
#pragma once
#ifdef JAPROJEKTCPPLIBRARY_EXPORTS
#define JAPROJEKTCPPLIBRARY_API __declspec(dllexport)
#else
#define JAPROJEKTCPPLIBRARY_API __declspec(dllimport)
#endif

extern "C" JAPROJEKTCPPLIBRARY_API int testFunction(unsigned long a);
extern "C" JAPROJEKTCPPLIBRARY_API void cppBinarization1(char* begin, char* end, long biWidth, float treshold);
extern "C" JAPROJEKTCPPLIBRARY_API void cppHistogram1(char* begin, char* end, long biWidth, unsigned long long R[255], unsigned long long G[255], unsigned long long B[255]);