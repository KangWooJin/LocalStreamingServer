#include <Wininet.h>      //  밑의 함수를 쓰려면 이 라인을 반드시 추가해야 됨

#define READ_BUF_SIZE    1024

// 웹상의 파일을 다운로드

int getFileFromHttp(char* pszUrl, char* pszFile)

{

	HINTERNET    hInet, hUrl;

	DWORD        dwReadSize = 0;


	// WinINet함수 초기화 

	if ((hInet = InternetOpen("MyWeb",            // user agent in the HTTP protocol

		INTERNET_OPEN_TYPE_DIRECT,    // AccessType

		NULL,                        // ProxyName

		NULL,                        // ProxyBypass

		0)) != NULL)                // Options

	{

		// 입력된 HTTP주소를 열기

		if ((hUrl = InternetOpenUrl(hInet,        // 인터넷 세션의 핸들

			pszUrl,                        // URL

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
