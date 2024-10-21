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
	bool LoadSymbol(const std::string& symbolName, Ret(*symbolFunction)(Args...))
	{
		*symbolFunction = nullptr;
		return Platform::LoadSymbol(&mSharedModule,symbolName,(void**)&symbolFunction);
	}

	const std::filesystem::path mModulePath;
	const std::filesystem::path mModuleCopyPath;

private:
	SharedModuleHandle mSharedModule;
	std::filesystem::file_time_type mModuleLastWriteTime;
};

#endif