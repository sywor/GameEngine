#ifndef TEST_LOADER_HPP
#define TEST_LOADER_HPP

#include "../ResourceLoader.hpp"


namespace trr
{

	class TestLoader : public ResourceLoader
	{
	private:

		virtual void* internal_Load( DataContainer in)			override;
		virtual void internal_unload( void* data )				override;

	public:
		TestLoader();
		virtual ~TestLoader() override;

		const std::string GetExtension() final { return "txt"; }

	private:

	};

}

#endif