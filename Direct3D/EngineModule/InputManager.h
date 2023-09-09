#pragma once

struct KeyInfo
{
	KEY_STATE state;
	bool prevPush;
};

/// <summary>
/// �Է°���
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
	HWND m_hwnd; // ���� ������ �ڵ�
	std::vector<KeyInfo> m_keyInfo;	// Ű �������� ���� �迭
	const int m_matchVK[static_cast<int>(KEY::LAST)];// KEY ���� ����Ű���� ��Ī�����ִ� �迭
};

