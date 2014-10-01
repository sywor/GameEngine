#include "TestSystem.hpp"

#include <Instances/Instances.hpp>



namespace trr
{

	TestSystem::TestSystem()
	{
		m_entityBucketID = systemHandler.RegisterBucket( 
			entityHandler.GenerateMask< int, float >() );
	}

	TestSystem::~TestSystem()
	{
	}

	void TestSystem::Update( float dt, float realtime )
	{
	}




}