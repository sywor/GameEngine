#include "LoDSystem.hpp"

#include <Instances/Instances.hpp>
#include <Core/ContentHandler/Containers/ObjMesh.hpp>
#include <Core/ContentHandler/Containers/TextureContainer.hpp>

#include <Components/TransformationComponent.hpp>
#include <Components/GraphicsComponent.hpp>

#include <gfx/GraphicsJobInformation.h>

#include "../shared/RenderInterface.h"
extern RenderInterface* renderInterface;



namespace trr
{
	void LoadAsset(Entity ent, std::string path, bool mesh)
	{
		trr::contentManager.GetResource(path, [ent, mesh](const void* data, std::uint64_t hash)
		{
			if (trr::entityHandler.GetIndex(ent) < 0)
			{
				LOG_DEBUG << "entity " << ent << " is dead in callback" << std::endl;
			}
			else
			{
				if (CONTENT_CHECK_VALID_DATA(data))
				{
					GraphicsComponent* grc = entityHandler.GetData< GraphicsComponent >(ent);
					if (mesh)
					{						
						if (grc != nullptr)
						{
							ObjMesh* mesh = (ObjMesh*)data;
							grc->meshIndex = mesh->meshIndex;
							grc->meshHash = hash;
							LOG_DEBUG << "loading succeeded!" << std::endl;
						}
					}
					else
					{
						if (grc != nullptr)
						{
							TextureContainer* text = (TextureContainer*)data;
							grc->textureIndex = text->textureIndex;
							grc->textureHash = hash;
							LOG_DEBUG << "loading succeeded!" << std::endl;
						}
					}
				}
				else
				{
					LOG_DEBUG << "loading failed!" << std::endl;
				}
			}
		});
	}


	LoDSystem::LoDSystem()
	{
		m_entityBucketID = systemHandler.RegisterBucket( 
			entityHandler.GenerateMask< TransformationComponent, GraphicsComponent >());
	}

	LoDSystem::~LoDSystem()
	{
	}


	void LoDSystem::Update(float dt, float realtime)
	{
		EntityBucket& localBucket = systemHandler.GetBucket( m_entityBucketID );
		
		std::vector< int >& entityIndicies = localBucket.GetVector();
		
		// "wall.obj.mesh.mesh"
		// "smiley.bmp.image.image"
		ICamera *Cam = renderInterface->getCamera();
		XMMATRIX tempMatrix = XMMatrixTranspose( Cam->getViewMatrix() );
		XMFLOAT4X4 matrix;
		XMStoreFloat4x4(&matrix, tempMatrix);
		XMFLOAT3 position = XMFLOAT3(matrix._41, matrix._42, matrix._43 );
		//XMVECTOR position = XMVECTOR( tempMatrix );

		for (auto it = entityIndicies.begin(); it != entityIndicies.end(); it++ )
		{
			GraphicsComponent* grc = entityHandler.GetData< GraphicsComponent >( *it );
			TransformationComponent* trc = entityHandler.GetData< TransformationComponent >(*it);

			if (grc->meshIndex != -1 && grc->textureIndex != -1)
			{
				XMFLOAT3 objectPos = XMFLOAT3(trc->pos[0], trc->pos[1], trc->pos[2]);

				XMFLOAT3 temp2;
				XMStoreFloat3(&temp2, XMLoadFloat3(&position) - XMLoadFloat3(&objectPos));
				XMStoreFloat3(&temp2, XMVector3Length(XMLoadFloat3(&temp2)));
				float distance = temp2.x;

				if (distance > 20)
				{
					// wall : 3245331227418423246
					// smiley : 16130332233530715104
					if (grc->meshHash != 3245331227418423246)
					{
						contentManager.Unload(grc->meshHash);
						contentManager.Unload(grc->textureHash);
						grc->meshHash = 0;
						grc->meshIndex = -1;
						grc->textureHash = 0;
						grc->textureIndex = -1;

						LoadAsset(entityHandler.GetEntity(*it), "wall.obj.mesh", true);
						LoadAsset(entityHandler.GetEntity(*it), "smiley.bmp.image", false);
					}
				}
				else
				{
					// bunker : 14737407184233560585
					// clown : 9273229865042457818
					if (grc->meshHash != 14737407184233560585)
					{
						contentManager.Unload(grc->meshHash);
						contentManager.Unload(grc->textureHash);
						grc->meshHash = 0;
						grc->meshIndex = -1;
						grc->textureHash = 0;
						grc->textureIndex = -1;
						
						LoadAsset(entityHandler.GetEntity(*it), "Bunker.obj.mesh", true );
						LoadAsset(entityHandler.GetEntity(*it), "clown.jpg.image", false );
					}
				}
			}

			if ( grc->meshIndex != -1 && grc->textureIndex != -1 )
			{
				GraphicsJobInfo* info = stackAllocator.allocate<GraphicsJobInfo>();

				if (info != nullptr)
				{
					info->mesh = grc->meshIndex;
					info->texture = grc->textureIndex;
					info->scale = trc->scale;
					std::memcpy(info->pos, trc->pos, 3 * sizeof(float));
					std::memcpy(info->quaternion, trc->quat, 4 * sizeof(float));

					renderInterface->AddRenderJob( info );

				}
			}
		}
	}




}