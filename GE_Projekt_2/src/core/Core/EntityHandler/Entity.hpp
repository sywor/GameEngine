#ifndef TRR_ENTITY_DEFINITION_HPP
#define TRR_ENTITY_DEFINITION_HPP

#include <cstdint>
#include <climits>

namespace trr
{
	typedef std::uint32_t							EntityMask;
	typedef unsigned int							Entity;
	#define InvalidEntity							(unsigned int)-1
}

#endif