//#include <Rendering/GFXCore.hpp>
//#include <Mesh/MeshManager.hpp>
#include <gfx/GFXInterface.hpp>

#include <GLEW/glew.h>
#include <SDL/SDL_opengl.h>
#include <iostream>



//#include <gfx/GFXViewport.hpp>
//#include <Viewport/ViewportContainer.hpp>
//#include <Rendering/RenderJob/RenderJobManager.hpp>
//#include <Texture/TextureManager.hpp>
//#include <Material/MaterialManager.hpp>
//#include <Text/fontsystem.hpp>
//#include <DebugSystems/DebugManager.hpp>

void APIENTRY glErrorCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const char* _message, void* _userParam)
{

}

namespace gfx
{
	int Initialize(int screenWidth, int screenHeight)
	{
		
	
		return GFX_SUCCESS;
	}
	
	int Render(float dt)
	{

		return 1;
	}
	
	int Resize(int screenWidth, int screenHeight)
	{
		return 1;
	}
	
	//int SetViewport(int viewportID, Viewport* vp)
	//{
	//	return VPContainerInstance().SetVP(*vp, viewportID);
	//}
	//
	int SetViewportCamera( glm::mat4* view, glm::mat4* projection)
	{

		return GFX_SUCCESS;
	}
		
	int Execute( gfxBitMask bitmask, void* data )
	{

		return GFX_SUCCESS;
	}
	
	DLL_API int SwapRenderJobBuffer()
	{

		return GFX_SUCCESS;
	}
}

namespace gfx
{
	//namespace content
	//{
	//	template<>
	//	DLL_API int LoadMesh<StaticVertex>(unsigned int& meshID, int& sizeVerts, int& sizeIndices, StaticVertex* verts, int* indices, bool replace)
	//	{
	//		if (!replace)
	//			return MeshManagerInstance().LoadMesh<StaticVertex>(meshID, sizeVerts, sizeIndices, verts, indices);
	//		else
	//			return MeshManagerInstance().ReplaceMesh<StaticVertex>(meshID, sizeVerts, sizeIndices, verts, indices);
	//	}
	//
	//	template<>
	//	DLL_API int LoadMesh<AnimatedVertex>(unsigned int& meshID, int& sizeVerts, int& sizeIndices, AnimatedVertex* verts, int* indices, bool replace)
	//	{
	//		if (!replace)
	//			return MeshManagerInstance().LoadMesh<AnimatedVertex>(meshID, sizeVerts, sizeIndices, verts, indices);
	//		else
	//			return MeshManagerInstance().ReplaceMesh<AnimatedVertex>(meshID, sizeVerts, sizeIndices, verts, indices);
	//	}
	//
	//	int DeleteMesh( int meshID )
	//	{
	//		return 0;
	//	}
	//	
	//	int LoadTexture(unsigned int& id, const char* path, unsigned char* data, int width, int height, bool replace)
	//	{
	//		TextureManagerInstance().LoadTexture(id, path, data, width, height, false, replace);
	//		return GFX_SUCCESS;
	//	}
	//
	//	int LoadCubemap(unsigned int& id, const char* path, unsigned char* posX, unsigned char* negX, unsigned char* posY, unsigned char* negY, unsigned char* posZ, unsigned char* negZ, int width, int height)
	//	{
	//		TextureManagerInstance().LoadCubemap(id, path, posX, negX, posY, negY, posZ, negZ, width, height);
	//		return GFX_SUCCESS;
	//	}
	//
	//	int DeleteTexture(unsigned int id)
	//	{
	//		TextureManagerInstance().DeleteTexture(id);
	//		return GFX_SUCCESS;
	//	}
	//
	//	int CreateMaterial(unsigned long long int& id)
	//	{
	//		MaterialManagerInstance().CreateMaterial(id);
	//		return GFX_SUCCESS;
	//	}
	//	
	//	int DeleteMaterial(unsigned long long int& id)
	//	{
	//		MaterialManagerInstance().DeleteMaterial(id);
	//		return GFX_SUCCESS;
	//	}
	//
	//	int SetMaterialTexture(const unsigned long long int& materialID, const unsigned long long int& textureID, TextureType type)
	//	{
	//		MaterialManagerInstance().SetTexture(materialID, textureID, type);
	//		return GFX_SUCCESS;
	//	}
	//
	//	int GetShaderID(unsigned int& shaderID, const char* shaderName)
	//	{
	//		MaterialManagerInstance().GetShaderID(shaderID, shaderName);
	//		return GFX_SUCCESS;
	//	}
	//
	//	int LoadFont(Font &font, FontSize size, const char* path)
	//	{
	//		Font ret_font = FontSystem::LoadFont(std::string(path), size);
	//		if (ret_font)
	//		{
	//			font = ret_font;
	//			return GFX_SUCCESS;
	//		}
	//		else
	//		{
	//			return GFX_FAILURE;
	//		}
	//	}
	//
	//	int DeleteFont(const Font& font)
	//	{
	//		return FontSystem::UnloadFont(font) ? GFX_SUCCESS : GFX_FAILURE;
	//	}
	//
	//}
	namespace debug
	{
	//	void DrawString(const char* text, int x, int y, glm::vec4 color)
	//	{
	//		float ncolor[4] = {color.r, color.g, color.b, color.a};
	//		debugManagerQueuePointer->AddText(std::string(text), x, y, ncolor);
	//	}
	
		void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec4 color)
		{
		}
	
	//	void DrawSphere(glm::vec3 position, float radius, glm::vec4 color)
	//	{
	//		float ncolor[4] = { color.r, color.g, color.b, color.a };
	//		float nposition[3] = { position.x, position.y, position.z };
	//		debugManagerQueuePointer->AddSphere(nposition, radius, ncolor);
	//	}
	//
	//	void DrawAABB(glm::vec3 min, glm::vec3 max, glm::vec4 color)
	//	{
	//		float ncolor[4] = { color.r, color.g, color.b, color.a };
	//		float nmin[3] = { min.x, min.y, min.z };
	//		float nmax[3] = { max.x, max.y, max.z };
	//		debugManagerQueuePointer->AddAABB(nmin, nmax, ncolor);
	//	}
	//
	//	void SwapDebugBuffers()
	//	{
	//		DebugManager* temp = debugManagerRenderPointer;
	//		debugManagerRenderPointer = debugManagerQueuePointer;
	//		debugManagerQueuePointer = temp;
	//	}		
	//
	//	//void DrawOBB(glm::vec3 min, glm::vec3 max, glm::quat rotation, glm::vec4 color)
	//	//{
	//	//	float ncolor[4] = { color.r, color.g, color.b, color.a };
	//	//	float nrotation[4] = { rotation.w, rotation.x, rotation.y, rotation.z };
	//	//	float nmin[3] = { min.x, min.y, min.z };
	//	//	float nmax[3] = { max.x, max.y, max.z };
	//	//	DebugManager::AddOBB(nmin, nmax, nrotation, ncolor);
	//	//}
	}
}