#ifndef PLATFORM_UTIL_HPP
#define PLATFORM_UTIL_HPP

#ifdef WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif

#ifdef WIN32
#define DLL_INIT
#else
#define DLL_INIT __attribute__((constructor))
#endif

#ifdef WIN32
#define DLL_CLEAR
#else
#define DLL_CLEAR __attribute__((destructor))
#endif

#endif
