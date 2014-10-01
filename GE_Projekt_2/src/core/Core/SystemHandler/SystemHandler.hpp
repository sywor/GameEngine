#ifndef TRR_SYSTEM_HANDLER_HPP
#define TRR_SYSTEM_HANDLER_HPP

#include <Core/SystemHandler/SystemHandlerTemplateUtility.hpp>
#include <Core/SystemHandler/EntityBucket.hpp>
#include <Core/EntityHandler/Entity.hpp>
#include <utility/StickyIndexVector.hpp>

#include <array>

namespace trr
{
	template< typename... systems >
	class SystemHandler
	{
	private:

	public:
		SystemHandler()
		{
			m_systemList = {{ (new systems())... }};
		}

		~SystemHandler()
		{
			DeleteSystemsCall< systems... >( (int*)m_systemList.data() );
		}

		void Update( float deltaTime, float realTime )
		{
			UpdateSystemsCall< systems... >( (int*)&m_systemList[0], deltaTime, realTime );
		}

		int RegisterBucket( EntityMask mask )
		{
			return m_buckets.push_back( EntityBucket( mask ) );
		}

		EntityBucket& GetBucket( int index )
		{
			return m_buckets[index];
		}

		void ProcessEntityMask( int entityIndex, EntityMask mask, EntityMask old )
		{
			int size = m_buckets.Size();
			for( int i = 0; i < size; ++i )
			{
				m_buckets[ i ].AddEntity( entityIndex, mask, old );
			}
		}

	private:
		std::array<void*, sizeof...( systems )>		m_systemList;
		StickyIndexVector< EntityBucket >			m_buckets;
	};
}

#endif