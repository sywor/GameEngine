#ifndef TEMPLATE_MAGIC_HPP
#define TEMPLATE_MAGIC_HPP





#include <array>
#include "../Memory/Allocation.h"
#include <Core/ContentHandler/ResourceLoader.hpp>
#include <malloc.h>

template <typename... Args>
struct Impl_contentManager;

template <typename First, typename... Args>
struct Impl_contentManager<First, Args...>
{
	static void Initialize(void* map, Allocator* allocator)
	{		
		(*reinterpret_cast< std::map< const std::string, trr::ResourceLoader* >* >(map))
			[((new(_malloca(sizeof(First))) First())->GetExtension())] 
			= new(allocator->FlatAllocate(sizeof(First))) First();

		return Impl_contentManager< Args... >::Initialize(map, allocator);
	}
};

template <>
struct Impl_contentManager<>
{
	static void Initialize(void* map, Allocator* allocator)
	{   
	}
};

template <typename... Args>
void InitializeArrayWithNew( void* map, Allocator* allocator )
{
	Impl_contentManager<Args...>::Initialize(map, allocator);
}



#endif