#pragma once

struct KeyInfo
{
	KEY_STATE state;
	bool prevPush;
};

/// <summary>
/// 입력관리
/// </summary>
class InputManager
{
public:
	InputManager();
	~InputManager();

	void Initalize(HWND main);
	void Update();
	void Finalize();

	KEY_STATE GetKeyState(KEY _key)const { return m_keyInfo[static_cast<int>(_key)].state; }
	bool IsKeyState(const KEY& _key, const KEY_STATE& _state) const;

private:
	HWND m_hwnd; // 메인 윈도우 핸들
	std::vector<KeyInfo> m_keyInfo;	// 키 정보들을 담은 배열
	const int m_matchVK[static_cast<int>(KEY::LAST)];// KEY 값을 가상키값과 매칭시켜주는 배열
};

