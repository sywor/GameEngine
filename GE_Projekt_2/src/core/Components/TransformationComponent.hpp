#ifndef TRR_TRANSFORMATION_COMPONENT_HPP
#define TRR_TRANSFORMATION_COMPONENT_HPP

namespace trr
{

	struct TransformationComponent
	{
		float pos[ 3 ];
		float quat[ 4 ];
		float scale;
	};

}

#endif