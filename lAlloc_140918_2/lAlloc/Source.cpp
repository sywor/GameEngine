
#include <ios>
#include <crtdbg.h>
#include <cstddef>
#include <fstream>
#include <type_traits>
#include <string>

#include "Allocation.h"
#include "StackAllocator.h"
#include "AlignedStackAllocator.h"
#include "PoolAllocator.h"
#include "DefaultAllocator.h"

#include <thread>
#include "Timer.h"
static const int NUMALLOC = 4096;
static const int BLOCK_SIZE = 8;
#include <new>

struct littleFunc
{
	littleFunc(int _id,int _lifeTime)
	{ 
		this;
		id = _id; 
		lifeTime = _lifeTime;
	}



	int id;
	int lifeTime;
	void doFunc()
	{
		lifeTime--;
		//printf("id:%i\n", id);
	}

	~littleFunc()
	{
		//printf("%i destroyed!\n", id);
	}
};

struct funky1 : littleFunc
{
	funky1(int _id,int _lifeTime):littleFunc(_id,_lifeTime)
	{

	}
	int sizeDif[1];
};

struct funky2 : littleFunc
{
	funky2(int _id,int _lifeTime):littleFunc(_id,_lifeTime)
	{

	}
	int sizeDif[2];
};
struct funky3 : littleFunc
{
	funky3(int _id,int _lifeTime):littleFunc(_id,_lifeTime)
	{

	}
	int sizeDif[3];
};
struct funky4 : littleFunc
{
	funky4(int _id,int _lifeTime):littleFunc(_id,_lifeTime)
	{

	}
	int sizeDif[4];
};


struct AllocationManager
{
	void Init()
	{
		allocationTimer.init();
		deallocationTimer.init();
		NrOfAlocations = 0;
		TotalAllocationTime = 0;
		TotalDeallocationTime = 0;
	}

	int numberOfThread;
	int lifeTimeMin;
	int lifeTimeMax;
	int sizeMin;
	int sizeMax;
	int allocationsPerFrameMin;
	int allocationsPerFrameMax;

	// statistics
	int NrOfAlocations;
	float TotalAllocationTime;
	float AvarageAllocationTime;
	float TotalDeallocationTime;
	float AvarageDeallocationTime;
	Timer allocationTimer;
	Timer deallocationTimer;

	void PrintStatistics( int threadIndex, std::string tag, float totalTime )
	{
		AvarageAllocationTime = TotalAllocationTime / NrOfAlocations;
		AvarageDeallocationTime = TotalDeallocationTime / NrOfAlocations;

		printf("T%i: Total allocs: %i \n", threadIndex, NrOfAlocations );
		printf("T%i: Total All Time: %f sek\n", threadIndex, TotalAllocationTime * 0.000001f );
		printf("T%i: Total Deall Time: %f sek\n", threadIndex, TotalDeallocationTime * 0.000001f );
		printf("T%i: Avg All Time: %f mili sek\n", threadIndex, AvarageAllocationTime );
		printf("T%i: Avg Deall Time: %f mili sek\n", threadIndex, AvarageDeallocationTime );
		printf("\n");

		std::fstream ff;
		ff.open( "Statistics.txt", std::ios::app );
		if (!ff.is_open())
			return;

		ff << tag << std::endl;
		ff << "Total run time: " << totalTime << std::endl;
		ff << "T" << threadIndex << ": Total allocs:       " << NrOfAlocations << std::endl;
		ff << "T" << threadIndex << ": Total alloc time:   " << TotalAllocationTime * 0.000001f << " sec" << std::endl;
		ff << "T" << threadIndex << ": Total dealloc time: " << TotalDeallocationTime * 0.000001f << " sec" << std::endl;
		ff << "T" << threadIndex << ": Avg alloc time:     " << AvarageAllocationTime << " ms " << std::endl;
		ff << "T" << threadIndex << ": Avg dealloc time:   " << AvarageDeallocationTime << " ms " << std::endl;
		ff << std::endl;

		ff.close();
	}
};

