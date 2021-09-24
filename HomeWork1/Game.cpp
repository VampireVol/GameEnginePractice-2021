#include "Game.h"

Game::Game() :
	m_pRenderEngine(nullptr)
{
	m_pRenderEngine = new RenderEngine();

	for (int i = 0; i < 5; ++i)
	{
		m_Planets.push_back(Planet((i + 1) * 100));
	}

	m_Timer.Start();
}

Game::~Game()
{
}

void Game::Run()
{
	m_Timer.Reset();

	while (true)
	{
		m_pRenderEngine->GetRT()->RC_BeginFrame();

		m_Timer.Tick();

		if (!Update())
			break;

		m_pRenderEngine->GetRT()->RC_EndFrame();
	}
}

bool Game::Update()
{
	static float t = 0;
	float deltaTime = m_Timer.DeltaTime();
	// t += m_Timer.DeltaTime();
	t += 0.001f;
	m_pRenderEngine->GetRT()->RC_OscillateCamera(10.0f + sin(t));
	for (int i = 0; i < m_Planets.size(); ++i)
	{
		m_Planets[i].Update(deltaTime);
		m_pRenderEngine->GetRT()->RC_MovePlanet(i, m_Planets[i].GetPosition());
	}

	return true;
}
