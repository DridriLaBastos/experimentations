#include "graphics/ray.hpp"
#include "graphics/camera.hpp"

Camera::Camera(const Point3f& center, const float focalLength): mCenter(center), mFocalLength(focalLength), mSamplePerPixel(4)
{
	WithAspectRatio(16.0/9.0,300,4.0);
}

void Camera::ComputeInternal(const unsigned int pixelWidth, const unsigned int pixelHeight,
							const float viewPortWidth, const float viewportHeight)
{
	mPixelWidth = pixelWidth;
	mPixelHeight = pixelHeight;
	mViewportWidth = viewPortWidth;
	mViewportHeight = viewportHeight;

	mBuffer.resize(pixelWidth*pixelHeight);
	mTexture.create(pixelWidth,pixelHeight);
	mSprite.setTexture(mTexture);

	mPixelDx = viewPortWidth / mPixelWidth;
	mPixelDy = viewportHeight / pixelHeight;

	Point3f viewportCenter = mCenter - Point3f{.0f,.0f, mFocalLength };
	Point3f viewportUpperLeft = viewportCenter + Point3f(-viewPortWidth/2.f,viewportHeight/2.f,0);
	mPixel00Loc = viewportUpperLeft + Point3f{ mPixelDx/2.0,-mPixelDy/2.0,0 };
}

Camera& Camera::WithAspectRatio(const float widthOverHeightRatio, const float pixelWidth, const float viewPortWidth)
{
	ComputeInternal(pixelWidth,pixelWidth/widthOverHeightRatio,viewPortWidth,viewPortWidth/widthOverHeightRatio);
	return *this;
}

Camera& Camera::WithSamplePerPixel(const unsigned int sample)
{
	mSamplePerPixel = sample;
	return *this;
}

static Color3f RayColor(const Ray& r, const HittableList& world)
{
	HitRecord rec;

	if (world.Hit(r,0,100,rec))
	{
		return .5f*Color3f(rec.normal.x+1.f,rec.normal.y+1.f,rec.normal.z+1.f);
	}

	const Vec3f unitDirection = glm::normalize(r.Direction());
	const float a = .5f*(unitDirection.y + 1.0);
	// printf("%.3f   %.3f   %.3f\n",r.Direction().x,r.Direction().y,r.Direction().z);
	return (1.f-a)*Color3f(1.0, 1.0, 1.0) + a*Color3f(0.5, 0.7, 1.0);
}

void Camera::Render(const HittableList& world)
{
	for (size_t y = 0; y < mPixelHeight; y += 1)
	{
		for (size_t x = 0; x < mPixelWidth; x += 1)
		{
			const Point3f currentPixel = mPixel00Loc + Vec3f{x*mPixelDx,-(float)y*mPixelDy,0};
			const Ray r (mCenter,currentPixel-mCenter);
			// printf("%.3f   %.3f   %.3f\n",mViewportWidth,mViewportHeight,currentPixel.z);
			const Color3f c = RayColor(r,world);

			const size_t currentPixelIndex = y * mPixelWidth + x;

			mBuffer[currentPixelIndex].red = c.r*255;
			mBuffer[currentPixelIndex].green = c.g*255;
			mBuffer[currentPixelIndex].blue = c.b*255;
			mBuffer[currentPixelIndex].alpha = 255;
		}
	}
}

void Camera::Display(sf::RenderWindow& target)
{
	mTexture.update((sf::Uint8*)mBuffer.data());
	target.draw(mSprite);
}
