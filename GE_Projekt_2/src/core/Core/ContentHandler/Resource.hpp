#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <cstdint>

namespace trr
{

	class Resource
	{
		std::uint64_t	hash;
		unsigned int	nrReferences;
		void*			data;
		int				loaderIndex;
	};

}

#endif