#include "Level1.hpp"
#include <Instances/Instances.hpp>
#include <Core/ContentHandler/Containers/ObjMesh.hpp>
#include <Core/ContentHandler/Containers/TextureContainer.hpp>

namespace trr
{

	Level1::Level1()
	{
	}

	Level1::~Level1()
	{
	}

	void Level1::Init()
	{
		
		trr::Entity e = trr::entityHandler.CreateEntity();
		trr::TransformationComponent* trc = trr::entityHandler.AddComponent<trr::TransformationComponent>(e);
		trr::GraphicsComponent* grc = trr::entityHandler.AddComponent<trr::GraphicsComponent>(e);
		grc->meshIndex = -1;
		grc->textureIndex = -1;
		grc->meshHash = 0;
		grc->textureHash = 0;

		trc->pos[0] = 0.0f;
		trc->pos[1] = 0.0f;
		trc->pos[2] = 10.0f;

		//trr::contentManager.GetResource("smiley.bmp.image.image");
		//trr::contentManager.GetResource("wall.obj.mesh.mesh");


		trr::contentManager.GetResource("wall.obj.mesh.mesh", [e](const void* data, std::uint64_t hash)
		{
			if (trr::entityHandler.GetIndex(e) < 0)
			{
				LOG_DEBUG << "entity " << e << " is dead in callback" << std::endl;
			}
			else
			{
				if (CONTENT_CHECK_VALID_DATA(data))
				{
					ObjMesh* mesh = (ObjMesh*)data;
					GraphicsComponent* grc = entityHandler.GetData< GraphicsComponent >(e);
					if (grc != nullptr)
					{
						grc->meshHash = hash;
						grc->meshIndex = mesh->meshIndex;
						LOG_DEBUG << "loading succeeded!" << std::endl;
					}
				}
				else
				{
					LOG_DEBUG << "loading failed!" << std::endl;
				}
			}
		});

		trr::contentManager.GetResource("smiley.bmp.image.image", [e](const void* data, std::uint64_t hash)
		{
			if (trr::entityHandler.GetIndex(e) < 0)
			{
				LOG_DEBUG << "entity " << e << " is dead in callback" << std::endl;
			}
			else
			{
				if (CONTENT_CHECK_VALID_DATA(data))
				{
					TextureContainer* texture = (TextureContainer*)data;
					GraphicsComponent* grc = entityHandler.GetData< GraphicsComponent >(e);
					if (grc != nullptr)
					{
						grc->textureHash = hash;
						grc->textureIndex = texture->textureIndex;
						LOG_DEBUG << "loading succeeded!" << std::endl;
					}
				}
				else
				{
					LOG_DEBUG << "loading failed!" << std::endl;
				}
			}
		});


	}

	void Level1::Unload()
	{
		int size = m_entities.size();
		for (int i = 0; i < size; i++) {
			trr::entityHandler.RemoveEntity( m_entities[ i ] );
		}
		m_entities.clear();
	}
}