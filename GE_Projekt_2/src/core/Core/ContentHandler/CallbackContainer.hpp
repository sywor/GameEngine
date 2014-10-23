#ifndef CALLBACK_CONTAINER_HPP
#define CALLBACK_CONTAINER_HPP

#include <cstdint>
#include <functional>

namespace trr
{

	struct CallbackContainer
	{
		CallbackContainer(std::uint64_t hash_, std::function<void(const void*)> function)
		: hash(hash_), callback( function )
		{ }

		std::function<void(const void*)>	callback;
		std::uint64_t				hash;
	};

}

#endif