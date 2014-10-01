#ifndef TRR_CORE_INSTANCES_HPP
#define TRR_CORE_INSTANCES_HPP

#include <Core/EntityHandler/EntityHandlerTypedef.hpp>
#include <Core/SystemHandler/SystemHandlerTypedef.hpp>
#include <Core/ContentHandler/ResourceManagerTypedef.hpp>

#include <Core/Memory/PoolAllocator.h>
#include <Core/Memory/StackAllocator.h>


namespace trr
{
	extern EntityHandlerImp		entityHandler;
	extern SystemHandlerImpl	systemHandler;
	extern ResourceManagerImpl	contentManager;
}

#endif