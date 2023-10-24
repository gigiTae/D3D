#pragma once

#include <d3d11.h> // dx11 종속적

namespace ToolModule
{
class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	// 초기화 
	void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	// 해제
	void Finalize();
	void NewFrame();
	void EndRnder();
};
}