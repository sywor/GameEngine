#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <cstdint>

namespace trr
{

	struct Resource
	{
		Resource()
		: hash(-1), nrReferences(0), data(nullptr), loaderIndex(-1)
		{}

		std::uint64_t	hash;
		unsigned int	nrReferences;
		void*			data;
		int				loaderIndex;
	};

}

#endif