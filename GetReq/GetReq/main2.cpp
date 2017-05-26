#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <tchar.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib") 

#define AGENT_NAME    L"TEST APP"

#define READ_BUF_SIZE    1024

#define URL L"http://10.83.32.215/mv_720_2500.mp4"
// 웹상의 파일을 다운로드
LPWSTR ptrCharToLPWSTR(char* pszUrl)
{
	/*char* to LPWSTR */
	wchar_t wtext[1024];
	mbstowcs(wtext, pszUrl, strlen(pszUrl) + 1);//Plus null
	LPWSTR url = wtext;

	return url;

}
int getFileFromHttp(char* pszUrl, char* pszFile)
{
	HINTERNET    hInet, hUrl;
	DWORD        dwReadSize = 0;

	// WinINet함수 초기화 
	if ((hInet = InternetOpen(L"MyWeb",            // user agent in the HTTP protocol
		INTERNET_OPEN_TYPE_DIRECT,    // AccessType
		NULL,                        // ProxyName
		NULL,                        // ProxyBypass
		0)) != NULL)                // Options
	{

		

		// 입력된 HTTP주소를 열기
		if ((hUrl = InternetOpenUrl(hInet,        // 인터넷 세션의 핸들
			ptrCharToLPWSTR(pszUrl),                        // URL
			NULL,                        // HTTP server에 보내는 해더
			0,                            // 해더 사이즈
			0,                            // Flag
			0)) != NULL)                // Context
		{
			FILE    *fp;
			// 다운로드할 파일 만들기
			if ((fp = fopen(pszFile, "wb")) != NULL)
			{
				TCHAR    szBuff[READ_BUF_SIZE];
				DWORD    dwSize;
				DWORD    dwDebug = 10;
				do {
					// 웹상의 파일 읽기
					InternetReadFile(hUrl, szBuff, READ_BUF_SIZE, &dwSize);
					// 웹상의 파일을 만들어진 파일에 써넣기
					fwrite(szBuff, dwSize, 1, fp);
					dwReadSize += dwSize;
				} while ((dwSize != 0) || (--dwDebug != 0));
				fclose(fp);
			}
			// 인터넷 핸들 닫기
			InternetCloseHandle(hUrl);
		}
		// 인터넷 핸들 닫기
		InternetCloseHandle(hInet);
	}

	return(dwReadSize);

}

void setHttpFromFile(char* pszUrl, char* pszFile)
{
	/*
	HINTERNET hRequest;
	DWORD  dwBytesWritten;
	char  pBuffer[READ_BUF_SIZE];
	InternetWriteFile(hRequest, pBuffer, READ_BUF_SIZE, &dwBytesWritten);
	*/

}



BOOL connectHTTP(const WCHAR *url)
{
	HINTERNET hInternet = NULL;
	HINTERNET hURL = NULL;

	DWORD ReadSize;
	BOOL bResult = TRUE;
	char buffer[4096] = { 0, };

	//-----------------
	// InternetOpen()
	hInternet = InternetOpen(AGENT_NAME,
		INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0);
	if (hInternet == NULL)
		return FALSE;

	//---------------------
	// InternetOpenUrl()
	hURL = InternetOpenUrl(hInternet,
		url,
		NULL,
		0,
		INTERNET_FLAG_RELOAD,
		0);
	if (hURL == NULL)
	{
		InternetCloseHandle(hInternet);
		return FALSE;
	}

	//-----------------------------------------
	// actually, receive the data right here...
	while (bResult)
	{
		bResult = InternetReadFile(hURL,
			buffer,
			4096,
			&ReadSize);

		printf("%s\n", buffer);
		memset(buffer, 0x0, sizeof(buffer));

		if (ReadSize == 0) break;
	}

	//-----------------
	// close handles..
	InternetCloseHandle(hURL);
	InternetCloseHandle(hInternet);

	return TRUE;

}


int _tmain(int argc, _TCHAR* argv[])
{
	getFileFromHttp("http://10.83.32.215/mv_720_2500.mp4", "test.mp4");

	return 0;
}


//출처: http ://wirex.tistory.com/entry/WinAPIWinInet-Library를-사용하여-웹페이지-파일-불러오기-예제 [Reload the Life...]