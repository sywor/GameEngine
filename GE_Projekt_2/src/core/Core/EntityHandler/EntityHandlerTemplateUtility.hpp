#ifndef TRR_TEMPLATE_HACKS_HPP
#define TRR_TEMPLATE_HACKS_HPP

#include <Core/EntityHandler/DataArray.hpp>
#include <Core/EntityHandler/Entity.hpp>

#include <cstdint>
#include <tuple>



// /////////////////////////////////////////////////////////////////////////////////////////
template <class... R>
struct Match;

template <class T, class U>
struct Match<T,U>
{
	static const bool exists = false;
};

template <class T>
struct Match<T,T>
{
	static const bool exists = true;
};

template <class T, class... R> 
struct Match<T,T,R...>
{
	static const bool exists = true;
};

template <class T, class U, class... R> 
struct Match<T,U,R...>
{
	static const bool exists = Match<T,R...>::exists;
};




// /////////////////////////////////////////////////////////////////////////////////////////
template< class T, class Tuple >
struct ArrayIndex;

template< class T, class... Types >
struct ArrayIndex< T, std::tuple< T, Types... >>
{
	static const std::size_t index = 0;
};

template< class T, class U, class... Types >
struct ArrayIndex< T, std::tuple< U, Types... >>
{
	static const std::size_t index = 1 + ArrayIndex< T, std::tuple< Types... >>::index;
};



// /////////////////////////////////////////////////////////////////////////////////////////
template <typename... Args>
struct Impl;

template <typename First, typename... Args>
struct Impl<First, Args...>
{
	static void RemoveComponentTemplateRecursion( int* dataArrayList, int* componentIndicies )
	{
		((trr::DataArray<First>*)dataArrayList[ sizeof...(Args) ])->RemoveData( componentIndicies[ sizeof...(Args) ] );
		Impl< Args... >::RemoveComponentTemplateRecursion( dataArrayList, componentIndicies );
	}
};

template <>
struct Impl<>
{
  static void RemoveComponentTemplateRecursion( int* dataArrayList, int* componentIndicies )
  {   
  }
};

template <typename... Args>
void RemoveComponentCall( int* dataArrayList, int* componentIndicies )
{
	Impl<Args...>::RemoveComponentTemplateRecursion( dataArrayList, componentIndicies );
}


#endif