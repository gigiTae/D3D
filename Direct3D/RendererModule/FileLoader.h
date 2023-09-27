#pragma once


namespace GrapicsEngine
{
	/// �����б⸦ �����ϴ� ��� 

	namespace File
	{
		/// <summary>
	    /// ���ϰ�� ���ο� ��� ������ ��θ� ���Ϳ� ��ȯ 
	    /// </summary>
	    /// <param name="_files">�����̸��� ������ ����</param>
	    /// <param name="_filePath">���ϰ��</param>
	    /// <param name="_allDirectories">���ϳ����� ���ϱ��� �д���</param>
		void GetFileMemberPath(std::vector<std::wstring>& _files, const std::wstring& _filePath, bool _allDirectories);

		/// <summary>
		///  ���� ��� ���ο� ��� ������ �̸��� ���Ϳ� ��ȯ
		/// </summary>
		/// <param name="_files">�����̸��� ������ ����</param>
		/// <param name="_filePath">���ϰ��</param>
		/// <param name="_allDirectories">���ϳ����� ���ϱ��� �д���</param>
		void GetFileMemberName(std::vector<std::wstring>& _files, const std::wstring& _filePath, bool _allDirectories);


		/// ���ϰ�θ� ��ȯ
		const std::wstring& GetCurrentFilePath();

		/// ���� Ȯ���� ��ȯ  
		wstring GetFileExtension(const wstring& filePath);

		/// ���ϰ�ο��� �����̸� ��ȯ  (Key �� �����) 
		wstring GetFileName(const wstring& filePath);

	};





}

