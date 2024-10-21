#ifndef PLATFORM_UTIL_HPP
#define PLATFORM_UTIL_HPP

#ifdef WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __attribute__((visibility("default")))
#endif

#endif