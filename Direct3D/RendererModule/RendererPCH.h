
// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

// 여기에 미리 컴파일하려는 헤더 추가

#include <iostream>
#include <windows.h>
#include <assert.h>
#include <memory.h> // 스마트 포인터 
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr; //편하지만 지양해야한다. 4학기 엔진에서는 쓰지 않을 예정 

/// =============== D3D ========================= 
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <dxgi.h>
#include <DirectXPackedVector.h>


/// =============== 이펙트 라이브러리 설정 ====================
#include "../Resource/Lib/d3dx11effect.h"

#ifdef _DEBUG
#ifdef _WIN64 // x64 설정인 경우
#pragma comment( lib, "../Resource/Lib/64/Effects11d.lib" )
#else // x86 설정인 경우
#pragma comment( lib, "../Resource/Lib/86/Effects11d.lib" )
#endif
#else
#ifdef _WIN64 // x64 설정인 경우
#pragma comment( lib, "../Resource/Lib/64/Effects11.lib" )
#else // x86 설정인 경우
#pragma comment( lib, "../Resource/Lib/86/Effects11.lib" )
#endif
#endif
/// ==========================================================

/// =================== DirectXTex ===========================
#include "..\Resource\Lib\DirectXTex.h"
#include "..\Resource\Lib\DirectXTex.inl"

#ifdef _DEBUG
#ifdef _WIN64 // x64 설정인 경우
#pragma comment( lib, "../Resource/Lib/64/DirectXTexd.lib" )
#else // x86 설정인 경우
#pragma comment( lib, "../Resource/Lib/86/DirectXTexd.lib" )
#endif
#else
#ifdef _WIN64 // x64 설정인 경우
#pragma comment( lib, "../Resource/Lib/64/DirectXTex.lib" )
#else // x86 설정인 경우
#pragma comment( lib, "../Resource/Lib/86/DirectXTex.lib" )
#endif
#endif
/// ==========================================================

/// ====================== DirectXTK =========================
#include "..\Resource\Lib\SpriteBatch.h"
#include "..\Resource\Lib\SpriteFont.h"

#ifdef _DEBUG
#ifdef _WIN64 // x64 설정인 경우
#pragma comment( lib, "../Resource/Lib/64/DirectXTKd.lib" )
#else // x86 설정인 경우
#pragma comment( lib, "../Resource/Lib/86/DirectXTKd.lib" )
#endif
#else
#ifdef _WIN64 // x64 설정인 경우
#pragma comment( lib, "../Resource/Lib/64/DirectXTK.lib" )
#else // x86 설정인 경우
#pragma comment( lib, "../Resource/Lib/86/DirectXTK.lib" )
#endif
#endif

/// ==========================================================



#include "HRDefine.h"

#include "MathModule/MathHeader.h"
#include "../RendererModule/FL/DDSTextureLoader.h"
