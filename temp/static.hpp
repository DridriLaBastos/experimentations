#ifndef STATIC_HPP
#define STATIC_HPP

namespace NS
{
	extern "C" void StaticFunction(void) __attribute__((visibility("default")));
}

#endif