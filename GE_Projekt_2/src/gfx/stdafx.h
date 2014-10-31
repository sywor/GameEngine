#ifndef _STDAFX__H
#define _STDAFX__H

#include <windows.h>
#include <D3D11.h>


#include <d3dCompiler.h>
//#include <DirectXMath.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>

//#define D3DXMATRIX DirectX::XMMATRIX
//#define D3DXVECTOR3 DirectX::XMFLOAT3

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }
#define PI (3.14159265358979323846f)

static const int WIDTH = 800;
static const int HEIGHT = 600;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment (lib,"dxerr.lib")

#ifdef _DEBUG
//#pragma comment(lib, "d3dx11d.lib")
//#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#endif

struct MATRIX4X4;
struct VECTOR4;
struct VECTOR3;
struct VECTOR2;

struct COLOR
{
	FLOAT d[4];
	COLOR(float r, float g, float b, float a)
	{
		d[0] = r;
		d[1] = g;
		d[2] = b;
		d[3] = a;
	}

	operator const FLOAT* () { return (FLOAT*)this; }
};


	#include <DirectXMath.h>
	using namespace DirectX;

	//struct VECTOR
	//{
	//	XMVECTOR xmv;
	//	VECTOR() {}
	//	VECTOR(const XMVECTOR& _xmv) { xmv = _xmv; }
	//	VECTOR(XMFLOAT4 _xmf4) { xmv = XMLoadFloat4(&_xmf4); }
	//	VECTOR(XMFLOAT3 _xmf3) { xmv = XMLoadFloat3(&_xmf3); }
	//	VECTOR(XMFLOAT2 _xmf2) { xmv = XMLoadFloat2(&_xmf2); }
	//	VECTOR(float x, float y, float z, float w) { xmv = XMVectorSet(x, y, z, w); }
	//	VECTOR(float x, float y, float z) { xmv = XMLoadFloat3(&XMFLOAT3(x, y, z)); }
	//	VECTOR(float x, float y) { xmv = XMLoadFloat2(&XMFLOAT2(x, y)); }
	//};

	struct VECTOR2
	{
		XMFLOAT2 d;
		VECTOR2() : d(XMFLOAT2(0, 0))
		{}
		VECTOR2(float x, float y) : d(XMFLOAT2(x, y))
		{}
		XMFLOAT2 operator=(const XMFLOAT2& d2) { this->d = d2; }

		operator const XMFLOAT2(){ return this->d; }
	};

	struct VECTOR3
	{
		XMFLOAT3 d;

		VECTOR3() : d(XMFLOAT3(0, 0, 0))
		{}
		VECTOR3(float x, float y, float z) : d(XMFLOAT3(x, y, z))
		{}
		XMFLOAT3 operator=(const XMFLOAT3& d2) { this->d = d2; }

		operator const XMFLOAT3(){ return this->d; }
	};

	struct VECTOR4
	{
		XMFLOAT4 d;

		VECTOR4() : d(XMFLOAT4(0, 0, 0, 0))
		{}

		VECTOR4(float x, float y, float z, float w) : d(XMFLOAT4(x, y, z, w))
		{}

		XMFLOAT4 operator=(const XMFLOAT4& d2) { this->d = d2; }

		operator const XMFLOAT4(){ return this->d; }
	};

	struct MATRIX4X4
	{
		XMFLOAT4X4 d;

		MATRIX4X4()
		{
			XMStoreFloat4x4(&d, XMMatrixIdentity());
		}

		MATRIX4X4(const MATRIX4X4& m4x4)
			: d(m4x4.d)
		{}

		MATRIX4X4(const XMFLOAT4X4& m4x4)
			: d(m4x4)
		{}

		//XMFLOAT4X4 operator=(const XMFLOAT4X4& d2)	{ XMStoreFloat4x4(&this->d, XMLoadFloat4x4(&d2)); }
		operator const XMFLOAT4*()					{ return (XMFLOAT4*)this; }
		operator XMMATRIX()							{ return XMLoadFloat4x4(&d); }
	};

	struct cbWorld
	{
		MATRIX4X4 world;
		MATRIX4X4 view;
		MATRIX4X4 projection;
		MATRIX4X4 worldInv;
	};

	struct Vertex
	{
		XMFLOAT4 pos;
		XMFLOAT4 normal;
		XMFLOAT2 texC;
	};

	//struct Vertex
	//{
	//	VECTOR4 pos;
	//	VECTOR4 normal;
	//	VECTOR2 texC;
	//};










//////////////////////////////////////////////////////////////////////////
// to find memory leaks
//////////////////////////////////////////////////////////////////////////
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define myMalloc(s)       _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myCalloc(c, s)    _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myRealloc(p, s)   _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myExpand(p, s)    _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define myFree(p)         _free_dbg(p, _NORMAL_BLOCK)
#define myMemSize(p)      _msize_dbg(p, _NORMAL_BLOCK)
#define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define myDelete delete  // Set to dump leaks at the program exit.
#define myInitMemoryCheck() \
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#define myDumpMemoryLeaks() \
	_CrtDumpMemoryLeaks()
#else
#define myMalloc malloc
#define myCalloc calloc
#define myRealloc realloc
#define myExpand _expand
#define myFree free
#define myMemSize _msize
#define myNew new
#define myDelete delete
#define myInitMemoryCheck()
#define myDumpMemoryLeaks()
#endif 
//////////////////////////////////////////////////////////////////////////



#endif
