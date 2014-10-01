#ifndef TRR_BINARY_SEARCH_VECTOR_HPP
#define TRR_BINARY_SEARCH_VECTOR_HPP

#include <vector>
#include <cstring>

namespace trr
{
	template< typename T >
	class BinarySearchVector
	{
	public:
		BinarySearchVector()
		{
		}
		
		~BinarySearchVector()
		{
		}

		int push_back( T& param )
		{
			data.push_back( param );
			return data.size() - 1;
		}

		int push_back_sorted( T& param )
		{
			int size = data.size();
			if( size == 0 )
			{
				data.push_back( param );
				return 0;
			}
			else
			{
				// binary search for insert index
				int top		= size - 1;
				int bottom	= 0;
				int mid = size / 2;
				while( 1 < top - bottom )
				{
					if( param < data[ mid ] )
					{
						top = mid;
					}
					else
					{
						bottom = mid;
					}					
					mid = bottom + ( top - bottom ) / 2;
				}

				if( param <= data[ mid ] )
				{
					data.insert( data.begin() + mid, param );
					return mid;
				}
				else if( mid + 1 < size && param <= data[ mid + 1 ] )
				{
					data.insert( data.begin() + mid + 1, param );
					return mid + 1;
				}
				else 
				{
					data.push_back( param );
					return 0;
				}
			}
		}

		void erase( unsigned int index )
		{
			data.erase( data.begin() + index );
		}


		int Find( T& param )
		{
			int size = data.size();
			if( size == 0 )
			{
				return -1;
			}
			else
			{
				// binary search for insert index
				int top		= size - 1;
				int bottom	= 0;
				int mid		= bottom + ( top - bottom ) / 2;
				while( 1 < top - bottom )
				{
					if( param < data[ mid ] )
					{
						top = mid;
					}
					else
					{
						bottom = mid;
					}					
					mid = bottom + ( top - bottom ) / 2;
				}

				if( param == data[ mid ] )
				{
					return mid;
				}
				else if ( mid + 1 >= size )
				{
					return -1;
				}
				else if( param == data[ mid + 1 ] )
				{
					return mid + 1;
				}
				else
				{
					return -1;
				}
			}
		}

		T& operator[]( unsigned int index )
		{
			return data[ index ];
		}

		unsigned int Size()
		{
			return data.size();
		}

		void Insert( int index, T& obj )
		{
			data.insert( data.begin() + index, obj );
		}

	private:
		std::vector< T > data;
	};
}


#endif