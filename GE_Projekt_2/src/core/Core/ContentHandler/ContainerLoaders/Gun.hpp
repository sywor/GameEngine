#pragma once
#include <string>
#include "Directory.hpp"


namespace Potato
{
	class Gun
	{
	public:
		Gun();
		~Gun();
		void Load(const std::string& _spudName);
		void* Shoot(const std::string& _spudName);

	private:
		Directory* root;
		std::string fileName;

		Directory* ReadHeader(void* _data, uint& _readPointer);
		std::string ReadString(void* _data, uint& _readPointer);
		uint ReadUint(void* _data, uint& _readPointer);
	};
}
