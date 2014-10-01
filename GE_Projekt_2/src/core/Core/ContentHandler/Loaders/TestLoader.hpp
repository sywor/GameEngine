#ifndef TEST_LOADER_HPP
#define TEST_LOADER_HPP

#include "../ResourceLoader.hpp"


namespace trr
{

	class TestLoader : public ResourceLoader
	{
	private:

		virtual void internal_Load( std::string path, std::uint64_t hash )	override;
		virtual void internal_unload( std::uint64_t hash )					override;

	public:
		TestLoader();
		virtual ~TestLoader() override;


	private:

		int testarray[500];


	};

}

#endif