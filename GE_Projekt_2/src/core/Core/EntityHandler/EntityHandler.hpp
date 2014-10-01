#ifndef TRR_ENTITY_HANDLER_HPP
#define TRR_ENTITY_HANDLER_HPP

#include <Core/EntityHandler/EntityStructs.hpp>

#include <utility/StickyIndexVector.hpp>
#include <utility/BinarySearchVector.hpp>
#include <Core/EntityHandler/EntityLinkVector.hpp>
#include <Core/EntityHandler/DataArray.hpp>
#include <Core/EntityHandler/Entity.hpp>
#include <Core/EntityHandler/EntityHandlerTemplateUtility.hpp>

#include <vector>
#include <array>

#define DATA_INDEX						ArrayIndex< T, std::tuple< components... >>::index
#define DATAHANDLER_ARRAY_META_SIZE		sizeof...( components )

namespace trr
{
	template< typename... components > 
	class EntityHandler
	{
	private:
		
		struct ComponentList 
		{ 
			int data[ sizeof...(components) ]; 

			int& operator[]( unsigned int index )
			{
				return data[ index ];
			}
		};

		void StripEntity( int index )
		{
			ComponentList& componentList = m_entities[ index ];
			RemoveComponentCall<components...>( (int*)&m_dataPool[0], (int*)&componentList );
			std::memset( &componentList, -1, sizeof...( components ) * sizeof( float ) );
		}

		EntityMask GenerateMask( int indexOfEntity )
		{
			EntityMask mask;
			std::memset( &mask, 0, sizeof( EntityMask ) );
			ComponentList& componentList = m_entities[ indexOfEntity ];
			for( int i = 0; i < sizeof...( components ); ++i )
				mask |= componentList[ i ] < 0 ? 0 : 1 << i;
			return mask;
		}


	public:
		EntityHandler()
		{
			m_dataPool = {{ (new DataArray<components>())... }};
			ID_Pool = 0;
		}

		~EntityHandler()
		{
			for( int i = 0; i < sizeof...( components ); ++i )
			{
				if( m_dataPool[ i ] != nullptr )
					delete m_dataPool[ i ];
				m_dataPool[ i ] = nullptr;
			}
		}

		/* Entity utility */
		Entity CreateEntity()
		{
			int index = m_entities.push_back( ComponentList() );
			std::memset( &m_entities[ index ], -1, sizeof...( components ) * sizeof( int ) );
			m_entityLinkList.push_back( EntityLink( ID_Pool, index ) );
			m_sceneGraph.push_back( ScenGraphInfo( TRR_NO_PARENT ) );
			return ID_Pool++;
		}
		
		void RemoveEntity( Entity id )
		{
			int entityListIndex = m_entityLinkList.Find( id );
			int index = m_entityLinkList[ entityListIndex ].index;
			EntityMask old = GenerateMask( index );
			StripEntity( index );
			UpdateTrrSystemHandlerBuckets( id, GenerateMask( index ), old );
			m_entityLinkList.erase( entityListIndex );
			m_entities.erase( index );
			m_sceneGraph.erase( index );
		}

		int GetIndex( Entity id )
		{
			return m_entityLinkList.Find( id );
		}

		Entity GetEntity( int entityIndex )
		{
			return m_entityLinkList.Find( entityIndex );
		}

		/* Component utility */
		template< typename T >
		T* AddComponent( Entity id )
		{
			int index = m_entityLinkList[ m_entityLinkList.Find( id ) ].index;
			EntityMask old = GenerateMask( index );
			m_entities[ index ][ DATA_INDEX ] = ((DataArray<T>*)m_dataPool[ DATA_INDEX ])->PushData();
			UpdateTrrSystemHandlerBuckets( id, GenerateMask( index ), old );
			return (*((DataArray<T>*)m_dataPool[ DATA_INDEX ]))[ m_entities[ index ][ DATA_INDEX ] ];
		}

		template< typename T > 
		void RemoveComponent( Entity id )
		{
			int index = m_entityLinkList[ m_entityLinkList.Find( id ) ].index;
			EntityMask old = GenerateMask( index );
			((DataArray<T>*)m_dataPool[ DATA_INDEX ])->RemoveData( m_entities[ index ][ DATA_INDEX ] );
			m_entities[ index ][ DATA_INDEX ] = -1;
			UpdateTrrSystemHandlerBuckets( id, GenerateMask( index ), old );
		}

		template< typename T > 
		T* GetData( int entityIndex )
		{
			return (*(DataArray<T>*)m_dataPool[ DATA_INDEX ])[ m_entities[ entityIndex ][ DATA_INDEX ] ];
		}

		template< typename T >
		T* GetData( Entity id )
		{
			int entityIndex = m_entityLinkList[ m_entityLinkList.Find( id ) ].index;
			return (*(DataArray<T>*)m_dataPool[ DATA_INDEX ])[ m_entities[ entityIndex ][ DATA_INDEX ] ];
		}

		/* system & bucket utility */
		template< typename... maskComponentList >
		EntityMask GenerateMask()
		{
			EntityMask mask;
			std::memset( &mask, 0, sizeof( EntityMask ) );
			std::array< int, sizeof...( maskComponentList ) > temp = 
				{{ (ArrayIndex< maskComponentList, std::tuple< components... >>::index)... }};
			for( int i = 0; i < sizeof...( maskComponentList ); ++i )
				mask |= 1 << temp[ i ];
			return mask;
		}

