#ifndef TRR_SYSTEM_HANDLER_TYPEDEF_HPP
#define TRR_SYSTEM_HANDLER_TYPEDEF_HPP

#include <Core/SystemHandler/SystemHandler.hpp>

/* Include system headers here */
#include <Systems/TestSystem.hpp>



namespace trr
{
	typedef SystemHandler< 

		/* List systems here */
		TestSystem


	> SystemHandlerImpl;


}

#endif