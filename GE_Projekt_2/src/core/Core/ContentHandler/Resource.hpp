#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <cstdint>
#include <string>

namespace trr
{
	enum RState
	{
		WAITING_LOAD,
		LOADING,
		UNLOADING,
		READY
	};

	class Resource
	{
	public:
		Resource(std::uint64_t _hash = -1)
		: hash(_hash), nrReferences(0), data(nullptr), loaderExtension(nullptr), state(LOADING), path(nullptr)
		{}

	private:
		void operator=(const Resource& r)
		{
			hash			= r.hash;
			nrReferences	= r.nrReferences;
			state			= r.state;
			data			= r.data;
			loaderExtension = r.loaderExtension;
			path			= r.path;
		}

		template< int MemoryBlockSize, int sizeOfMemory, int internalMetaMemoryLimit, typename... LoadersDef >
		friend class ResourceManager;
		friend class ResourceLoader;

	protected:
		std::uint64_t		hash;
		unsigned int		nrReferences;
		RState				state;
		void*				data;
		char*				loaderExtension;
		char*				path;

	public:
		const unsigned int	getReferences() const { return nrReferences; }
		const void*			getData()		const { return data; }
		const std::string	getExtension()	const { return std::string( loaderExtension ); }
		const std::uint64_t	getHash()		const { return hash; }
		const RState		getState()		const { return state; }
		const std::string	getPath()		const { return std::string( path ); }
	};
}

#endif