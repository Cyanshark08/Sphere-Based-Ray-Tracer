#pragma once
#include "Sphere.h"
#include "RigidSphere.h"
#include <vector>
#include <string>

namespace XRay {

	class Scene
	{
	public:
		Scene() = default;
		Scene(const Scene& p_Other);

		uint32_t getNumOfObjects() const;

		const std::vector<RigidSphere>& getObjectArray() const;
		std::vector<RigidSphere>* getMutableObjectArray();

		const std::vector<RigidSphere>& pushObject(const RigidSphere& p_Object);
		const std::vector<RigidSphere>& popObject();
		const std::vector<RigidSphere>& deleteSphereAt(uint32_t p_Index);
		void clearScene();

		bool writeToFile(const std::string& p_FileName);
		bool loadFromFile(const std::string& p_FileName);
	private:
		std::vector<RigidSphere> m_Objects;

	};

}
