#ifndef TRR_ENTITY_STRUCTS_DEFINITIONS_HPP
#define TRR_ENTITY_STRUCTS_DEFINITIONS_HPP

#include <Core/EntityHandler/Entity.hpp>

#define TRR_NO_PARENT						-1
#define TRR_NO_CHILDREN						-1

namespace trr
{
	/*
		Used for soft mapping of entites when identity is of concern.
	*/
	struct EntityLink
	{
		EntityLink( int ID, int index_ )
		{
			id = ID;
			index = index_;
		}

		Entity id;
		unsigned int index;
	};

	/*
		Used for scene graph implementation
	*/
	struct ScenGraphInfo
	{
		ScenGraphInfo( int index )
			: parentIndex( index ), sceneGraphListIndex( TRR_NO_CHILDREN ), nrChildren( 0 )
		{
		}

		int parentIndex;
		int sceneGraphListIndex;
		int nrChildren;
	};

	/*
		Used for storing sceneGraph relationships in a list.
	*/
	struct ScenGraphLink
	{
		ScenGraphLink( int indexOFChild )
			: entityIndex( indexOFChild )
		{
		}

		int entityIndex;
	};
}

bool operator<( const trr::Entity& lhs, const trr::EntityLink& rhs );
bool operator<=( const trr::Entity& lhs, const trr::EntityLink& rhs );

bool operator<( const int& lhs, const trr::EntityLink& rhs );
bool operator<=( const int& lhs, const trr::EntityLink& rhs );
bool operator==( const int& lhs, const trr::EntityLink& rhs );

bool operator<( const trr::ScenGraphLink& lhs, const trr::ScenGraphLink& rhs );
bool operator<=( const trr::ScenGraphLink& lhs, const trr::ScenGraphLink& rhs );
bool operator==( const trr::ScenGraphLink& lhs, const trr::ScenGraphLink& rhs );


#endif