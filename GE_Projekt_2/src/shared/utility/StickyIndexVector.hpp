#ifndef TRR_STICKY_INDEX_VECTOR
#define TRR_STICKY_INDEX_VECTOR

#include <vector>
#include <cstring>

namespace trr
{
	template< typename T >
	class StickyIndexVector
	{
	public:
		StickyIndexVector()
		{
			m_openListTop = -1;
		}

		~StickyIndexVector()
		{
		}

		int push_back( T& param )
		{
			if( m_openListTop != -1 )
			{
				int index = m_openList.front();
				m_openListTop--;
				if( m_openListTop != -1 )
				{
					int size = m_openList.size();
					std::memcpy( &m_openList[0], &m_openList[1], sizeof(int) * (size - 1) );
				}
				std::memcpy( &m_data[ index ], &param, sizeof(T) );
				return index;
			}
			else
			{
				m_data.push_back( param );
				return m_data.size() - 1;
			}
		}

		void erase( unsigned int index )
		{
			int size = m_openList.size();
			m_openListTop++;
			if( size == m_openListTop )
			{
				m_openList.push_back( index );
			}
			else
			{
				m_openList[ m_openListTop ] = index;
			}
		}

		T& operator[]( unsigned int index )
		{
			return m_data[ index ];
		}

		unsigned int Size()
		{
			return m_data.size();
		}

	private:
		std::vector< T >			m_data;
		
		std::vector< unsigned int > m_openList;
		int							m_openListTop;
	};
}


#endif