#ifndef TRR_LEVEL1_HPP
#define TRR_LEVEL1_HPP

#include <Base/BaseLevel.hpp>
#include <Instances/Instances.hpp>

#include <vector>

namespace trr
{

	class Level1 : public BaseLevel
	{
	private:

	public:
		Level1();
		~Level1();

		virtual void Init()		override;
		virtual void Unload()	override;

	private:
		std::vector< Entity >	m_entities;

	};

}


#endif