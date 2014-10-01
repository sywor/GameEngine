#ifndef TRR_DATA_ARRAY_HPP
#define TRR_DATA_ARRAY_HPP

#include <utility/StickyIndexVector.hpp>
#include <Core/EntityHandler/EntityHandler.hpp>

#include <vector>
#include <array>
#include <cstdint>

namespace trr
{
	template< typename T> 
	class DataArray
	{
	private:
		struct SizeObject
		{
			std::uint8_t data[ sizeof( T ) ];
		};


	public:
		DataArray()
		{
		}

		~DataArray()
		{
		}

		int PushData()
		{
			return data.push_back( SizeObject() );
		}

		void RemoveData( int index )
		{
			if( index >= 0  )
			{
				data.erase( index );
			}
		}

		T* operator[]( int index )
		{
			return (T*)&data[ index ];
		}

	private:
		StickyIndexVector< SizeObject > data;
	};

}




#endif