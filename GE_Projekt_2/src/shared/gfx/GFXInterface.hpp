#ifndef SRC_SHARED_GFX_GFXINTERFACE_HPP
#define SRC_SHARED_GFX_GFXINTERFACE_HPP

#ifdef _WIN32
#ifdef GFX_DLL_EXPORT
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#else
#define DLL_API 
#endif

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLEW/glew.h>
#include <gfx/GFXDefines.hpp>

//#include <gfx/GFXVertex.hpp>
//#include <gfx/GFXViewport.hpp>
//#include <gfx/GFXMaterial.hpp>
//#include <gfx/GFXFont.hpp>
//#include <bitmask/bitmask_types.hpp>
#include "../shared/RenderInterface.h"

extern RenderInterface* renderInterface;

namespace gfx
{
	DLL_API int Initialize(int screenWidth, int screenHeight);
	DLL_API int Resize(int screenWidth, int screenHeight);
	
	//DLL_API int SetViewport(int viewportID, Viewport* vp);
	DLL_API int SetViewportCamera( glm::mat4* view, glm::mat4* projection);
	
	DLL_API int Execute( gfxBitMask bitmask, void* data);
	
	DLL_API int SwapRenderJobBuffer();

	DLL_API int Render( float dt );

	int AddTexture(char* _data, int w, int h, int bpp)
	{

		//renderInterface->graphics->createTextureView(_data, w, h, bpp);

		return 0;
	}
}

namespace gfx
{
	//namespace content
	//{
	//	DLL_API int CreateMaterial(unsigned long long int& id);
	//	DLL_API int DeleteMaterial(unsigned long long int& id);
	//	DLL_API int SetMaterialTexture(const unsigned long long int& materialID, const unsigned long long int& textureID, TextureType type);
	//
	//	DLL_API int GetShaderID(unsigned int& shaderID, const char* shaderName);
	//
	//	DLL_API int LoadTexture(unsigned int& id, const char* path, unsigned char* data, int width, int height, bool replace = false);
	//	DLL_API int LoadCubemap(unsigned int& id, const char* path, unsigned char* posX, unsigned char* negX, unsigned char* posY, unsigned char* negY, unsigned char* posZ, unsigned char* negZ, int width, int height);
	//	DLL_API int DeleteTexture(unsigned int id);
	//
	//	template<class T>
	//	DLL_API int LoadMesh(unsigned int& meshID, int& sizeVerts, int& sizeIndices, T* verts, int* indices, bool replace = false);
	//	DLL_API int DeleteMesh( int meshID );
	//
	//	DLL_API int LoadFont( Font &font, FontSize size, const char* path );
	//	DLL_API int DeleteFont( const Font& font );
	//
	//}
	namespace debug
	{
		//DLL_API void DrawString(const char* text, int x, int y, glm::vec4 color);
	
		DLL_API void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec4 color);
	
		//DLL_API void DrawSphere(glm::vec3 position, float radius, glm::vec4 color);
	
		//DLL_API void DrawAABB(glm::vec3 min, glm::vec3 max, glm::vec4 color);
		
		//DLL_API void SwapDebugBuffers();
	
		//DLL_API void DrawOBB(glm::vec3 min, glm::vec3 max, glm::quat rotation, glm::vec4 color);
	}
}

#endif


// fixa content
	// content mapping
	// content loading
	// textured mesh test
	// fix lighting
		// ambient for voxels
		// fix texture for voxels

	// test lighting with voxels
	
