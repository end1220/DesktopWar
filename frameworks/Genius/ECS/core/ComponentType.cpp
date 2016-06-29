
#include "ComponentType.h"
namespace Genius
{

	ComponentType::ComponentType()
	{
		init();
	}

	void ComponentType::init()
	{
		bit = nextBit;
		nextBit = nextBit << 1;
		id = nextId++;
	}

	std::bitset<BITSIZE> ComponentType::getBit() const
	{
		return bit;
	}

	int ComponentType::GetId() const
	{
		return id;
	}

	std::bitset<BITSIZE> ComponentType::nextBit(1);
	int ComponentType::nextId = 1;
};