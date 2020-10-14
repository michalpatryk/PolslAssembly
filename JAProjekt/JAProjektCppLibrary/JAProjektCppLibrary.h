
#pragma once
#ifdef JAPROJEKTCPPLIBRARY_EXPORTS
#define JAPROJEKTCPPLIBRARY_API __declspec(dllexport)
#else
#define JAPROJEKTCPPLIBRARY_API __declspec(dllimport)
#endif

extern "C" JAPROJEKTCPPLIBRARY_API int testFunction(unsigned long a);
