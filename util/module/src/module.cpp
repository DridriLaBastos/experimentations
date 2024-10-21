#include <dlfcn.h>
#include <iostream>

#include "module.hpp"

Module::Module(const std::filesystem::path& modulePath,const std::filesystem::path& moduleCopyPath): mModulePath(modulePath),mModuleCopyPath(moduleCopyPath),mSharedModule(nullptr),mModuleLastWriteTime()
{
	if (std::filesystem::exists(modulePath))
	{
		if (!Platform::LoadSharedModule(modulePath,&mSharedModule))
		{
			fprintf(stderr,"Loading '%s' failed with error %d : '%s'\n",modulePath.c_str(),Platform::GetSharedModuleErrorCode(),Platform::GetSharedModuleErrorDescription(Platform::GetSharedModuleErrorCode()).c_str());
		}
		mModuleLastWriteTime = std::filesystem::last_write_time(modulePath);
	}
}

Module::~Module()
{
	if(!Platform::UnloadSharedModule(&mSharedModule))
	{
		fprintf(stderr, "Unloading module '%s' failed with error %d : '%s'",mModulePath.c_str(),Platform::GetSharedModuleErrorCode(),Platform::GetSharedModuleErrorDescription(Platform::GetSharedModuleErrorCode()).c_str());
	}
}

bool Module::NeedReload() const
{
	if (!std::filesystem::exists(mModulePath))
	{
		return false;
	}

	return std::filesystem::last_write_time(mModulePath) > mModuleLastWriteTime;
}

bool Module::ReloadIfNeeded()
{
	if (!NeedReload())
	{
		return false;
	}

	std::error_code errorCode;
	const bool status = std::filesystem::copy_file(mModulePath,mModuleCopyPath,std::filesystem::copy_options::overwrite_existing,errorCode);

	if (!status)
	{
		fprintf(stderr, "Copying '%s' failed with error '%d' : '%s'",mModulePath.c_str(),errorCode.value(),Platform::GetPlatformErrorCodeDescription(errorCode.value()).c_str());
		return false;
	}

	Platform::UnloadSharedModule(&mSharedModule);
	return Platform::LoadSharedModule(mModuleCopyPath,&mSharedModule);
}
