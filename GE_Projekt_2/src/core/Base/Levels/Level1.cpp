#include "Level1.hpp"

namespace trr
{

	Level1::Level1()
	{
	}

	Level1::~Level1()
	{
	}

	void Level1::Init()
	{
		





	}

	void Level1::Unload()
	{
		int size = m_entities.size();
		for (int i = 0; i < size; i++) {
			trr::entityHandler.RemoveEntity( m_entities[ i ] );
		}
		m_entities.clear();
	}
}