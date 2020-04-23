#pragma once

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11_1.h>
#include <dinput.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "DDSTextureLoader.h"
using namespace DirectX;


#include "AlignedAllocationPolicy.h"
