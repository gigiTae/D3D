#pragma once


namespace RendererModule
{
	/// 파일읽기를 지원하는 헤더 

	namespace File
	{
		/// <summary>
	    /// 파일경로 내부에 모든 파일의 경로를 벡터에 반환 
	    /// </summary>
	    /// <param name="_files">파일이름을 저장할 벡터</param>
	    /// <param name="_filePath">파일경로</param>
	    /// <param name="_allDirectories">파일내부의 파일까지 읽는지</param>
		void GetFileMemberPath(std::vector<std::wstring>& _files, const std::wstring& _filePath, bool _allDirectories);

		/// <summary>
		///  파일 경로 내부에 모든 파일의 이름을 벡터에 반환
		/// </summary>
		/// <param name="_files">파일이름을 저장할 벡터</param>
		/// <param name="_filePath">파일경로</param>
		/// <param name="_allDirectories">파일내부의 파일까지 읽는지</param>
		void GetFileMemberName(std::vector<std::wstring>& _files, const std::wstring& _filePath, bool _allDirectories);


		/// 현재 파일경로를 반환
		const std::wstring& GetCurrentFilePath();

		/// 파일 확장자 반환  
		std::wstring GetFileExtension(const std::wstring& filePath);

		/// 파일경로에서 파일이름 반환  (Key 값 만들기) 
		std::wstring GetFileName(const std::wstring& filePath);

	};





}

