#ifndef TRR_BASE_LEVEL_HPP
#define TRR_BASE_LEVEL_HPP


namespace trr
{
	
	class BaseLevel abstract
	{
	private:

	public:

		virtual void Init() abstract;
		virtual void Unload() abstract;

	};

}

#endif