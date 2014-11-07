#ifndef TRR_LOD_SYSTEM_HPP
#define TRR_LOD_SYSTEM_HPP

#include <Core/SystemHandler/EntityBucket.hpp>

namespace trr
{
	class LoDSystem
	{
	private:

	public:
		LoDSystem();

		~LoDSystem();
		
		void Update( float deltaTime, float realTime );

	private:
		int		m_entityBucketID;
		

	};
}

#endif