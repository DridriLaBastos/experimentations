#include "platforme.hpp"

SharedModuleError Platform::GetSharedModuleErrorCode()
{
	return GetLastError();
}

std::string Platform::GetSharedModuleErrorDescription(const SharedModuleError errorCode)
{
	return GetPlatformErrorCodeDescription((PlatformErrorCode)errorCode);
}

std::string Platform::GetPlatformErrorCodeDescription(const PlatformErrorCode error)
{
	LPSTR msgBuffer = NULL;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL,error,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),msgBuffer,0,NULL);

	std::string ret (msgBuffer);

	LocalFree(msgBuffer);

	return ret;
}

bool Platform::LoadSharedModule(const std::filesystem::path path, SharedModuleHandle* outHandle)
{
	HMODULE module = LoadLibrary(path.generic_string().c_str());
	*outHandle = module;
	return module != NULL;
}

bool Platform::UnloadSharedModule(SharedModuleHandle* sharedModuleHandle)
{
	return FreeLibrary(*sharedModuleHandle);
}

bool Platform::LoadSymbol(const SharedModuleHandle* sharedModuleHandle, const std::string& symbol, void** sym)
{
	FARPROC proc = GetProcAddress(*sharedModuleHandle,symbol.c_str());
	*sym = (void*)proc;
	return proc != NULL;
}
