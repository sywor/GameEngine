#include "EntityHandler.hpp"

#include <Instances/Instances.hpp>


namespace trr
{

	void UpdateTrrSystemHandlerBuckets( int entityIndex, EntityMask mask, EntityMask old )
	{
		systemHandler.ProcessEntityMask( entityIndex, mask, old );
	}

}