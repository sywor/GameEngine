
#include <ios>
#include <crtdbg.h>
#include <cstddef>
#include <fstream>
#include <type_traits>
#include <string>

#include <Core/Memory/Allocation.h>
#include <Core/Memory/StackAllocator.h>

#include <Core/Memory/PoolAllocator.h>
#include <Core/Memory/DefaultAllocator.h>

#include <thread>
#include <Core/Memory/Timer.h>

#include <new>
#include <Core/Memory/Tests.h>





// Application.cpp : Defines the entry point for the console application.

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>


//#define TESTING


//int _tmain(int argc, _TCHAR* argv[])
//{
//	{
//		// this bracket is here to properly release all allocated data so crtdebug doesn't get confused
//
//		std::vector< AllocationManager > allocationManagers;
//		int i;
//		// Set up experiment parameters
//#pragma region parse_commands
//		while (1)
//		{
//			std::string command;
//			std::cin >> command;
//			if (0);
//			else if (command == "help")
//			{
//				std::cout << "List of commands: " << std::endl;
//				std::cout << " \"AM\" to add a new allocation manager" << std::endl;
//				std::cout << " \"load\" to load allocation managers from a file" << std::endl;
//				std::cout << " \"save\" to save all allocation managers configurations to a file" << std::endl;
//			}
//			else if (command == "AM") // AllocationManager
//			{
//				AllocationManager am;
//				std::cout << "Number of thread to spawn them in: ";
//				std::cin >> am.numberOfThread;
//				std::cout << "Life time min: ";
//				std::cin >> am.lifeTimeMin;
//				std::cout << "Life time max: ";
//				std::cin >> am.lifeTimeMax;
//				std::cout << "Size min: ";
//				std::cin >> am.sizeMin;
//				std::cout << "Size max: ";
//				std::cin >> am.sizeMax;
//				std::cout << "Allocations per frame min: ";
//				std::cin >> am.allocationsPerFrameMin;
//				std::cout << "Allocations per frame max: ";
//				std::cin >> am.allocationsPerFrameMax;
//				allocationManagers.push_back(am);
//				std::cout << "Added allocation manager" << std::endl;
//			}
//			else if (command == "load")
//			{
//				std::cout << "Enter file name: ";
//				std::string fileName;
//				std::cin >> fileName;
//				std::ifstream file(fileName);
//				if (!file.is_open())
//				{
//					std::cout << "Failed to load file: " << fileName << std::endl;
//					continue;
//				}
//				while (1)
//				{
//					std::string line;
//					std::getline(file, line);
//					if (line != "AM")
//						break;
//					AllocationManager am;
//					file >> am.numberOfThread;
//					file >> am.lifeTimeMin;
//					file >> am.lifeTimeMax;
//					file >> am.sizeMin;
//					file >> am.sizeMax;
//					file >> am.allocationsPerFrameMin;
//					file >> am.allocationsPerFrameMax;
//					file >> am.allocatorSize;
//					allocationManagers.push_back(am);
//					std::getline(file, line);
//				}
//
//			}
//			else if (command == "save")
//			{
//				std::cout << "Enter file name: ";
//				std::string fileName;
//				std::cin >> fileName;
//				std::ofstream file(fileName);
//				if (!file.is_open())
//				{
//					std::cout << "Failed to open file: " << fileName << std::endl;
//					continue;
//				}
//
//				for (int i(0); i < allocationManagers.size(); i++)
//				{
//					file << "AM" << std::endl;
//					AllocationManager &am = allocationManagers[i];
//					file << am.numberOfThread << " ";
//					file << am.lifeTimeMin << " ";
//					file << am.lifeTimeMax << " ";
//					file << am.sizeMin << " ";
//					file << am.sizeMax << " ";
//					file << am.allocationsPerFrameMin << " ";
//					file << am.allocationsPerFrameMax << " ";
//					file << std::endl;
//				}
//			}
//			else if (command == "run")
//			{
//				std::cout << "How many frames to run: " << std::endl;
//				std::cin >> i;
//
//				break;
//			}
//			else if (command == "quit")
//			{
//				return 0;
//			}
//			else
//			{
//				std::cout << "Unknown command: " << command << std::endl;
//			}
//		}
//#pragma endregion
//
//#ifdef TESTING
//		std::cout << std::endl << "TESTING!" << std::endl;
//
//		// To test if everything works
//		for( auto &am : allocationManagers )
//		{
//			std::cout << "AM:" << std::endl;
//			std::cout << am.lifeTimeMin << " " << am.lifeTimeMax << std::endl;
//			std::cout << am.sizeMin << " " << am.sizeMax << std::endl;
//			std::cout << am.allocationsPerFrameMin << " " << am.allocationsPerFrameMax << std::endl;
//			std::cout << std::endl;
//		}
//#endif
//
//#ifndef TESTING
//		std::cout << std::endl << "RUNNING!" << std::endl;
//
//		Timer t;
//		bool b = t.init();
//		if (!b)// Print result
//			return 1;
//		int sized = sizeof(bigFunc);
//		std::vector< AllocationManager > allocationManagers1 = allocationManagers;
//		std::vector< AllocationManager > allocationManagers2 = allocationManagers;
//
//		allocationManagers[0].Init();
//		allocationManagers1[0].Init();
//		allocationManagers2[0].Init();
//
//		t.start();
//	
//		/* --- POOL TESTS --- */
//		doAllocByThePool(&allocationManagers1.at(0), i);
//		//std::thread q(doAllocByThePool, &allocationManagers1.at(0), i);
//		//std::thread p(doAllocByThePool, &allocationManagers2.at(0), i);
//		//q.join();
//		//p.join();
//		/* --- /POOL TESTS ---*/
//
//		/* --- STACK TESTS --- */
//		/* NO THREADS */
//		//stackScen1<StackAllocator>(&allocationManagers1.at(0), i);
//		//stackScen1<DefaultAllocator>(&allocationManagers2.at(0), i);
//	
//		//stackScen2<StackAllocator, funky2>(&allocationManagers1.at(0), i);
//		//stackScen2<DefaultAllocator, funky2>(&allocationManagers2.at(0), i);
//	
//		/* /NO THREADS */
//
//		/* MULTI THREADS */
//		//std::thread q(stackScen1<StackAllocator>, &allocationManagers1.at(0), i);
//		//std::thread p(stackScen1<DefaultAllocator>, &allocationManagers2.at(0), i);
//		//std::thread q(stackScen2<StackAllocator, littleFunc>, &allocationManagers1.at(0), i);
//		//std::thread p(stackScen2<StackAllocator, littleFunc>, &allocationManagers2.at(0), i);
//
//		//q.join();
//		//p.join();
//		/* /MULTI THREADS */
//		/* --- /STACK TESTS --- */
//		t.end();
//
//		printf("Time: %f\n", t.getTimeElapsed()*0.000001f);
//		float time = t.getTimeElapsed()*0.000001f;
//
//		std::string tag = "PoolAllocator";
//		//std::string tag = "StackAllocator";
//		//std::string tag = "DefaultAllocator";
//
//		allocationManagers1[0].PrintStatistics(1, "PoolAllocator - thread A", time);
//		//allocationManagers2[0].PrintStatistics(2, "PoolAllocator - thread B", time);
//
//
//		/*
//		allocationManagers.at(0).AvarageAllocationTime = allocationManagers.at(0).TotalAllocationTime / allocationManagers.at(0).NrOfAlocations;
//		allocationManagers.at(0).AvarageDeallocationTime = allocationManagers.at(0).TotalDeallocationTime / allocationManagers.at(0).NrOfAlocations;
//		printf("T0: Total allocs: %i \n", allocationManagers.at(0).NrOfAlocations);
//		printf("T0: Total All Time: %f sek\n", allocationManagers.at(0).TotalAllocationTime * 0.000001f);
//		printf("T0: Total Deall Time: %f sek\n", allocationManagers.at(0).TotalDeallocationTime * 0.000001f);
//		printf("T0: Avg All Time: %f mili sek\n", allocationManagers.at(0).AvarageAllocationTime);
//		printf("T0: Avg Deall Time: %f mili sek\n", allocationManagers.at(0).AvarageDeallocationTime);
//		*/
//
//		/*allocationManagers1.at(0).AvarageAllocationTime = allocationManagers1.at(0).TotalAllocationTime / allocationManagers1.at(0).NrOfAlocations;
//		allocationManagers2.at(0).AvarageAllocationTime = allocationManagers2.at(0).TotalAllocationTime / allocationManagers2.at(0).NrOfAlocations;
//
//		allocationManagers1.at(0).AvarageDeallocationTime = allocationManagers1.at(0).TotalDeallocationTime / allocationManagers1.at(0).NrOfAlocations;
//		allocationManagers2.at(0).AvarageDeallocationTime = allocationManagers2.at(0).TotalDeallocationTime / allocationManagers2.at(0).NrOfAlocations;
//
//		printf("T1: Total allocs: %i \n", allocationManagers1.at(0).NrOfAlocations);
//		printf("T1: Avg All Time: %f \n", allocationManagers1.at(0).AvarageAllocationTime * 0.000001f);
//		printf("T1: Avg Deall Time: %f \n", allocationManagers1.at(0).AvarageDeallocationTime * 0.000001f);
//
//		printf("T2: Total allocs : %i \n", allocationManagers2.at(0).NrOfAlocations);
//		printf("T2: Avg All Time: %f \n", allocationManagers2.at(0).AvarageAllocationTime * 0.000001f);
//		printf("T2: Avg Deall Time: %f \n", allocationManagers2.at(0).AvarageDeallocationTime * 0.000001f);
//		*/
//
//#endif
//
//		printf("Press any key to kill program\n");
//		std::cin.ignore();
//		std::cin.get();
//
//	}	// this bracket is here to properly release all allocated data so crtdebug doesn't get confused
//
//	_CrtDumpMemoryLeaks();
//
//	return 0;
//}

/*
#include <Core/ContentHandler/ResourceManager.hpp>
#include <Core/ContentHandler/ResourceLoader.hpp>
#include <Core/ContentHandler/Resource.hpp>
#include <Core/ContentHandler/Loaders/TestLoader.hpp>

typedef trr::TestLoader TempLoader;
typedef trr::ResourceManager< 1024, 8, TempLoader, TempLoader > ExampleHandler;

int _tmain(int argc, _TCHAR* argv[])
{
	ExampleHandler handler;



	return 0;
}
*/