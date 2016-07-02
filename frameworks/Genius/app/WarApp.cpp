

#include <string>
#include <io.h>
#include <fcntl.h>
#include "common/Logger.h"
#include "common/Performance.h"
#include "common/TimeSystem.h"
#include "app/Config.h"
#include "WarApp.h"
#include "DBSystem.h"
#include "gfx/gfx.h"
#include "gfx/CocosApp.h"
#include "pawn/bevtree/BehaviorsAndPreconditions.h"
#include "ECS/ecs.h"
#include "entity/EntityCreators.h"
#include "skill/SkillSystem.h"
#include "skill/BuffSystem.h"
#include "GamePlay.h"
#include "AppStatus/AppStateLaunch.h"
#include "AppStatus/AppStateLoading.h"
#include "AppStatus/AppStateBattle.h"


CocosApp cocosApp;

USING_NS_CC;
using namespace Genius;


bool WarApp::Init()
{
	do
	{
		srand((unsigned)time(nullptr));

		cocosApp.PostRun();

		OpenConsole();

		Logger::LogInfo("WarApp::Init Begin...");

		Performance::Begin();

		TimeSystem::GetInstance().Init();
		DBSystem::GetSingleton().Init();
		SceneManager::GetInstance().Init();
		SystemStatusPanel::GetSingleton()->Init();
		AI::BHTree::BehaviorLoader::GetInstance().Init(gBehaviorMap, gPreconditionMap);
		ECSWorld::GetSingleton()->GetSystemManager()->Init();
		//GamePlay::GetSingleton().Init();

		cocosApp.RunScript();

		float dt = Performance::End();
		Logger::LogInfo("WarApp::Init End. Cost : %f s", dt);

		m_pLaunchState = new AppStateLaunch(this);
		m_pLoadingState = new AppStateLoading(this);
		m_pBattleState = new AppStateBattle(this);
		m_pCurrentState = nullptr;
		m_pLastState = nullptr;
		SwitchState(AppState::eLaunch);

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
	float tickSeconds = time;
	TimeSystem::GetInstance().Tick(tickSeconds);
	SystemStatusPanel::GetSingleton()->ClearStatus();
	
	ECSWorld::GetSingleton()->LoopStart();
	ECSWorld::GetSingleton()->SetDeltaTime(tickSeconds);
	ECSWorld::GetSingleton()->Process();

	SkillSystem::GetSingleton().Update(tickSeconds);
	BuffSystem::GetSingleton().Update(tickSeconds);

	SceneManager::GetInstance().Update();

	cocosApp.StepRun();

	if (m_pCurrentState)
	{
		if (m_pCurrentState != m_pLastState)
		{
			if (m_pLastState)
				m_pLastState->Destroy();
			
			m_pCurrentState->Init();
			m_pLastState = m_pCurrentState;
		}
		m_pCurrentState->Update(tickSeconds);
	}
}

void WarApp::Destroy()
{
	m_pLaunchState->Destroy();
	delete m_pLaunchState;
	m_pLoadingState->Destroy();
	delete m_pLoadingState;
	m_pBattleState->Destroy();
	delete m_pBattleState;

	Genius::EntityManager * em = ECSWorld::GetSingleton()->GetEntityManager();
	em->RemoveAllEntities();

	AI::BHTree::BehaviorLoader::GetInstance().Destroy();
	DBSystem::GetSingleton().Destroy();

	cocosApp.AfterRun();
	//GamePlay::GetSingleton().Destroy();

	if (cfg_EnableConsole)
	{
		fclose(stdout);
		//FreeConsole();
	}
}

void WarApp::OpenConsole()
{
	if (!cfg_EnableConsole)
		return;
	AllocConsole();
	int nCrt = int(_open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT));
	FILE* pStdout = _fdopen(nCrt, "w");
	*stdout = *pStdout;
	setvbuf(stdout, NULL, _IONBF, 0);
}

void WarApp::SwitchState(int state)
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
}