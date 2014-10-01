#include "Tests.h"
#include "StackAllocator.h"

//#define printmemusage
//#define printoutofmem
void doAllocByThePool(AllocationManager *settings, int _framesToRun)
{
	std::vector< littleFunc* > list;
	int size = sizeof(littleFunc);
	int tick = 0;
	static PoolAllocator sa(BLOCK_SIZE, NUMALLOC); // test1020 : 1mil runs : 3.79sec
	//DefaultAllocator sa; // test1020 : 1mil runs : 15.049 sec
	//sa.init(NUMALLOC);

	for (int j = 0; j < _framesToRun; j++)
	{

		int allocationsPerFrame = settings->allocationsPerFrameMin + ((rand() % settings->allocationsPerFrameMax) - settings->allocationsPerFrameMin);

		settings->NrOfAlocations += allocationsPerFrame;

		settings->allocationTimer.start();

		for (int i = 0; i < allocationsPerFrame; ++i)
		{
			int lifeTime = settings->lifeTimeMin + ((rand() % settings->lifeTimeMax) - settings->lifeTimeMin);
			int size = (rand() % 5);

			if (size == 0)
				list.push_back(sa.allocate<littleFunc>(i, lifeTime));
			else if (size == 1)
				list.push_back(sa.allocate<funky1>(i, lifeTime));
			else if (size == 2)
				list.push_back(sa.allocate<funky2>(i, lifeTime));
			else if (size == 3)
				list.push_back(sa.allocate<funky3>(i, lifeTime));
			else if (size == 4)
				list.push_back(sa.allocate<funky4>(i, lifeTime));




			if (list.back() == nullptr)
			{
				list.pop_back();
#ifdef printoutofmem
				printf("out of memory, allocated objects:%i\n Used memory: %i \n", list.size(), sa.getMemoryUtilization());
				int mem = sa.getMemoryUtilization();
				printf("Out of memory: ");

				for (int j = 0; j < mem / 5; j++)
					printf("|");

				for (int j = 0; j < 20 - mem / 5; j++)
					printf(" ");

				printf("| %i%%\n", sa.getMemoryUtilization());
#endif
				break;
			}
		}

		settings->allocationTimer.end();
		settings->TotalAllocationTime += settings->allocationTimer.getTimeElapsed();



		for (int i = 0; i < list.size(); ++i)
		{
			list[i]->doFunc();
		}

		settings->deallocationTimer.start();

		for (int i = 0; i < list.size();)
		{
			if (list[i]->lifeTime <= 0)
			{
				sa.deallocate(list[i]);
				list.erase(list.begin() + i);
			}
			else
			{
				i++;
			}
		}

		settings->deallocationTimer.end();
		settings->TotalDeallocationTime += settings->deallocationTimer.getTimeElapsed();



#ifdef printmemusage

		if (tick > 10)
		{
			int mem = sa.getMemoryUtilization();
			printf("Used memory: ");

			for (int j = 0; j < mem / 5; j++)
				printf("|");

			for (int j = 0; j < 20 - mem / 5; j++)
				printf(" ");

			printf("| \n");

			tick = 0;
		}
		else
		{
			tick++;
		}
#endif

	}

	while (list.size() > 0)
	{
		for (int i = 0; i < list.size(); ++i)
		{
			list[i]->doFunc();
		}

		settings->deallocationTimer.start();

		for (int i = 0; i < list.size();)
		{
			if (list[i]->lifeTime <= 0)
			{
				sa.deallocate(list[i]);
				list.erase(list.begin() + i);
			}
			else
			{
				i++;
			}
		}

		settings->deallocationTimer.end();
		settings->TotalDeallocationTime += settings->deallocationTimer.getTimeElapsed();

#ifdef printmem

		if (tick > 10)
		{
			int mem = sa.getMemoryUtilization();
			printf("Used memory: ");

			for (int j = 0; j < mem / 5; j++)
				printf("|");

			for (int j = 0; j < 20 - mem / 5; j++)
				printf(" ");

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
