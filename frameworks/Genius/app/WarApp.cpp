

#include <string>

#include "common/Log.h"
#include "common/Performance.h"
#include "common/TimeSystem.h"
#include "app/Config.h"
#include "WarApp.h"
#include "ConfigPool.h"
#include "gfx/gfx.h"
#include "gfx/CocosApp.h"
#include "pawn/bevtree/BehaviorsAndPreconditions.h"
#include "ECS/ecs.h"
#include "entity/EntityCreators.h"
#include "skill/SkillSystem.h"
#include "skill/BuffSystem.h"
#include "GamePlay.h"
#include "scripting/js-bindings/manual/ScriptingCore.h"


CocosApp cocosApp;

USING_NS_CC;
using namespace Genius;


bool WarApp::Init()
{
	do
	{
		cocosApp.PostRun();

		if (cfg_EnableConsole)
		{
			AllocConsole();
			freopen("CONIN$", "r", stdin);
			freopen("CONOUT$", "w", stdout);
			freopen("CONOUT$", "w", stderr);
		}

		Log::Info("WarApp::Init Begin...");

		Performance::Begin();

		TimeSystem::GetSingleton()->Init();
		cfg::ConfigPool::GetSingleton()->Init();
		SceneManager::GetSingleton()->Init();
		SystemStatusPanel::GetSingleton()->Init();
		AI::BHTree::BehaviorLoader::GetSingleton()->Init(gBehaviorMap, gPreconditionMap);
		ECSWorld::GetSingleton()->GetSystemManager()->Init();
		//GamePlay::GetSingleton().Init();

		cocosApp.RunScript();

		float dt = Performance::End();
		Log::Info("WarApp::Init End. Cost : %f s", dt);

		/*m_pLaunchState = new AppStateLaunch(this);
		m_pLoadingState = new AppStateLoading(this);
		m_pBattleState = new AppStateBattle(this);
		m_pCurrentState = nullptr;
		m_pLastState = nullptr;
		SwitchState(AppState::eLaunch);*/

		// test
		//int width = GetSystemMetrics(SM_CXSCREEN);
		//EntityCreator::CreateBornPoint(50, 133, Team_Monster);
		//EntityCreator::CreateBornPoint(width - 50, 133, Team_Human);

		//EntityCreator::CreatePawn("Militia", 100, 133, Team_Human);		EntityCreator::CreatePawn("Goblin", 240, 133, Team_Monster);
		//EntityCreator::CreatePawn("Footman", 200, 133, Team_Human);	EntityCreator::CreatePawn("Goblin", 340, 133, Team_Monster);
		//EntityCreator::CreatePawn("Knight", 300, 133, Team_Human);		EntityCreator::CreatePawn("Goblin", 440, 133, Team_Monster);
		//EntityCreator::CreatePawn("HolyKnight", 400, 133, Team_Human);	EntityCreator::CreatePawn("Barbarian", 540, 133, Team_Monster);
		//EntityCreator::CreatePawn("Barbarian", 500, 133, Team_Human);		EntityCreator::CreatePawn("Goblin", 640, 133, Team_Monster);

	} while (0);

	return true;
}

void WarApp::Tick(float time)
{
	float tickSec = time;
	TimeSystem::GetSingleton()->Tick(tickSec);
	SystemStatusPanel::GetSingleton()->ClearStatus();
	
	ECSWorld::GetSingleton()->LoopStart();
	ECSWorld::GetSingleton()->SetDeltaTime(tickSec);
	ECSWorld::GetSingleton()->Process();

	SkillSystem::GetSingleton().Update(tickSec);
	BuffSystem::GetSingleton().Update(tickSec);

	SceneManager::GetSingleton()->Update();

	cocosApp.StepRun();

	_Update_Fast(tickSec);
	_Update_Normal(tickSec);
	_Update_Slow(tickSec);

	/*if (m_pCurrentState)
	{
		if (m_pCurrentState != m_pLastState)
		{
			if (m_pLastState)
				m_pLastState->Destroy();
			
			m_pCurrentState->Init();
			m_pLastState = m_pCurrentState;
		}
		m_pCurrentState->Update(tickSeconds);
	}*/
}

void WarApp::Destroy()
{
	/*m_pLaunchState->Destroy();
	delete m_pLaunchState;
	m_pLoadingState->Destroy();
	delete m_pLoadingState;
	m_pBattleState->Destroy();
	delete m_pBattleState;*/

	Genius::EntityManager * em = ECSWorld::GetSingleton()->GetEntityManager();
	em->RemoveAllEntities();

	AI::BHTree::BehaviorLoader::GetSingleton()->Destroy();
	cfg::ConfigPool::GetSingleton()->Destroy();

	cocosApp.AfterRun();
	//GamePlay::GetSingleton().Destroy();

	if (cfg_EnableConsole)
	{
		FreeConsole();
	}
}

void WarApp::_Update_Fast(float time)
{

}

void WarApp::_Update_Normal(float time)
{
	static float timePassed = 0;
	timePassed += time;
	if (timePassed < 0.2f)
		return;

	ScriptingCore::getInstance()->evalString("Game.OnUpdate(0.2)");
	
	// set zero
	timePassed = 0;
}

void WarApp::_Update_Slow(float time)
{
	static float timePassed = 0;
	timePassed += time;
	if (timePassed < 1.0f)
		return;

	//ScriptingCore::getInstance()->evalString("Game.OnUpdate(1.0)");

	// set zero
	timePassed = 0;
}

/*void WarApp::SwitchState(int state)
{
	if (state < AppState::eLaunch || state > AppState::eBattle)
		return;

	m_pLastState = m_pCurrentState;
	m_pCurrentState = m_pLoadingState;
	AppState* pNextState = nullptr;
	switch (state)
	{
	case AppState::eLaunch:
		pNextState = m_pLaunchState;
		break;
	case AppState::eLoading:
		pNextState = m_pLoadingState;
		break;
	case AppState::eBattle:
		pNextState = m_pBattleState;
		break;
	}
	m_pLoadingState->StartLoading(m_pLastState, pNextState);
}

void WarApp::SetCurrentState(AppState* state)
{
	m_pLastState = m_pCurrentState;
	m_pCurrentState = state;
}*/