		/* Scene graph utility */
		void AddChild( Entity parent, Entity child )
		{
			int parentIndex = m_entityLinkList.Find( parent );
			int childIndex = m_entityLinkList.Find( child );

			// increase nr children
			m_sceneGraph[ parentIndex ].nrChildren++;

			// set parentindex etc. in child
			m_sceneGraph[ childIndex ].parentIndex = parentIndex;

			// if parent dont have scenegraphlistindex, add one
			if( m_sceneGraph[ parentIndex ].sceneGraphListIndex == TRR_NO_CHILDREN )
			{
				// insert child index into scenegraphlist
				int parent_SceneListIndex = m_sceneGraphList.push_back( ScenGraphLink( childIndex ) );
				m_sceneGraph[ parentIndex ].sceneGraphListIndex = parent_SceneListIndex;
			}
			else
			{
				// insert child index into scenegraphlist
				int insertIndex = m_sceneGraph[ parentIndex ].sceneGraphListIndex;
				while( insertIndex != m_sceneGraphList.Size() && m_sceneGraphList[ insertIndex ].entityIndex < childIndex )
				{
					insertIndex++;
				}
				
				// insert at end of list
				if( insertIndex == m_sceneGraphList.Size() )
				{
					m_sceneGraphList.push_back( ScenGraphLink( childIndex ) );
				}
				else
				{
					m_sceneGraphList.Insert( insertIndex, ScenGraphLink( childIndex ) );

					// loop through all following sceneGraph entries, 
					// if index is after insertion point, increase by one
					int size = m_sceneGraph.Size();
					for( int i = 0; i < size; ++i )
					{
						m_sceneGraph[ i ].sceneGraphListIndex += (m_sceneGraph[ i ].sceneGraphListIndex > insertIndex);
					}
				}
			}

			// switch parent with in list earlies child in order to always be updated first in systems
			int firstChildIndex = m_sceneGraphList[ m_sceneGraph[ parentIndex ].sceneGraphListIndex ].entityIndex;
			if( parentIndex > firstChildIndex )
			{
				ComponentList parentIndicies = m_entities[ parentIndex ];
				ComponentList childIndicies = m_entities[ firstChildIndex ];

				m_entities[ parentIndex ] = childIndicies;
				m_entities[ childIndex ] = parentIndicies;

				// update EntityLink so Entity id points correctly
				m_entityLinkList[ parentIndex ].index = childIndex;
				m_entityLinkList[ childIndex ].index = parentIndex;
			}
		}

		void RemoveChild( Entity child )
		{
			int childIndex = m_entityLinkList.Find( child );
			int parentIndex = m_sceneGraph[ childIndex ].parentIndex;

			if( m_sceneGraph[ childIndex ].parentIndex == TRR_NO_PARENT )
			{
				return;
			}
			
			// remove child from list in sceneGraphList'
			int eraseIndex = m_sceneGraph[ parentIndex ].sceneGraphListIndex;
			while( m_sceneGraphList[ eraseIndex ].entityIndex != childIndex )
			{
				eraseIndex++;
			}
			m_sceneGraphList.erase( eraseIndex );
			
			// loop through all in m_sceneGraph, if index is after point of removal decrease index by 1
			int size = m_sceneGraph.Size();
			for( int i = 0; i < size; ++i )
			{
				m_sceneGraph[ i ].sceneGraphListIndex -= ( m_sceneGraph[ i ].sceneGraphListIndex > eraseIndex );
			}

			// if no more children, scengraphlistindex to NO_CHILDREN
			m_sceneGraph[ parentIndex ].nrChildren--;
			if( m_sceneGraph[ parentIndex ].nrChildren == 0 )
			{
				m_sceneGraph[ parentIndex ].sceneGraphListIndex = TRR_NO_CHILDREN;
			}
			
			// set parent to NO_PARENT in m_sceneGraph for child
			m_sceneGraph[ childIndex ].parentIndex = TRR_NO_PARENT;

		}
		
		Entity GetParent( Entity id )
		{
			int index = m_entityLinkList.Find( id );
			return m_entityLinkList.Find( m_sceneGraph[ index ].parentIndex );
		}
		
		int	GetParent( int entityIndex )
		{
			return m_sceneGraph[ entityIndex ].parentIndex;
		}

		ScenGraphLink* GetChildren( int entityIndex )
		{
			int index = m_sceneGraph[ entityIndex ].sceneGraphListIndex;
			if( index == TRR_NO_CHILDREN )
			{
				return nullptr;
			}
			return &m_sceneGraphList[ m_sceneGraph[ entityIndex ].sceneGraphListIndex ];
		}
		
		int	GetNumberOfChildren( Entity parent )
		{
			int index = m_entityLinkList.Find( parent );
			return m_sceneGraph[ index ].nrChildren;
		}

		int	GetNumberOfChildren( int entityIndex )
		{
			return m_sceneGraph[ entityIndex ].nrChildren;
		}

	private:
		unsigned int											ID_Pool;

		StickyIndexVector< ComponentList >						m_entities;
		EntityLinkVector										m_entityLinkList;

		StickyIndexVector< ScenGraphInfo >						m_sceneGraph;
		BinarySearchVector< ScenGraphLink >						m_sceneGraphList;

		std::array< void*, sizeof...( components ) >			m_dataPool;
	};

	void UpdateTrrSystemHandlerBuckets( int entityIndex, EntityMask mask, EntityMask old );
}

#endif


// add comments to this class
	// note volotile functions etc.
// add class to hold premade scene graph operations in a classy manner


