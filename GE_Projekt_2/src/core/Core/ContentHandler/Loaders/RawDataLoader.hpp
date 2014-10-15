#ifndef __RAWDATA_LOADER__HPP
#define __RAWDATA_LOADER__HPP

#include "Core\ContentHandler\ResourceLoader.hpp"

namespace trr
{
	class RawDataLoader
		: public ResourceLoader
	{
	private:
		bool internal_Load(void* data, DataContainer _data) final;
		void internal_unload(void* data) final;
	protected:
	public:

		const std::string GetExtension() final { return "*"; }
	};
}
#endif