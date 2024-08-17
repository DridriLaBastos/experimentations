//
// Created by Adrien COURNAND on 17/08/2024.
//

#ifndef EXPERIMENTATIONS_PPM_HPP
#define EXPERIMENTATIONS_PPM_HPP

#include <memory>
#include <filesystem>

#include <cstddef>

#include "vec.hpp"

class PPMUtil
{
public:
	PPMUtil(const size_t w, const size_t h);

public:
	UVec4& at (const size_t x, const size_t y);
	[[nodiscard]] const UVec4& at (const size_t x, const size_t y) const;
	void write (const std::filesystem::path& path, const uint16_t maxColor = 256);

public:
	const size_t width, height;
	std::unique_ptr<UVec4> m_buffer;
};


#endif //EXPERIMENTATIONS_PPM_HPP
