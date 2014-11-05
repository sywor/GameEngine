#ifndef __OBJ_LOADER__HPP
#define __OBJ_LOADER__HPP

#include "Core\ContentHandler\ResourceLoader.hpp"

namespace trr
{
	class ObjLoader
		: public ResourceLoader
	{
	private:
		void* internal_Load(DataContainer in) final;
		void internal_unload(void* data) final;
	protected:
	public:

		const std::string GetExtension() final { return "mesh"; }
	};
}
#endif