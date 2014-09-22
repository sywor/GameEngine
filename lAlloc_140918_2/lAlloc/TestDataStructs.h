#pragma once

struct littleFunc
{
	littleFunc(int _id, int _lifeTime)
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
	funky1(int _id, int _lifeTime) :littleFunc(_id, _lifeTime)
	{

	}
	int sizeDif[1];
};

struct funky2 : littleFunc
{
	funky2(int _id, int _lifeTime) :littleFunc(_id, _lifeTime)
	{

	}
	int sizeDif[2];
};
struct funky3 : littleFunc
{
	funky3(int _id, int _lifeTime) :littleFunc(_id, _lifeTime)
	{

	}
	int sizeDif[3];
};
struct funky4 : littleFunc
{
	funky4(int _id, int _lifeTime) :littleFunc(_id, _lifeTime)
	{

	}
	int sizeDif[4];
};
struct bigFunc : littleFunc
{
	bigFunc(int _id, int _lifeTime) :littleFunc(_id, _lifeTime)
	{

	}
	int sizeDif[6];
};