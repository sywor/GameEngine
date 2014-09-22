#pragma once

#include <ios>
#include <fstream>
#include <string>
#include <vector>

#include "Timer.h"
#include "PoolAllocator.h"
#include "TestDataStructs.h"

static const int NUMALLOC = 4096;
static const int BLOCK_SIZE = 8;

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
	int allocatorSize;

	// statistics
	int NrOfAlocations;
	float TotalAllocationTime;
	float AvarageAllocationTime;
	float TotalDeallocationTime;
	float AvarageDeallocationTime;
	Timer allocationTimer;
	Timer deallocationTimer;

	void PrintStatistics(int threadIndex, std::string tag, float totalTime)
	{
		AvarageAllocationTime = TotalAllocationTime / NrOfAlocations;
		AvarageDeallocationTime = TotalDeallocationTime / NrOfAlocations;

		printf("T%i: Total allocs: %i \n", threadIndex, NrOfAlocations);
		printf("T%i: Total All Time: %f sek\n", threadIndex, TotalAllocationTime * 0.000001f);
		printf("T%i: Total Deall Time: %f sek\n", threadIndex, TotalDeallocationTime * 0.000001f);
		printf("T%i: Avg All Time: %f mili sek\n", threadIndex, AvarageAllocationTime);
		printf("T%i: Avg Deall Time: %f mili sek\n", threadIndex, AvarageDeallocationTime);
		printf("\n");

		std::fstream ff;
		ff.open("Statistics.txt", std::ios::app);
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

void doAllocByThePool(AllocationManager *settings, int _framesToRun);

/* requriements: 
	1) single frame lifetime
	2) same or similiar sizes of objects 
	notes: littlefuncs only */
template<typename Allocator>
void stackScen1(AllocationManager *settings, int _framesToRun)
{
	std::vector<littleFunc*> list;	// list of datastruct to test
	Allocator sa;					// the allocator to test
	if (!sa.init(settings->allocatorSize))
		return;

	uint numAllocsPerFrame = settings->allocationsPerFrameMin;
	settings->NrOfAlocations = numAllocsPerFrame * _framesToRun;

	/* Run by alloted number of frames */
	for (int i = 0; i < _framesToRun; ++i)
	{
		/* Allocate objects */
		settings->allocationTimer.start();
		for (int i = 0; i < numAllocsPerFrame; ++i)
		{
			int size = (rand() % 6);

			switch (size)
			{
			case 0:
				list.push_back(sa.allocate<littleFunc>(i, 1));
				break;
			case 1:
				list.push_back(sa.allocate<funky1>(i, 1));
				break;
			case 2:
				list.push_back(sa.allocate<funky2>(i, 1));
				break;
			case 3:
				list.push_back(sa.allocate<funky3>(i, 1));
				break;
			case 4:
				list.push_back(sa.allocate<funky4>(i, 1));
				break;
			case 5:
				list.push_back(sa.allocate<bigFunc>(i, 1));
				break;
			}
		}
		settings->allocationTimer.end();
		settings->TotalAllocationTime += settings->allocationTimer.getTimeElapsed();

		// deallocate all objects by fifo
		settings->deallocationTimer.start();
		while (!list.empty())
		{
			sa.deallocate(list[list.size() - 1]);
			list.pop_back();
		}
		settings->deallocationTimer.end();
		settings->TotalDeallocationTime += settings->deallocationTimer.getTimeElapsed();
	}
}

/* requriements:
	1) single frame lifetime 
	2) same sized objects 
	notes: whatever data type you want */
template<typename Allocator, typename DataStruct>
void stackScen2(AllocationManager *settings, int _framesToRun)
{
	std::vector<DataStruct*> list;	// list of datastruct to test
	Allocator sa;					// the allocator to test
	if (!sa.init(settings->allocatorSize))
		return;

	uint numAllocsPerFrame	= settings->allocationsPerFrameMin;
	settings->NrOfAlocations= numAllocsPerFrame * _framesToRun;
	
	/* Run by alloted number of frames */
	for (int i = 0; i < _framesToRun; ++i)
	{	
		/* Allocate objects */
		settings->allocationTimer.start();
		for (int i = 0; i < numAllocsPerFrame; ++i)
		{
			list.push_back(sa.allocate<DataStruct>(i, 1));
		}
		settings->allocationTimer.end();
		settings->TotalAllocationTime += settings->allocationTimer.getTimeElapsed();
		
		// deallocate all objects by fifo
		settings->deallocationTimer.start();
		while (!list.empty())
		{
			sa.deallocate(list[list.size()-1]);
			list.pop_back();
		}
		settings->deallocationTimer.end();
		settings->TotalDeallocationTime += settings->deallocationTimer.getTimeElapsed();
	}
}