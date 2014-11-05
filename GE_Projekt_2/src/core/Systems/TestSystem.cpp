#include "TestSystem.hpp"

#include <Instances/Instances.hpp>
#include <Components/TransformationComponent.hpp>
#include <Components/GraphicsComponent.hpp>


namespace trr
{

	TestSystem::TestSystem()
	{
		m_entityBucketID = systemHandler.RegisterBucket( 
			entityHandler.GenerateMask< TransformationComponent, GraphicsComponent >());
	}

	TestSystem::~TestSystem()
	{
	}


	void TestSystem::Update( float dt, float realtime )
	{
		EntityBucket& localBucket = systemHandler.GetBucket( m_entityBucketID );
		
		std::vector< int >& entityIndicies = localBucket.GetVector();
		
		for (auto it = entityIndicies.begin(); it != entityIndicies.end(); it++ )
		{
			GraphicsComponent* grc = entityHandler.GetData< GraphicsComponent >( *it );
			TransformationComponent* trc = entityHandler.GetData< TransformationComponent >(*it);

			int o = 0;


		}
	}




}