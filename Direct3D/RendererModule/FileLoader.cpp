#include "RendererPCH.h"
#include "FileLoader.h"


void RendererModule::File::GetFileMemberPath(std::vector<std::wstring>& _files, const std::wstring& _filePath, bool _allDirectories)
{
	std::wstring tmp = _filePath + std::wstring(L"\\*.*");
	WIN32_FIND_DATA fd;

	HANDLE hFind = FindFirstFile(tmp.c_str(), &fd);

	if (INVALID_HANDLE_VALUE != hFind)
	{
		do {
			if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				/// 하위 폴더까지 읽는지
				if (_allDirectories)
				{
					if (fd.cFileName[0] != '.')
					{
						tmp = _filePath + std::wstring(L"\\") + std::wstring(fd.cFileName);
						GetFileMemberPath(_files, tmp, _allDirectories);
					}
				}
			}
			else
			{
				/// 벡터에 파일 경로를 저장한다.
				tmp = _filePath + std::wstring(L"\\") + std::wstring(fd.cFileName);
				_files.push_back(tmp);
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}

}

void RendererModule::File::GetFileMemberName(std::vector<std::wstring>& _files, const std::wstring& _filePath, bool _allDirectories)
{
	std::wstring tmp = _filePath + std::wstring(L"\\*.*");
	WIN32_FIND_DATA fd;

	HANDLE hFind = FindFirstFile(tmp.c_str(), &fd);

	if (INVALID_HANDLE_VALUE != hFind)
	{
		do {
			if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				/// 하위 폴더까지 읽는지
				if (_allDirectories)
				{
					if (fd.cFileName[0] != '.')
					{
						tmp = _filePath + std::wstring(L"\\") + std::wstring(fd.cFileName);
						GetFileMemberName(_files, tmp, _allDirectories);
					}
				}
			}
			else
			{
				/// 벡터에 파일 이름을 저장한다.
				tmp = std::wstring(fd.cFileName);
				_files.push_back(tmp);
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
}

const std::wstring& RendererModule::File::GetCurrentFilePath()
{
	static std::wstring filePath{};

	/// 파일 경로 불러오기
	if (filePath.size() == 0)
	{
		wchar_t relativePath[256]{};
		GetCurrentDirectoryW(255, relativePath);
		filePath = relativePath;

		while (filePath.back() != '\\')
		{
			filePath.pop_back();
		}
	}
	
	return filePath;
}

std::wstring RendererModule::File::GetFileExtension(const std::wstring& filePath)
{
	// 파일 확장자 가져오기  
	std::wstring extension = filePath.substr(filePath.find_last_of(TEXT(".")) + 1);

	assert(filePath.length() != extension.length() || !L"확장자를 찾기 못했습니다");

	// 대문자를 소문자로 변환 
	std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

	return extension; 
	// std::move를 사용하지 않아도 최적화가 적용 
	// C++ 컴파일러는 RVO(Return Value Optimization)와 NRVO(Named Return Value Optimization) 등의 
	// 최적화 기법을 사용하여 함수에서 생성한 지역 변수를 복사 없이 반환하려고 시도합니다. 
	// 따라서 std::move 를 사용하지 않아도 최적화가 적용되어 효율적으로 동장할 가능성이 높다. 
}

std::wstring RendererModule::File::GetFileName(const std::wstring& filePath)
{
	// 파일명 앞의 경로 제거 
	std::wstring name = filePath.substr(filePath.find_last_of(L'\\') +1);

	std::wstring tmp = name;
	// 확장자 제거  
	while (true)
	{
		if (name.size() == 0)
		{
			return tmp;
		}

		wchar_t c = name.back();
		name.pop_back();

		if (c == L'.')
			break;
	}

	return name;
}
