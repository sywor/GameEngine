#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <cstdint>
#include <string>


namespace trr
{
	enum RState
	{
		LOADING,
		UNLOADING,
		READY
	};

	class Resource
	{
	public:
		Resource(std::uint64_t _hash = -1)
		: hash(_hash), nrReferences(0), data(nullptr), loaderExtension("*"), state(LOADING)
		{}

	private:
		void operator=(const Resource& r)
		{
			hash			= r.hash;
			nrReferences	= r.nrReferences;
			state			= r.state;
			data			= r.data;
			loaderExtension = r.loaderExtension;
		}

		template< int MemoryBlockSize, int sizeOfMemory, typename... LoadersDef >
		friend class ResourceManager;
		friend class ResourceLoader;

	protected:
		std::uint64_t		hash;
		unsigned int		nrReferences;
		RState				state;
		void*				data;
		std::string			loaderExtension;

	public:
		const unsigned int	getReferences() { return nrReferences; }
		const void const*	getData()		{ return data; }
		const std::string	getExtension()	{ return loaderExtension; }
		const std::uint64_t	getHash()		{ return hash; }
		const RState		getState()		{ return state; }
	};
}

#endif