#ifndef TRR_ENTITY_LINK_VECTOR_HPP
#define TRR_ENTITY_LINK_VECTOR_HPP

#include <Core/EntityHandler/Entity.hpp>
#include <Core/EntityHandler/EntityStructs.hpp>

#include <vector>
#include <cstring>

namespace trr
{
	class EntityLinkVector
	{
	public:
		EntityLinkVector()
		{
			m_dataTop = -1;
		}
		
		~EntityLinkVector()
		{
		}

		void push_back( EntityLink& param )
		{
			int size = m_data.size();
			++m_dataTop;
			if( m_dataTop == size )
			{ 
				m_data.push_back( param );
			}
			else
			{
				m_data[ m_dataTop ] = param;
			}
		}

		void erase( unsigned int index )
		{
			int delta = m_dataTop - index;
			std::memcpy( &m_data[ index ], &m_data[ index + 1 ], sizeof( EntityLink ) * delta );
			m_dataTop -= 1;
		}

		EntityLink& operator[]( unsigned int index )
		{
			return m_data[ index ];
		}

		int Find( Entity id )
		{
			int size = m_dataTop + 1;
			if( size < 2 )
			{
				if( size == 1 )
				{
					if( id == m_data[ 0 ].id )
					{
						return 0;
					}
					else
					{
						return -1;
					}
				}
				else
				{
					return -1;
				}
			}
			else
			{
				// binary search for insert index
				int top		= size - 1;
				int bottom	= 0;
				int mid		= bottom + ( top - bottom ) / 2;
				while( 1 < top - bottom )
				{
					if( id < m_data[ mid ] )
					{
						top = mid;
					}
					else
					{
						bottom = mid;
					}					
					mid = bottom + ( top - bottom ) / 2;
				}

				if( id <= m_data[ mid ].id )
				{
					return mid;
				}
				else if( id == m_data[ mid + 1 ].id )
				{
					return mid + 1;
				}
				else
				{
					return -1;
				}
			}
		}

		Entity Find( int index )
		{
			int size = m_dataTop + 1;
			if( size == 0 )
			{
				return InvalidEntity;
			}
			else
			{
				// binary search for insert index
				int top		= size - 1;
				int bottom	= 0;
				int mid		= bottom + ( top - bottom ) / 2;
				while( 1 < top - bottom )
				{
					if( index < m_data[ mid ] )
					{
						top = mid;
					}
					else
					{
						bottom = mid;
					}					
					mid = bottom + ( top - bottom ) / 2;
				}

				if( index == m_data[ mid ] )
				{
					return m_data[ mid ].id;
				}
				else if( index == m_data[ mid + 1 ] )
				{
					return m_data[ mid + 1 ].id;
				}
				else 
				{
					return InvalidEntity;
				}
			}
		}

	private:
		std::vector< EntityLink >	m_data;
		int							m_dataTop;
	};
}


#endif