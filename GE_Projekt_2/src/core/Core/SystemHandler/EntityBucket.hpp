#ifndef TRR_ENTITY_BUCKET_HPP
#define TRR_ENTITY_BUCKET_HPP


#include <utility/BinarySearchVector.hpp>
#include <Core/EntityHandler/Entity.hpp>

namespace trr
{
	class EntityBucket
	{
	public:
		EntityBucket( EntityMask mask )
		{
			m_mask = mask;
		}

		~EntityBucket()
		{
		}

		void AddEntity( int entityIndex, EntityMask mask, EntityMask oldMask )
		{
			if( m_mask == (mask & m_mask) )
			{
				int index = m_entites.Find( entityIndex );
				if( index == -1 )
				{
					m_entites.push_back( entityIndex );
					return;
				}
			}
			else if( m_mask == (oldMask & m_mask) )
			{
				int index = m_entites.Find( entityIndex );
				if( index != -1 )
				{
					m_entites.erase( index );
					return;
				}
			}			
		}

		std::vector< int >& GetVector()
		{
			return m_entites.GetVector();
		}


	private:
		BinarySearchVector< int >		m_entites;
		EntityMask						m_mask;
	};
}

#endif