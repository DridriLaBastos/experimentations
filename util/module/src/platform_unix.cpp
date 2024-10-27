#include <dlfcn.h>
#include <cstring>

#include "platforme.hpp"

bool Platform::LoadSharedModule(const std::filesystem::path path, SharedModuleHandle* outHandle)
{
	*outHandle = dlopen(path.c_str(),RTLD_LAZY);
	return *outHandle != nullptr;
}

bool Platform::UnloadSharedModule(SharedModuleHandle* sharedModuleHandle)
{
	return dlclose(*sharedModuleHandle) == 0;
}

bool Platform::LoadSymbol(const SharedModuleHandle* sharedModuleHandle, const std::string& symbol, void** sym)
{
	void* symHandler = dlsym(*sharedModuleHandle,symbol.c_str());

	if (symHandler == nullptr)
	{
		return false;
	}

	*sym = symHandler;
	return true;
}

SharedModuleError Platform::GetSharedModuleErrorCode(void)
{
	return 0;
}

std::string Platform::GetSharedModuleErrorDescription(const SharedModuleError error)
{
	(void)error;
	return dlerror();
}

std::string Platform::GetPlatformErrorCodeDescription(const PlatformErrorCode errorCode)
{
	return strerror(errorCode);
}