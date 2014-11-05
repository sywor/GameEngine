#ifndef __RAWDATA_LOADER__HPP
#define __RAWDATA_LOADER__HPP

#include "Core\ContentHandler\ResourceLoader.hpp"

namespace trr
{
	class RawDataLoader
		: public ResourceLoader
	{
	private:
		void* internal_Load( DataContainer in ) final;
		void internal_unload( void* data ) final;
	protected:
	public:

		const std::string GetExtension() final { return "image"; }
	};
}
#endif