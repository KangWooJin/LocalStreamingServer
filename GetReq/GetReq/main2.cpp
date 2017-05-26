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
// ������ ������ �ٿ�ε�
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

	// WinINet�Լ� �ʱ�ȭ 
	if ((hInet = InternetOpen(L"MyWeb",            // user agent in the HTTP protocol
		INTERNET_OPEN_TYPE_DIRECT,    // AccessType
		NULL,                        // ProxyName
		NULL,                        // ProxyBypass
		0)) != NULL)                // Options
	{

		

		// �Էµ� HTTP�ּҸ� ����
		if ((hUrl = InternetOpenUrl(hInet,        // ���ͳ� ������ �ڵ�
			ptrCharToLPWSTR(pszUrl),                        // URL
			NULL,                        // HTTP server�� ������ �ش�
			0,                            // �ش� ������
			0,                            // Flag
			0)) != NULL)                // Context
		{
			FILE    *fp;
			// �ٿ�ε��� ���� �����
			if ((fp = fopen(pszFile, "wb")) != NULL)
			{
				TCHAR    szBuff[READ_BUF_SIZE];
				DWORD    dwSize;
				DWORD    dwDebug = 10;
				do {
					// ������ ���� �б�
					InternetReadFile(hUrl, szBuff, READ_BUF_SIZE, &dwSize);
					// ������ ������ ������� ���Ͽ� ��ֱ�
					fwrite(szBuff, dwSize, 1, fp);
					dwReadSize += dwSize;
				} while ((dwSize != 0) || (--dwDebug != 0));
				fclose(fp);
			}
			// ���ͳ� �ڵ� �ݱ�
			InternetCloseHandle(hUrl);
		}
		// ���ͳ� �ڵ� �ݱ�
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


//��ó: http ://wirex.tistory.com/entry/WinAPIWinInet-Library��-����Ͽ�-��������-����-�ҷ�����-���� [Reload the Life...]