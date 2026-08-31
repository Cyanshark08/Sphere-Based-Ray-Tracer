#include "Scene.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace XRay;

XRay::Scene::Scene(const Scene& p_Other)
{
	this->m_Objects.reserve(p_Other.m_Objects.size());

	for (const RigidSphere& sphere : p_Other.m_Objects)
		this->m_Objects.push_back(sphere);

}

uint32_t XRay::Scene::getNumOfObjects() const
{
	return m_Objects.size();
}

const std::vector<RigidSphere>& XRay::Scene::getObjectArray() const
{
	return m_Objects;
}

std::vector<RigidSphere>* XRay::Scene::getMutableObjectArray()
{
	return &m_Objects;
}

const std::vector<RigidSphere>& XRay::Scene::pushObject(const RigidSphere& p_Object)
{
	m_Objects.push_back(p_Object);
	return m_Objects;
}

const std::vector<RigidSphere>& XRay::Scene::popObject()
{
	if (m_Objects.size() > 0)
		m_Objects.pop_back();

	return m_Objects;
}

const std::vector<RigidSphere>& XRay::Scene::deleteSphereAt(uint32_t p_Index)
{
	std::vector<RigidSphere>temp;
	temp.reserve(m_Objects.size() - 1);
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (i == p_Index)
			continue;

		temp.push_back(m_Objects[i]);
	}

	m_Objects.clear();
	m_Objects = temp;
}

void XRay::Scene::clearScene()
{
	m_Objects.clear();
}

bool XRay::Scene::writeToFile(const std::string& p_FileName)
{
	std::fstream fileStream;

	fileStream.open(p_FileName + ".sce", std::ios::binary | std::ios::out | std::ios::trunc);

	if (fileStream.fail())
	{
		std::cout << "\nERROR: File," + p_FileName + ", save failed!";
		return false;
	}

	for(RigidSphere& obj : m_Objects)
		fileStream.write(reinterpret_cast<char*>(&obj), sizeof(RigidSphere));

	fileStream.close();

	std::cout << "\n\tFile ," + p_FileName + ", save success!";
	return true;
}

bool XRay::Scene::loadFromFile(const std::string& p_FileName)
{
	std::fstream fileStream;

	fileStream.open(p_FileName + ".sce", std::ios::binary | std::ios::in);

	if (fileStream.fail())
	{
		std::cout << "\nERROR: File," + p_FileName + ", load failed!";
		return false;
	}

	fileStream.seekg(0L, std::ios::end);
	uint32_t numOfObj = fileStream.tellg() / sizeof(RigidSphere);

	fileStream.seekg(0L, std::ios::beg);

	clearScene();
	RigidSphere tempSphere;

	for (size_t i = 0; i < numOfObj; i++)
	{
		fileStream.read(reinterpret_cast<char*>(&tempSphere), sizeof(RigidSphere));
		this->pushObject(tempSphere);
	}

	fileStream.close();

	std::cout << "\n\tFile," + p_FileName + ", load success!";
	return true;
}


