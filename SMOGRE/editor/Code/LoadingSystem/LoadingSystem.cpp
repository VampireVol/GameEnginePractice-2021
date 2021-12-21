#include "LoadingSystem.h"
#include "../EntityManager.h"
#include "OgreSceneManager.h"
#include "tinyxml.h"
#include <regex>

LoadingSystem::LoadingSystem(EntityManager* pEntityManager, const std::string& scriptsRoot) :
	m_pEntityManager(pEntityManager),
	m_strSavesRootPath(scriptsRoot)
{

}

void LoadingSystem::LoadFromXML(const std::string fileName)
{
	const auto pathName = m_strSavesRootPath + fileName;

	TiXmlDocument doc(pathName.c_str());

	if (doc.LoadFile())
	{
		const auto elem = doc.FirstChildElement("scene");
		for (TiXmlElement* e = elem->FirstChildElement("character"); e != nullptr; e = e->NextSiblingElement("character"))
		{
			EntityInfo currentCharacter;
			currentCharacter.meshName = e->Attribute("meshName");
			currentCharacter.scriptName = e->Attribute("scriptName");
			currentCharacter.position = ParsePosition(e->Attribute("position"));
			currentCharacter.scale = ParsePosition(e->Attribute("scale"));

			m_pEntityManager->CreateEntity(currentCharacter);
		}
	}
}

void LoadingSystem::SaveToXML(const std::string fileName)
{
	const auto pathName = m_strSavesRootPath + fileName;
	TiXmlDocument doc(pathName.c_str());
	TiXmlElement* decl = new TiXmlElement("scene");

	std::time_t result = std::time(nullptr);
	decl->SetAttribute("name", fileName.c_str());

	const auto elem = doc.LinkEndChild(decl);
	auto entityQueue = m_pEntityManager->GetEntityQueue();
	TiXmlElement* e;
	for (auto& entity : entityQueue)
	{
		e = new TiXmlElement("character");
		elem->LinkEndChild(e);
		std::string scriptName = entity.second.pScriptNode->m_strScriptPath.c_str();
		std::size_t found = scriptName.find_last_of("\\");
		e->SetAttribute("scriptName", scriptName.substr(found + 1).c_str());
		e->SetAttribute("meshName", entity.second.pRenderNode->GetMeshName().c_str());
		std::string res = { std::to_string(entity.second.pRenderNode->GetPosition().x) + ","
							+ std::to_string(entity.second.pRenderNode->GetPosition().y) + ","
							+ std::to_string(entity.second.pRenderNode->GetPosition().z) };
		e->SetAttribute("position", res.c_str());
		res = { std::to_string(entity.second.pRenderNode->GetScale().x) + ","
							+ std::to_string(entity.second.pRenderNode->GetScale().y) + ","
							+ std::to_string(entity.second.pRenderNode->GetScale().z) };
		e->SetAttribute("scale", res.c_str());
	}

	doc.SaveFile();
}

LoadingSystem::~LoadingSystem()
{

};

Ogre::Vector3 LoadingSystem::ParsePosition(const char* strPosition)
{
	std::regex regex("[+-]?([0-9]*[.])?[0-9]+");
	std::cmatch match;

	Ogre::Vector3 vPosition;

	std::regex_search(strPosition, match, regex);
	vPosition.x = std::stof(match[0]);

	strPosition = match.suffix().first;
	std::regex_search(strPosition, match, regex);
	vPosition.y = std::stof(match[0]);

	strPosition = match.suffix().first;
	std::regex_search(strPosition, match, regex);
	vPosition.z = std::stof(match[0]);

	return vPosition;
}