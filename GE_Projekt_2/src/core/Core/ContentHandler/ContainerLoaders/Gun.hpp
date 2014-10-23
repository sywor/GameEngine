#pragma once
#include <string>
#include <Core/Memory/PoolAllocator.h>
#include "Directory.hpp"


namespace Potato
{
	class Gun
	{
	public:
		Gun(PoolAllocator* _allocator);
		~Gun();

		/*
		Reads the header from the Spud file. This must be done before Shoot is called. 
		*/
		void Load(const std::string& _spudName);

		/*
		Returns the data of the querried file from the Spud file. Returns nullptr if file not found. 
		*/
		void* Shoot(const std::string& _spudName);

	private:
		Directory* root;
		std::string fileName;
		PoolAllocator* allocator;

		Directory* ReadHeader(void* _data, uint& _readPointer);
		std::string ReadString(void* _data, uint& _readPointer);
		uint ReadUint(void* _data, uint& _readPointer);
	};
}
