#ifndef TRR_GRAPHICS_COMPONENT_HPP
#define TRR_GRAPHICS_COMPONENT_HPP

#include <cstdint>

namespace trr
{
	struct GraphicsComponent
	{
		int textureIndex;
		int meshIndex;
		std::uint64_t meshHash;
		std::uint64_t textureHash;
	};
}

#endif