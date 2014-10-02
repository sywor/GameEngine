#pragma once

#include <utility/utilities.hpp>
#include <Instances/Instances.hpp>
#include <Base/Levels/Level1.hpp>
#include <utility/HighresTimer.hpp>
#include <logger/Logger.hpp>
#include <Systems/TestSystem.hpp>
#include <Core\ContentHandler\OssiansPlaygroud_DeleteUponRelese.hpp>


int main( int argc, char* argv[] )
{
	//trr::Level1 level;
	//level.Init();

	ossianTest::zipTest();

	utilities::pause();
	return 0;
}



