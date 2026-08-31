#include "Renderer.h"
#include <iostream>
#include "Walnut/Random.h"

#include <execution>
#include <algorithm>
#include <limits>

#define MT 1

using namespace XRay;

Renderer::Renderer(std::shared_ptr<Scene> p_CurrentScene)
	: m_CurrentScene(p_CurrentScene)
{
}

void Renderer::Render()
{
#if !MT
	uint32_t height = m_Image->GetHeight(), width = m_Image->GetWidth();
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			Vec3 coord((float)x, (float)y, 0.f);
			m_ImageData[x + y * width] = PerPixel(width, height, coord);
			//std::cout << "0x" << std::hex << m_ImageData[count] << '\n';
		}
	}

	m_Image->SetData(m_ImageData);
#else
	std::for_each(std::execution::par, m_ImageVerticalIterator.begin(), m_ImageVerticalIterator.end(),
		[this](uint32_t y)
		{
			std::for_each(std::execution::par, m_ImageHoritionalIterator.begin(), m_ImageHoritionalIterator.end(),
				[this, y](uint32_t x)
				{
					Vec3 coord((float)x, (float)y, 0.f);
					this->m_ImageData[x + y * m_Image->GetWidth()] = PerPixel(m_Image->GetWidth(), m_Image->GetHeight(), coord);
					//std::cout << "0x" << std::hex << m_ImageData[count] << '\n';
				});
		});

	m_Image->SetData(m_ImageData);
#endif
}

void Renderer::Render(uint32_t p_Width, uint32_t p_Height, std::fstream& p_PPMStream)
{
#if 1
	for (uint32_t y = 0; y < p_Height; y++)
	{
		for (uint32_t x = 0; x < p_Width; x++)
		{
			Vec3 coord((float)x, (float)y, 0.f);
			uint32_t color = PerPixel(p_Width, p_Height, coord);

			uint16_t blue = (uint16_t)((color >> 16) & 0xff);
			uint16_t green = (uint16_t)((color >> 8) & 0xff);
			uint16_t red = (uint16_t)(color & 0xff);

			//format RGB
			p_PPMStream << red << ' ' << green << ' ' << blue << '\n';
		}
	}
#else
	std::for_each(std::execution::par, 0, p_Height,
		[&](uint32_t height)
		{
			std::for_each(std::execution::par, 0, p_Width,
				[&](uint32_t width)
				{
					Vec3 coord((float)width, (float)height, 0.f);
					Vec3 color = PerPixel(p_Width, p_Height, coord);

					uint16_t red = (uint16_t)(color.x * 255.f);
					uint16_t green = (uint16_t)(color.y * 255.f);
					uint16_t blue = (uint16_t)(color.z * 255.f);

					p_ImageStream << red << ' ' << green << ' ' << blue << '\n';
				});
		});
#endif
}

void XRay::Renderer::SaveCurrentRender(std::fstream& p_PPMStream) const
{
	for (uint32_t i = m_Image->GetWidth() * m_Image->GetHeight(); i > 0; i--)
	{

		uint16_t blue = (uint16_t)((m_ImageData[i] >> 16) & 0xff);
		uint16_t green = (uint16_t)((m_ImageData[i] >> 8) & 0xff);
		uint16_t red = (uint16_t)((m_ImageData[i]) & 0xff);

		//format RGB
		p_PPMStream << red << ' ' << green << ' ' << blue << '\n';
	}
}

uint32_t Renderer::PerPixel(uint32_t p_Width, uint32_t p_Height, Vec3& p_PixelCoord)
{
	constexpr float diffusionRate = 0.5f;
	float aspectRatio = (float)p_Width / (float)p_Height;
	bool intersectsObject = false;

	Vec3 pixel((float)p_PixelCoord.x / (float)p_Width * 2.f - 1.f, 1.f - (float)p_PixelCoord.y / (float)p_Height * 2.f, 1.f);
	pixel.x *= aspectRatio;



	Vec3 rayDirection(pixel.x, pixel.y, pixel.z);
	Vec3::normalize(rayDirection);
	Vec3 rayOrigin = m_CameraPosition;

	Ray ray(rayDirection, rayOrigin);

	Vec3 color = TraceRay(ray, 0, 3).clamp(0.f, 1.f);

	return color.toABGR();
}