//void doAlloc()
//{
//	StackAllocator sa;
//	sa.init(NUMALLOC * sizeof(littleFunc));
//
//	for(int i = 0; i < NUMALLOC; ++i)
//	{
//		littleFunc* d = sa.allocate<littleFunc>(i);
//	}
//	sa.clear();
//	
//	for(int i = 0; i < NUMALLOC; ++i)
//	{
//		littleFunc* d = (littleFunc*)(sa.getMarker() + sizeof(littleFunc) * i);
//		d->doFunc();
//	}
//}

struct A
{
	char c;
};




//#define printmemusage
//#define printoutofmem
void doAllocByThePool(AllocationManager *settings,int _framesToRun)
{
	std::vector< littleFunc* > list;
	int size = sizeof( littleFunc );
	int tick = 0;
	PoolAllocator sa(BLOCK_SIZE); // test1020 : 1mil runs : 3.79sec
	//DefaultAllocator sa; // test1020 : 1mil runs : 15.049 sec
	
	sa.init( NUMALLOC );

	for(int j = 0;j < _framesToRun;j++)
	{

		int allocationsPerFrame = settings->allocationsPerFrameMin + ((rand() % settings->allocationsPerFrameMax) - settings->allocationsPerFrameMin) ;

		settings->NrOfAlocations += allocationsPerFrame;

		settings->allocationTimer.start();

		for(int i = 0; i < allocationsPerFrame; ++i)
		{
			int lifeTime = settings->lifeTimeMin + ((rand() % settings->lifeTimeMax) - settings->lifeTimeMin) ;
			int size = (rand() % 5) ;

			if(size == 0)
				list.push_back( sa.allocate<littleFunc>(i,lifeTime) );
			else if(size == 1)
				list.push_back( sa.allocate<funky1>(i,lifeTime) );
			else if(size == 2)
				list.push_back( sa.allocate<funky2>(i,lifeTime) );
			else if(size == 3)
				list.push_back( sa.allocate<funky3>(i,lifeTime) );
			else if(size == 4)
				list.push_back( sa.allocate<funky4>(i,lifeTime) );




			if( list.back() == nullptr )
			{
				list.pop_back();
#ifdef printoutofmem
				//printf( "out of memory, allocated objects:%i\n Used memory: %i \n", list.size(),sa.getMemoryUtilization() );
				int mem = sa.getMemoryUtilization();
				printf( "Out of memory: ");

				for(int j = 0; j < mem/5;j++)
					printf( "|");

				for(int j = 0; j < 20 - mem/5;j++)
					printf( " ");

				printf("| %i%%\n", sa.getMemoryUtilization());
#endif
				break;
			}
		}

		settings->allocationTimer.end();
		settings->TotalAllocationTime += settings->allocationTimer.getTimeElapsed();
		


		for(int i = 0; i < list.size(); ++i)
		{
			list[i]->doFunc();
		}

		settings->deallocationTimer.start();

		for(int i = 0; i < list.size(); )
		{
			if(list[i]->lifeTime <= 0)
			{
				sa.deallocate( list[i] );
				list.erase(list.begin()+i);
			}
			else
			{
				i++;
			}
		}

		settings->deallocationTimer.end();
		settings->TotalDeallocationTime += settings->deallocationTimer.getTimeElapsed();



#ifdef printmemusage

		if(tick > 10)
		{
			int mem = sa.getMemoryUtilization();
				printf( "Used memory: ");

			for(int j = 0; j < mem/5;j++)
				printf( "|");

			for(int j = 0; j < 20 - mem/5;j++)
				printf( " ");

			printf("| \n");

			tick = 0;
		}
		else
		{
			tick++;
		}
#endif

	}

	while(list.size() > 0)
	{
		for(int i = 0; i < list.size(); ++i)
		{
			list[i]->doFunc();
		}

		settings->deallocationTimer.start();

		for(int i = 0; i < list.size(); )
		{
			if(list[i]->lifeTime <= 0)
			{
				sa.deallocate( list[i] );
				list.erase(list.begin()+i);
			}
			else
			{
				i++;
			}
		}

		settings->deallocationTimer.end();
		settings->TotalDeallocationTime += settings->deallocationTimer.getTimeElapsed();

#ifdef printmem

		if(tick > 10)
		{
			int mem = sa.getMemoryUtilization();
			printf( "Used memory: ");

			for(int j = 0; j < mem/5;j++)
				printf( "|");

			for(int j = 0; j < 20 - mem/5;j++)
				printf( " ");

			printf("| \n");

			tick = 0;
		}
		else
		{
			tick++;
		}
#endif
	}

}


