#include "EntityStructs.hpp"


bool operator<( const trr::Entity& lhs, const trr::EntityLink& rhs )
{
	if( lhs < rhs.id )
		return true;
	return false;
}

bool operator<=( const trr::Entity& lhs, const trr::EntityLink& rhs )
{
	if( lhs <= rhs.id )
		return true;
	return false;
}

bool operator<( const int& lhs, const trr::EntityLink& rhs )
{
	if( lhs < rhs.index )
		return true;
	return false;
}

bool operator<=( const int& lhs, const trr::EntityLink& rhs )
{
	if( lhs <= rhs.index )
		return true;
	return false;
}

bool operator==( const int& lhs, const trr::EntityLink& rhs )
{
	if( lhs == rhs.index )
		return true;
	return false;
}


bool operator<( const trr::ScenGraphLink& lhs, const trr::ScenGraphLink& rhs )
{
	if( lhs.entityIndex < rhs.entityIndex)
		return true;
	return false;
}

bool operator<=( const trr::ScenGraphLink& lhs, const trr::ScenGraphLink& rhs )
{
	if( lhs.entityIndex <= rhs.entityIndex)
		return true;
	return false;
}

bool operator==( const trr::ScenGraphLink& lhs, const trr::ScenGraphLink& rhs )
{
	if( lhs.entityIndex == rhs.entityIndex)
		return true;
	return false;
}