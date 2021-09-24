#pragma once
#include "Ogre.h"
class Planet
{
public:
	Planet(float radius);
	void Update(float deltaTime);
	Ogre::Vector3 GetPosition();

private:
	float m_fRadius;
	Ogre::Vector3 m_Position;
	Ogre::Vector3 m_Velocity;
	//Ogre::Quaternion 
};