// Application.cpp : Defines the entry point for the console application.
//
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//#define TESTING


int _tmain(int argc, _TCHAR* argv[])
{
	std::vector< AllocationManager > allocationManagers;
	int i;
	// Set up experiment parameters
#pragma region parse_commands
	while (1)
	{
		std::string command;
		std::cin >> command;
		if (0);
		else if (command == "help")
		{
			std::cout << "List of commands: " << std::endl;
			std::cout << " \"AM\" to add a new allocation manager" << std::endl;
			std::cout << " \"load\" to load allocation managers from a file" << std::endl;
			std::cout << " \"save\" to save all allocation managers configurations to a file" << std::endl;
		}
		else if (command == "AM") // AllocationManager
		{
			AllocationManager am;
			std::cout << "Number of thread to spawn them in: ";
			std::cin >> am.numberOfThread;
			std::cout << "Life time min: ";
			std::cin >> am.lifeTimeMin;
			std::cout << "Life time max: ";
			std::cin >> am.lifeTimeMax;
			std::cout << "Size min: ";
			std::cin >> am.sizeMin;
			std::cout << "Size max: ";
			std::cin >> am.sizeMax;
			std::cout << "Allocations per frame min: ";
			std::cin >> am.allocationsPerFrameMin;
			std::cout << "Allocations per frame max: ";
			std::cin >> am.allocationsPerFrameMax;
			allocationManagers.push_back(am);
			std::cout << "Added allocation manager" << std::endl;
		}
		else if (command == "load")
		{
			std::cout << "Enter file name: ";
			std::string fileName;
			std::cin >> fileName;
			std::ifstream file(fileName);
			if (!file.is_open())
			{
				std::cout << "Failed to load file: " << fileName << std::endl;
				continue;
			}
			while (1)
			{
				std::string line;
				std::getline(file, line);
				if (line != "AM")
					break;
				AllocationManager am;
				file >> am.numberOfThread;
				file >> am.lifeTimeMin;
				file >> am.lifeTimeMax;
				file >> am.sizeMin;
				file >> am.sizeMax;
				file >> am.allocationsPerFrameMin;
				file >> am.allocationsPerFrameMax;
				allocationManagers.push_back(am);
				std::getline(file, line);
			}

		}
		else if (command == "save")
		{
			std::cout << "Enter file name: ";
			std::string fileName;
			std::cin >> fileName;
			std::ofstream file(fileName);
			if (!file.is_open())
			{
				std::cout << "Failed to open file: " << fileName << std::endl;
				continue;
			}

			for (int i(0); i < allocationManagers.size(); i++)
			{
				file << "AM" << std::endl;
				AllocationManager &am = allocationManagers[i];
				file << am.numberOfThread << " ";
				file << am.lifeTimeMin << " ";
				file << am.lifeTimeMax << " ";
				file << am.sizeMin << " ";
				file << am.sizeMax << " ";
				file << am.allocationsPerFrameMin << " ";
				file << am.allocationsPerFrameMax << " ";
				file << std::endl;
			}
		}
		else if (command == "run")
		{
			std::cout << "How many frames to run: " << std::endl;
			std::cin >> i;

			break;
		}
		else if (command == "quit")
		{
			return 0;
		}
		else
		{
			std::cout << "Unknown command: " << command << std::endl;
		}
	}
#pragma endregion

#ifdef TESTING
	std::cout << std::endl << "TESTING!" << std::endl;

	// To test if everything works
	for( auto &am : allocationManagers )
	{
		std::cout << "AM:" << std::endl;
		std::cout << am.lifeTimeMin << " " << am.lifeTimeMax << std::endl;
		std::cout << am.sizeMin << " " << am.sizeMax << std::endl;
		std::cout << am.allocationsPerFrameMin << " " << am.allocationsPerFrameMax << std::endl;
		std::cout << std::endl;
	}
#endif

#ifndef TESTING
	std::cout << std::endl << "RUNNING!" << std::endl;


	Timer t;
	bool b = t.init();
	if (!b)// Print result
		return 1;
	//doAlloc();system("pause");
	//while(true)return 0;
	//{
	//	t.start();
	//	doAlloc();
	//	t.end();

	//	printf("dt: %f\n", t.getTimeElapsed());
	//}

	std::vector< AllocationManager > allocationManagers1 = allocationManagers;
	std::vector< AllocationManager > allocationManagers2 = allocationManagers;

	allocationManagers[0].Init();
	allocationManagers1[0].Init();
	allocationManagers2[0].Init();

	t.start();
	//doAllocByThePool(&allocationManagers.at(0),i);

	std::thread q(doAllocByThePool, &allocationManagers1.at(0), i);
	std::thread p(doAllocByThePool, &allocationManagers2.at(0), i);

	q.join();
	p.join();

	t.end();





	printf("Time: %f\n", t.getTimeElapsed()*0.000001f);
	float time = t.getTimeElapsed()*0.000001f;

	std::string tag = "PoolAllocator";
	//std::string tag = "DefaultAllocator";

	allocationManagers1[0].PrintStatistics(1, tag, time);
	allocationManagers2[0].PrintStatistics(2, tag, time);

	/*
	allocationManagers.at(0).AvarageAllocationTime = allocationManagers.at(0).TotalAllocationTime / allocationManagers.at(0).NrOfAlocations;
	allocationManagers.at(0).AvarageDeallocationTime = allocationManagers.at(0).TotalDeallocationTime / allocationManagers.at(0).NrOfAlocations;
	printf("T0: Total allocs: %i \n", allocationManagers.at(0).NrOfAlocations);
	printf("T0: Total All Time: %f sek\n", allocationManagers.at(0).TotalAllocationTime * 0.000001f);
	printf("T0: Total Deall Time: %f sek\n", allocationManagers.at(0).TotalDeallocationTime * 0.000001f);
	printf("T0: Avg All Time: %f mili sek\n", allocationManagers.at(0).AvarageAllocationTime);
	printf("T0: Avg Deall Time: %f mili sek\n", allocationManagers.at(0).AvarageDeallocationTime);
	*/

	/*allocationManagers1.at(0).AvarageAllocationTime = allocationManagers1.at(0).TotalAllocationTime / allocationManagers1.at(0).NrOfAlocations;
	allocationManagers2.at(0).AvarageAllocationTime = allocationManagers2.at(0).TotalAllocationTime / allocationManagers2.at(0).NrOfAlocations;

	allocationManagers1.at(0).AvarageDeallocationTime = allocationManagers1.at(0).TotalDeallocationTime / allocationManagers1.at(0).NrOfAlocations;
	allocationManagers2.at(0).AvarageDeallocationTime = allocationManagers2.at(0).TotalDeallocationTime / allocationManagers2.at(0).NrOfAlocations;

	printf("T1: Total allocs: %i \n", allocationManagers1.at(0).NrOfAlocations);
	printf("T1: Avg All Time: %f \n", allocationManagers1.at(0).AvarageAllocationTime * 0.000001f);
	printf("T1: Avg Deall Time: %f \n", allocationManagers1.at(0).AvarageDeallocationTime * 0.000001f);

	printf("T2: Total allocs : %i \n", allocationManagers2.at(0).NrOfAlocations);
	printf("T2: Avg All Time: %f \n", allocationManagers2.at(0).AvarageAllocationTime * 0.000001f);
	printf("T2: Avg Deall Time: %f \n", allocationManagers2.at(0).AvarageDeallocationTime * 0.000001f);
*/

	_CrtDumpMemoryLeaks();
	
	#endif

	printf( "Press any key to kill program\n" );
	std::cin.ignore();
	std::cin.get();
	return 0;
}