#ifndef TRR_ENTITY_HANDLER_TYPEDEF_HPP
#define TRR_ENTITY_HANDLER_TYPEDEF_HPP

#include <Core/EntityHandler/EntityHandler.hpp>

/* Include component header here */
#include <Components/TransformationComponent.hpp>
#include <Components/GraphicsComponent.hpp>

	typedef trr::EntityHandler<
	
	/* List components here */
	trr::TransformationComponent,
	trr::GraphicsComponent
	
	> EntityHandlerImp;


#endif