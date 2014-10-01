#ifndef TRR_TESTSYSTEM_HPP
#define TRR_TESTSYSTEM_HPP

#include <Core/SystemHandler/EntityBucket.hpp>

namespace trr
{
	class TestSystem
	{
	private:

	public:
		TestSystem();

		~TestSystem();
		
		void Update( float deltaTime, float realTime );

	private:
		int		m_entityBucketID;
		

	};
}

#endif