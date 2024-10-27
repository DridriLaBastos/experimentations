#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>

#include "SFML/Graphics.hpp"

#include "util.hpp"

#include "geometry/hittable_list.hpp"

#include "graphics/color.hpp"

class Camera
{
public:
	Camera(const Point3f& center, const float focalLength);

	void Render(const HittableList& world);
	void Display(sf::RenderWindow& target);

public:
	Camera& WithAspectRatio(const float widthOverHeightRatio, const float pixelWidth, const float viewportWidth);

private:
	void ComputeInternal(const unsigned int pixelWidth, const unsigned int pixelHeight,
							const float viewPortWidth, const float viewportHeight);

private:
	Point3f mCenter;
	Point3f mPixel00Loc;
	float mFocalLength;

	float mPixelWidth;
	float mPixelHeight;
	float mViewportHeight;
	float mViewportWidth;

	float mPixelDx;
	float mPixelDy;

	sf::Texture mTexture;
	sf::Sprite mSprite;

	std::vector<RenderColor> mBuffer;
};

#endif