#pragma once

#include "Core\ContentHandler\ResourceLoader.hpp"

class RawImageLoader
	: public trr::ResourceLoader
{
private:
protected:
public:

private:
protected:
public:

	/*
	Specific function for loading implementation.
	Note: Should the memory allocator run out of memory
	CONTENT_CALLBACK_OUT_OF_MEMORY should be returned
	in order to propagate the correct error.
	If ignored and nullptr is used when an error occurrs,
	CONTENT_CALLBACK_LOADING_FAILED will be propagated insted.
	*/
	void* internal_Load(DataContainer in) final;
	void internal_unload(void* data) final;

	/*
	Get the file extension associated with a particular
	resource loader. Used for matching a type of resource
	with a loader.
	*/
	const std::string GetExtension() final { return "image"; }
};