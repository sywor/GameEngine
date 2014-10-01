#ifndef TRR_SYSTEM_TEMPLATE_HACKS_HPP
#define TRR_SYSTEM_TEMPLATE_HACKS_HPP

#include <Core/EntityHandler/DataArray.hpp>

#include <cstdint>
#include <tuple>

template <typename... Args>
struct Implsd;

template <typename First, typename... Args>
struct Implsd<First, Args...>
{
	static void DeleteSystemsTemplateRecursion( int* systemArray )
	{
		delete ((First*)systemArray[ sizeof...(Args) ]);
		systemArray[ sizeof...(Args) ] = 0;
		Implsd< Args... >::DeleteSystemsTemplateRecursion( systemArray );
	}
};

template <>
struct Implsd<>
{
  static void DeleteSystemsTemplateRecursion( int* systemArray )
  {   
  }
};

template <typename... Args>
void DeleteSystemsCall( int* systemArray )
{
	Implsd<Args...>::DeleteSystemsTemplateRecursion( systemArray );
}



// /////////////////////////////////////////////////////////////////////////////////////////
template <typename... Args>
struct Impls;

template <typename First, typename... Args>
struct Impls<First, Args...>
{
	static void UpdateSystemsTemplateRecursion( int* systemArray, float dt, float realTime )
	{
		((First*)systemArray[ sizeof...(Args) ])->Update( dt, realTime );

		Impls< Args... >::UpdateSystemsTemplateRecursion( systemArray, dt, realTime );
	}
};

template <>
struct Impls<>
{
  static void UpdateSystemsTemplateRecursion( int* systemArray, float dt, float realTime )
  {   
  }
};

template <typename... Args>
void UpdateSystemsCall( int* systemArray, float dt, float realTime )
{
	Impls<Args...>::UpdateSystemsTemplateRecursion( systemArray, dt, realTime);
}




#endif