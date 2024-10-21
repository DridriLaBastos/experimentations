#ifndef RAYTRACING_HPP
#define RAYTRACING_HPP

#include <cstddef>
#include "color.hpp"
#include "util/module/platform_util.hpp"

namespace Raytracing
{
	struct RenderingInfo
	{
		size_t width;
		size_t height;

		Color* buffer;
	};
}

#define QUOTE(macro) #macro
#define FUNCTION_NAME(name) QUOTE(name)

#define RAYTRACING_DRAW_MODULE_FUNC_RET void
#define RAYTRACING_DRAW_MODULE_FUN_PARAMETER Raytracing::RenderingInfo* outRenderingInfo
#define RAYTRACING_DRAW_MODULE_SYMBOL_NAME Raytracing_Compute

#define RAYTRACING_DRAW_MODULE_FUNC_DEFINITION RAYTRACING_DRAW_MODULE_FUNC_RET Raytracing_Compute (RAYTRACING_DRAW_MODULE_FUN_PARAMETER)

extern "C" DLL_EXPORT RAYTRACING_DRAW_MODULE_FUNC_DEFINITION;

#endif