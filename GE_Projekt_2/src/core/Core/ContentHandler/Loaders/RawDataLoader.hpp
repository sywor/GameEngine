#ifndef __RAWDATA_LOADER__HPP
#define __RAWDATA_LOADER__HPP

#include "Core\ContentHandler\ResourceLoader.hpp"

namespace trr
{
	class RawDataLoader
		: public ResourceLoader
	{
	private:
		bool internal_Load(std::string path, Resource& r) final;
		void internal_unload(Resource& r) final;
		const std::string GetExtension() final { return "*"; }
	protected:
	public:
	};
}
#endif