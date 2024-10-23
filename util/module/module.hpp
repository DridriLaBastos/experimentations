#ifndef MODULE_HPP
#define MODULE_HPP

#include <string>
#include <filesystem>

#include "platforme.hpp"

class Module
{
public:
	Module(const std::filesystem::path& modulePath, const std::filesystem::path& moduleCopyPath);
	~Module(void);

public:
	bool NeedReload(void) const;
	bool ReloadIfNeeded(void);

	template <typename Ret, typename ...Args>
	bool LoadSymbol(const std::string& symbolName, Ret(**symbolFunction)(Args...))
	{
		*symbolFunction = DoNothing;
		const bool status = Platform::LoadSymbol(&mSharedModule,symbolName,(void**)symbolFunction);

		if (!status)
		{
			fprintf(stderr, "Loading symbol '%s' failed with error %d : '%s'",
				symbolName.c_str(),Platform::GetSharedModuleErrorCode(),Platform::GetSharedModuleErrorDescription(Platform::GetSharedModuleErrorCode()).c_str());
		}

		return status;
	}

private:
	template <typename Ret, typename ...Args>
	static Ret DoNothing(Args...)
	{
		return Ret();
	}

private:
	std::error_code CopyAndLoad(void);

	const std::filesystem::path mModulePath;
	const std::filesystem::path mModuleCopyPath;

private:
	void DoNothing(void);

private:
	SharedModuleHandle mSharedModule;
	std::filesystem::file_time_type mModuleLastWriteTime;
};

#endif