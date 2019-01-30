#pragma once

#include <Windows.h>
#include <assert.h>

//STL
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <functional>
#include <iterator>
using namespace std;


//Direct3D
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

//DirectXTex
#include <DirectXTex.h>
#pragma comment(lib, "directxtex.lib")

//Fbx SDK
#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk.lib")
using namespace fbxsdk;


#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p){ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }


const wstring Assets = L"../_Assets/";
const wstring Shaders = L"../_Shaders/";
const wstring Textures = L"../_Textures/";
const wstring Models = L"../_Models/";


#include "./Systems/D3D.h"
#include "./Systems/Keyboard.h"
#include "./Systems/Mouse.h"
#include "./Systems/Time.h"

#include "./Renders/VertexLayouts.h"
#include "./Renders/Shader.h"
#include "./Renders/ShaderBuffer.h"
#include "./Renders/GlobalBuffers.h"
#include "./Renders/RasterizerState.h"
#include "./Renders/Texture.h"
#include "./Renders/Material.h"

#include "./Executes/Execute.h"

#include "./Utilities/Math.h"
#include "./Utilities/String.h"
#include "./Utilities/Path.h"

#include "./Boundings/Ray.h"
#include "./Boundings/BBox.h"
#include "./Boundings/BSphere.h"

#include "./Draw/MeshCube.h"
#include "./Draw/MeshCylinder.h"
#include "./Draw/MeshGrid.h"
#include "./Draw/MeshSphere.h"

#include "./Draw/DebugLine.h"