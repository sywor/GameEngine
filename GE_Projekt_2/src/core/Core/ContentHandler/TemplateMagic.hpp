#ifndef TEMPLATE_MAGIC_HPP
#define TEMPLATE_MAGIC_HPP





#include <array>
#include "../Memory/Allocation.h"

template <typename... Args>
struct Impl_contentManager;

template <typename First, typename... Args>
struct Impl_contentManager<First, Args...>
{
	static void Initialize( int** listToInit, Allocator* allocator )
  {
	  listToInit[ sizeof...(Args) ] = (int*)new( allocator->FlatAllocate( sizeof( First ))) First();
	  Impl_contentManager< Args... >::Initialize( listToInit, allocator );
  }
};

template <>
struct Impl_contentManager<>
{
  static void Initialize( int** listToInit, Allocator* allocator )
  {   
  }
};

template <typename... Args>
void InitializeArrayWithNew( int** listToInit, Allocator* allocator )
{
	Impl_contentManager<Args...>::Initialize( listToInit, allocator );
}


#endif