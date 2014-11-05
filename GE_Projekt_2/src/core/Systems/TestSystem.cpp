#include "TestSystem.hpp"

#include <Instances/Instances.hpp>
#include <Components/TransformationComponent.hpp>
#include <Components/GraphicsComponent.hpp>

#include <gfx/GraphicsJobInformation.h>

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
			if ( grc->meshIndex != -1 || grc->textureIndex != -1 )
			{
				GraphicsJobInfo* info = stackAllocator.allocate<GraphicsJobInfo>();
				if (info != nullptr)
				{
					info->mesh = grc->meshIndex;
					info->texture = grc->textureIndex;
					info->scale = trc->scale;
					std::memcpy(info->pos, trc->pos, 3 * sizeof(float));
					std::memcpy(info->quaternion, trc->quat, 4 * sizeof(float));
				}
			}
		}
	}




}