Vec3 XRay::Renderer::TraceRay(const Ray& p_Ray, size_t p_Iteration, size_t p_NumberOfBounces)
{
	constexpr float diffusionRate = 0.3f;
	std::vector<float> hitDistances;
	hitDistances.resize(m_CurrentScene->getNumOfObjects());
	Ray nextRay;
	Vec3 color;

	for (size_t i = 0; i < m_CurrentScene->getNumOfObjects(); i++)
	{
		hitDistances[i] = m_CurrentScene->getMutableObjectArray()->at(i).getIntersectionDistance(p_Ray);
	}

	uint32_t closestDistanceIndex = -1;
	float closestDistance = std::numeric_limits<float>::max();

	for (size_t i = 0; i < hitDistances.size(); i++)
	{
		if (hitDistances[i] < closestDistance && hitDistances[i] >= 0.f)
		{
			closestDistance = hitDistances[i];
			closestDistanceIndex = i;
		}
	}

	if (closestDistanceIndex == -1)
	{
		Vec3 planeNormal = Vec3(0.f, 1.f, 0.f);
		Vec3 planeOrigin = Vec3(0.f, m_PlaneBoundaries.y, 0.f);
		float t = Vec3::dot((planeOrigin - p_Ray.getOrigin()), planeNormal) / Vec3::dot(p_Ray.getDirection(), planeNormal);
		//printf("\n\tt: %.2f", t);
		if (t < 0)
			return m_BackgroundColor * std::powf(diffusionRate, p_Iteration);

		//glm::vec3 randomVec = Walnut::Random::Vec3(-0.5f, 0.5f);
		Vec3 newDirection = Vec3::reflect(p_Ray.getDirection(), planeNormal);
		Vec3 newOrigin = p_Ray.getDirection() * t + p_Ray.getOrigin() + planeNormal * 0.01f;
		nextRay = Ray(newDirection, newOrigin);
		color = m_BackgroundColor;
	}
	else
	{
		Vec3 sphereIntersection = p_Ray.getDirection() * hitDistances[closestDistanceIndex] + p_Ray.getOrigin();
		Vec3 sphereNormal = Vec3::normalize(sphereIntersection - m_CurrentScene->getMutableObjectArray()->at(closestDistanceIndex).getCenter());
		Vec3 newDirection = Vec3::reflect(p_Ray.getDirection(), sphereNormal);
		Vec3 newOrigin = sphereIntersection + sphereNormal * 0.01f;
		nextRay = Ray(newDirection, newOrigin);
		color = m_CurrentScene->getMutableObjectArray()->at(closestDistanceIndex).getIntersectionColor(
			m_LightDirection, sphereNormal, m_BackgroundColor, p_Ray);
	}

	if (p_Iteration < p_NumberOfBounces)
		color += TraceRay(nextRay, p_Iteration + 1, p_NumberOfBounces).clamp(0.f, 1.f) * diffusionRate;

	return color;
}

void XRay::Renderer::OnResize(uint32_t p_Width, uint32_t p_Height)
{

	//If image exists
	if (m_Image)
	{
		//If there is no need to resize
		if (m_Image->GetWidth() == p_Width && m_Image->GetHeight() == p_Height)
			return;

		m_Image->Resize(p_Width, p_Height);
	}
	else
	{
		m_Image = std::make_shared<Walnut::Image>(p_Width, p_Height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[p_Width * p_Height];

	m_ImageHoritionalIterator.resize(p_Width);
	m_ImageVerticalIterator.resize(p_Height);

	for (uint32_t i = 0; i < p_Width; i++)
		m_ImageHoritionalIterator[i] = i;
	for (uint32_t i = 0; i < p_Height; i++)
		m_ImageVerticalIterator[i] = i;
}

void Renderer::setScene(std::shared_ptr<Scene> p_ScenePtr)
{
	m_CurrentScene = p_ScenePtr;
}

void XRay::Renderer::setBoundaries(const Vec3& p_Boundaries)
{
	m_PlaneBoundaries = p_Boundaries;
}

const std::shared_ptr<Scene> XRay::Renderer::getScene() const
{
	return m_CurrentScene;
}


