#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <cstdint>

// class TEMPLATE
// int hej[8]

// template <t>
// class IRESOURCEDATA
// int size
// T   data;

// 36 1 2 3 4 5 6 7 8


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