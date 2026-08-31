#pragma once
#include <stdint.h>
#include <memory>
#include <vector>
#include <fstream>
#include <memory>

#include "Vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Walnut/Image.h"

namespace XRay
{
	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(std::shared_ptr<Scene> p_CurrentScene);
		void Render();
		void Render(uint32_t p_Width, uint32_t p_Height, std::fstream& p_PPMStream);

		void SaveCurrentRender(std::fstream& p_PPMStream) const;

		void OnResize(uint32_t p_Width, uint32_t p_Height);
		void setScene(std::shared_ptr<Scene> p_ScenePtr);
		void setBoundaries(const Vec3& p_Boundaries);

		const std::shared_ptr<Scene> getScene() const;

		float* getMutableBackgroundB() { return &m_BackgroundColor.x; }
		float* getMutableBackgroundG() { return &m_BackgroundColor.y; }
		float* getMutableBackgroundR() { return &m_BackgroundColor.z; }

		float* getMutableCameraX() { return &m_CameraPosition.x; }
		float* getMutableCameraY() { return &m_CameraPosition.y; }
		float* getMutableCameraZ() { return &m_CameraPosition.z; }

		Vec3* getMutableLight() { return &m_LightDirection; }

		std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_Image; }

	private:
		uint32_t PerPixel(uint32_t p_Width, uint32_t p_Height, Vec3& p_PixelCoord);
		Vec3 TraceRay(const Ray& p_Ray, size_t p_Iteration, size_t p_NumberOfBounces);

	private:
		//Vec3 m_BaseColor = Vec3(0.f, 1.f, 1.f);

		std::shared_ptr<Scene> m_CurrentScene;
		Vec3 m_BackgroundColor = Vec3(0.5f, 0.5f, 0.5f);
		Vec3 m_LightDirection = Vec3(-1.f, -1.f, 1.f);
		Vec3 m_CameraPosition = Vec3(0.f, 0.f, -2.f);
		std::shared_ptr<Walnut::Image> m_Image;
		uint32_t* m_ImageData = nullptr;
		Vec3 m_PlaneBoundaries;

		std::vector<uint32_t> m_ImageHoritionalIterator, m_ImageVerticalIterator;
	};
}


