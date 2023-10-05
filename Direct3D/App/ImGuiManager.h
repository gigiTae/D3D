#pragma once

namespace Tool
{
	/// <summary>
	/// 아임구이 매니져 
	/// 
	/// </summary>
	class ImGuiManager
	{
	public:
		ImGuiManager();
		~ImGuiManager();

		void Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		void BeginRender();
		void EndRender(ID3D11DeviceContext* deviceContext);

	private:


	};


}