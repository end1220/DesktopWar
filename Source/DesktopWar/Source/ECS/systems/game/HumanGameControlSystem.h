
#pragma once

#include "../../core/EntityProcessingSystem.h"
#include "../../core/ComponentMapper.h"
#include "../../components/common/PositionCom.h"
#include "../../components/common/TeamCom.h"
#include "../../components/GamePlayCom.h"

namespace Genius
{
	class HumanGameControlSystem : public EntityProcessingSystem
	{
	private:
		ComponentMapper<HumanGameControlCom>	gameControlMapper;
		ComponentMapper<TeamCom>	teamMapper;
		ComponentMapper<PositionCom>	positionMapper;

	public:
		HumanGameControlSystem()
		{
			SetComponentTypes<HumanGameControlCom, TeamCom, PositionCom>();
		}
		virtual const char* GetName(){ return "HumanGameControlSystem"; }
		virtual void Initialize();
		virtual void ProcessEntity(Entity* e);

	private:
		Entity* SpawnOne(float x, float y);
	};
};