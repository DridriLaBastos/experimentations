#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <string>
#include <filesystem>

#ifdef WIN32
#else
	using SharedModuleHandle = void*;
	using SharedModuleError  = int;
	using PlatformErrorCode  = int;
#endif

namespace Platform
{
	bool LoadSharedModule(const std::filesystem::path path, SharedModuleHandle* outHandle);
	bool UnloadSharedModule(SharedModuleHandle* sharedModuleHandle);

	bool LoadSymbol(const SharedModuleHandle* sharedModuleHandle, const std::string& symbol, void** sym);

	SharedModuleError GetSharedModuleErrorCode(void);
	std::string GetSharedModuleErrorDescription(const SharedModuleError error);
	std::string GetPlatformErrorCodeDescription(const PlatformErrorCode errorCode);
}

#endif