#ifndef TEST_LOADER_HPP
#define TEST_LOADER_HPP

#include "../ResourceLoader.hpp"


namespace trr
{

	class TestLoader : public ResourceLoader
	{
	private:

		virtual bool internal_Load(std::string path, Resource& r, DataContainer _data)	override;
		virtual void internal_unload(Resource& r)					override;

	public:
		TestLoader();
		virtual ~TestLoader() override;


	private:

		const std::string GetExtension() final { return "test"; }

	};

}

#endif