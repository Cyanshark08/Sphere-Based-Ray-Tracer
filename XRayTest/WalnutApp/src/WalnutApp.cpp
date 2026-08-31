#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include "Scene.h"

#include <iomanip>
#include <fstream>

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{

		ImGui::Begin("Settings");

		ImGui::Text("Last Render: %.3fms", m_LastRenderTime);
		ImGui::Text("Camera X-Pos: %.3fms", *m_Renderer.getMutableCameraX());
		ImGui::Text("Camera Y-Pos: %.3fms", *m_Renderer.getMutableCameraY());
		ImGui::Text("Camera Z-Pos: %.3fms", *m_Renderer.getMutableCameraZ());
		ImGui::SliderFloat("Camera Speed", &m_CameraSpeed, 0.01f, 10.f);

		ImGui::Text("Background Color Sliders");

		if (ImGui::SliderFloat("Red Channel (Background)", m_Renderer.getMutableBackgroundR(), 0.f, 1.0f))
			++m_AutoSaveActionCount;
		if (ImGui::SliderFloat("Green Channel (Background)", m_Renderer.getMutableBackgroundG(), 0.f, 1.0f))
			++m_AutoSaveActionCount;
		if (ImGui::SliderFloat("Blue Channel (Background)", m_Renderer.getMutableBackgroundB(), 0.f, 1.0f))
			++m_AutoSaveActionCount;

		ImGui::Text("Light Direction Sliders");

		if (ImGui::SliderFloat("Light Direction (x)", &(m_Renderer.getMutableLight()->x), -1.f, 1.0f))
			++m_AutoSaveActionCount;
		if (ImGui::SliderFloat("Light Direcction (y)", &(m_Renderer.getMutableLight()->y), -1.f, 1.0f))
			++m_AutoSaveActionCount;
		if (ImGui::SliderFloat("Light Direction (z)", &(m_Renderer.getMutableLight()->z), -1.f, 1.0f))
			++m_AutoSaveActionCount;

		float timeDelta = 1.f / m_LastRenderTime;

		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_W))
			(*m_Renderer.getMutableCameraZ()) += (timeDelta * m_CameraSpeed);
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_S))
			(*m_Renderer.getMutableCameraZ()) -= (timeDelta * m_CameraSpeed);
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_A))
			(*m_Renderer.getMutableCameraX()) -= (timeDelta * m_CameraSpeed);
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_D))
			(*m_Renderer.getMutableCameraX()) += (timeDelta * m_CameraSpeed);
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_Q))
			(*m_Renderer.getMutableCameraY()) -= (timeDelta * m_CameraSpeed);
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_E))
			(*m_Renderer.getMutableCameraY()) += (timeDelta * m_CameraSpeed);

		if (ImGui::Button("Origin"))
		{
			*m_Renderer.getMutableCameraX() = 0.f;
			*m_Renderer.getMutableCameraY() = 0.f;
			*m_Renderer.getMutableCameraZ() = -2.f;
		}

		ImGui::End();

		ImGui::Begin("Objects");

			if (m_CurrentScene)
			{
				if (ImGui::Button("Add Sphere"))
					m_CurrentScene->pushObject(XRay::RigidSphere());
				for (size_t i = 0; i < m_CurrentScene->getNumOfObjects(); i++)
				{
					std::string label = "Channel-";
					ImGui::Text("Object %d", i);

					if (ImGui::SliderFloat((label + "R" + std::to_string(i + 1)).c_str(), &m_CurrentScene->getMutableObjectArray()->at(i).getMutableColor()->x, 0.f, 1.0f))
						++m_AutoSaveActionCount;
					if (ImGui::SliderFloat((label + "G" + std::to_string(i + 1)).c_str(), &m_CurrentScene->getMutableObjectArray()->at(i).getMutableColor()->y, 0.f, 1.0f))
						++m_AutoSaveActionCount;
					if (ImGui::SliderFloat((label + "B" + std::to_string(i + 1)).c_str(), & m_CurrentScene->getMutableObjectArray()->at(i).getMutableColor()->z, 0.f, 1.0f))
						++m_AutoSaveActionCount;

					label = "Origin-";
					if (ImGui::SliderFloat((label + "X" + std::to_string(i + 1)).c_str(), &m_CurrentScene->getMutableObjectArray()->at(i).getMutableCenter()->x, -10.f, 10.f))
						++m_AutoSaveActionCount;
					if (ImGui::SliderFloat((label + "Y" + std::to_string(i + 1)).c_str(), &m_CurrentScene->getMutableObjectArray()->at(i).getMutableCenter()->y, -10.f, 10.f))
						++m_AutoSaveActionCount;
					if (ImGui::SliderFloat((label + "Z" + std::to_string(i + 1)).c_str(), &m_CurrentScene->getMutableObjectArray()->at(i).getMutableCenter()->z, -10.f, 10.f))
						++m_AutoSaveActionCount;

					label = "Radius";
					ImGui::SliderFloat((label + std::to_string(i)).c_str(), m_CurrentScene->getMutableObjectArray()->at(i).getMutableRadius(), 0.f, 10.f);

					label = "Elasticity";
					if (ImGui::SliderFloat((label + std::to_string(i + 1)).c_str(), m_CurrentScene->getMutableObjectArray()->at(i).GetMutableElasticity(), 0.f, 1.f))
						++m_AutoSaveActionCount;
				}

				if (ImGui::SliderFloat("Time Multiplier", &m_TimeMultiplier, 0.f, 1.f))
					++m_AutoSaveActionCount;

			}

		ImGui::End();

		ImGui::Begin("Viewport");

		Initialize();

		m_ViewportWidth = (uint32_t)ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

		auto finalImage = m_Renderer.GetFinalImage();
		if (finalImage)
			ImGui::Image(finalImage->GetDescriptorSet(), { (float)finalImage->GetWidth(), (float)finalImage->GetHeight() });

		ImGui::End();

		ImGui::Begin("Misc");

		//Take Screenshot
		if (ImGui::Button("Screenshot") && finalImage)
			this->Screenshot();

		if (ImGui::Button("High-Quality Screenshot") && finalImage)
			this->SaveHighQualityScreenshot();

		if (ImGui::Button("Save Scene") && m_CurrentScene)
			SaveCurrentScene();

		if (ImGui::Button("Save Scene As") && m_CurrentScene)
			SaveCurrentSceneAs();

		if (ImGui::Button("Load Scene") && m_CurrentScene)
			LoadScene();

		if (ImGui::Button("New Scene") && m_CurrentScene)
		{
			SaveCurrentScene();
			m_CurrentScene->clearScene();
			m_CurrentFileOpen = "unnamed";
		}

		if ((m_AutoSaveTimer.ElapsedMillis() > 20000.f && m_AutoSaveActionCount > 10) || m_AutoSaveTimer.ElapsedMillis() > 120000.f)
		{
			std::cout << "\n\tAuto-saving...";
			AutoSaveScene();
			std::cout << "\n\tTime: " << std::setprecision(3) << m_AutoSaveTimer.Elapsed();
			std::cout << "\n\tActions: " << m_AutoSaveActionCount;
			m_AutoSaveTimer.Reset();
		}

		ImGui::End();

		if (m_Physics)
		{
			XRay::RigidSphere::s_UpdatePhysics(*(m_CurrentScene->getMutableObjectArray()), m_PlaneBoundaries, m_LastRenderTime * 1E-3f * m_TimeMultiplier);
			//printf("\nElapsed Time: %.2f", m_LastRenderTime * 1E-3f);
		}

		this->Render();
	}

	void Render()
	{
		Walnut::Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render();

		m_LastRenderTime = timer.ElapsedMillis();
	}

	void Screenshot()
	{
		std::fstream PPMStream;

		PPMStream.open("Screenshot.ppm", std::ios::out);

		PPMStream << "P3\n";
		PPMStream << m_ViewportWidth << ' ' << m_ViewportHeight;
		PPMStream << "\n255\n";

		m_Renderer.SaveCurrentRender(PPMStream);

		PPMStream.close();
	}

	void SaveHighQualityScreenshot()
	{
		std::fstream PPMStream;

		PPMStream.open("HQScreenshot.ppm", std::ios::out);

		PPMStream << "P3\n";
		PPMStream << 4096 << ' ' << 2160;
		PPMStream << "\n255\n";

		m_Renderer.Render(4096, 2160, PPMStream);

		PPMStream.close();
	}

	void SaveCurrentScene()
	{
		if (m_CurrentFileOpen == "unnamed")
		{
			SaveCurrentSceneAs();
			return;
		}

		m_CurrentScene->writeToFile(m_CurrentFileOpen);
		AutoSaveScene();
	}

	void SaveCurrentSceneAs()
	{
		std::string fileName;

		std::cout << "\n\tInput the name of a file to save to: ";
		std::getline(std::cin, fileName);

		if (m_CurrentScene->writeToFile(fileName))
			m_CurrentFileOpen = fileName;

		AutoSaveScene();
		UpdateFileNameTXT();
	}

	void LoadScene()
	{
		std::string fileName;

		std::cout << "\n\tInput the name of a file to load from: ";
		std::getline(std::cin, fileName);

		if (m_CurrentScene->loadFromFile(fileName))
			m_CurrentFileOpen = fileName;

		UpdateFileNameTXT();
	}

	void UpdateFileNameTXT()
	{
		std::fstream fileStream;
		fileStream.open("LastFileOpened.txt", std::ios::out | std::ios::trunc);
		for (const char fileNameCharacter : m_CurrentFileOpen)
			fileStream << fileNameCharacter;

		fileStream.close();
	}

	bool AutoSaveScene()
	{
		std::cout << "\n\tCurrent file: " << m_CurrentFileOpen;
		return m_CurrentScene->writeToFile("LastAutoSave");
	}

	void LoadLastAutoSave()
	{
		if (!m_CurrentScene->loadFromFile("LastAutoSave"))
			throw e_NoAutoSave();
	}

	void Initialize()
	{
		if (!m_CurrentScene)
		{
			m_CurrentScene = std::make_shared<XRay::Scene>();

			try
			{
				LoadLastAutoSave();

				std::fstream fileStream;
				fileStream.open("LastFileOpened.txt", std::ios::in);
				std::getline(fileStream, m_CurrentFileOpen);

				fileStream.close();

			}
			catch (e_NoAutoSave)
			{
				std::cout << "\n\tNo auto-save found!";
			}

			m_Renderer.setScene(m_CurrentScene);
			m_Renderer.setBoundaries(m_PlaneBoundaries);
		}
	}

public:
	//exception class
	class e_NoAutoSave {};

private:

	std::shared_ptr<XRay::Scene> m_CurrentScene;
	XRay::Renderer m_Renderer;
	uint32_t m_ViewportWidth = 0u, m_ViewportHeight = 0u;
	std::string m_CurrentFileOpen = "unnamed";

	float m_LastRenderTime = 0.f;
	float m_CameraSpeed = 1.f;

	Walnut::Timer m_AutoSaveTimer;
	uint16_t m_AutoSaveActionCount = 0u;

	XRay::Vec3 m_PlaneBoundaries = XRay::Vec3(5.f, -2.f, 5.f);
	bool m_Physics = true;
	float m_TimeMultiplier = 0.f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "XRay";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					app->Close();
				}
				ImGui::EndMenu();
			}
		});
	return app;
}