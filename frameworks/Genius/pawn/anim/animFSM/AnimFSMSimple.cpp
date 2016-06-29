
#include "AnimFSMSimple.h"
#include "../animState/AnimStateIdle.h"
#include "../animState/AnimStateDie.h"
#include "../animState/AnimStateMove.h"
#include "../animState/AnimStateAttackNear.h"
#include "../animState/AnimStateAttackFar.h"
#include "pawn/action/ActionDefine.h"
#include "pawn/action/PawnAction.h"

using namespace Genius;

AnimFSMSimple::AnimFSMSimple(ComPawnAnim* pComPawnAnim) :
AnimFSM(pComPawnAnim)
{
	
}

AnimFSMSimple::~AnimFSMSimple()
{

}

void AnimFSMSimple::Initialize()
{
	Release();

	m_animStateList.insert(std::make_pair(PAT_Idle, new AnimStateIdle(m_pComPawnAnim)));
	m_animStateList.insert(std::make_pair(PAT_Die, new AnimStateDie(m_pComPawnAnim)));
	m_animStateList.insert(std::make_pair(PAT_Move, new AnimStateMove(m_pComPawnAnim)));
	m_animStateList.insert(std::make_pair(PAT_AttackNear, new AnimStateAttackNear(m_pComPawnAnim)));
	m_animStateList.insert(std::make_pair(PAT_AttackFar, new AnimStateAttackFar(m_pComPawnAnim)));

	AnimFSM::Initialize();
}

void AnimFSMSimple::DoAction(PawnAction* pAction)
{
	int actType = pAction->GetType();

	if (actType == PAT_ChangeDir)
	{
		m_pCurrentState->OnActive();
	}
	else if (m_pCurrentState->HandleNewAction(pAction))
	{
		m_pNextState = nullptr;
	}
	else
	{
		AnimState* pNewState = nullptr;

		switch (actType)
		{
		case PAT_Idle:
			pNewState = m_animStateList[actType];
			break;
		case PAT_Die:
			pNewState = m_animStateList[actType];
			break;
		case PAT_Move:
			pNewState = m_animStateList[actType];
			break;
		case PAT_AttackNear:
			pNewState = m_animStateList[actType];
			break;
		case PAT_AttackFar:
			pNewState = m_animStateList[actType];
			break;
		case PAT_Skill1:
			pNewState = m_animStateList[actType];
			break;
		case PAT_Skill2:
			break;
		case PAT_Skill3:
			break;
		default:
			break;
		}

		if (pNewState != nullptr)
		{
			m_pNextState = pNewState;

			SwitchToNextState(pAction);
		}

	}
}