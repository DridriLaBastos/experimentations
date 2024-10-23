#include <iostream>

#include "module.hpp"

Module::Module(const std::filesystem::path& modulePath,const std::filesystem::path& moduleCopyPath): mModulePath(modulePath),mModuleCopyPath(moduleCopyPath),mSharedModule(nullptr),mModuleLastWriteTime()
{
	if (std::filesystem::exists(modulePath))
	{
		const std::error_code errorCode = CopyAndLoad();
		if (errorCode)
		{
			fprintf(stderr,"Loading '%s' failed with error %d : '%s'\n",modulePath.c_str(),Platform::GetSharedModuleErrorCode(),Platform::GetSharedModuleErrorDescription(Platform::GetSharedModuleErrorCode()).c_str());
		}
	}
}

Module::~Module()
{
	if(!Platform::UnloadSharedModule(&mSharedModule))
	{
		fprintf(stderr, "Unloading module '%s' failed with error %d : '%s'",mModulePath.c_str(),Platform::GetSharedModuleErrorCode(),Platform::GetSharedModuleErrorDescription(Platform::GetSharedModuleErrorCode()).c_str());
	}
}

std::error_code Module::CopyAndLoad()
{
	std::error_code errorCode;
	const bool status = std::filesystem::copy_file(mModulePath,mModuleCopyPath,std::filesystem::copy_options::overwrite_existing,errorCode);

	if (!status)
	{
		return errorCode;
	}

	mModuleLastWriteTime = std::filesystem::last_write_time(mModuleCopyPath);
	Platform::UnloadSharedModule(&mSharedModule);
	Platform::LoadSharedModule(mModuleCopyPath,&mSharedModule);
	return errorCode;
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

	const std::error_code errorCode = CopyAndLoad();
	if (errorCode)
	{
		fprintf(stderr, "Copying '%s' failed with error '%d' : '%s'",mModulePath.c_str(),errorCode.value(),Platform::GetPlatformErrorCodeDescription(errorCode.value()).c_str());
		return false;
	}

	return true;
}
