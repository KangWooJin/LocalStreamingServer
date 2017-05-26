#include <Wininet.h>      //  ���� �Լ��� ������ �� ������ �ݵ�� �߰��ؾ� ��

#define READ_BUF_SIZE    1024

// ������ ������ �ٿ�ε�

int getFileFromHttp(char* pszUrl, char* pszFile)

{

	HINTERNET    hInet, hUrl;

	DWORD        dwReadSize = 0;


	// WinINet�Լ� �ʱ�ȭ 

	if ((hInet = InternetOpen("MyWeb",            // user agent in the HTTP protocol

		INTERNET_OPEN_TYPE_DIRECT,    // AccessType

		NULL,                        // ProxyName

		NULL,                        // ProxyBypass

		0)) != NULL)                // Options

	{

		// �Էµ� HTTP�ּҸ� ����

		if ((hUrl = InternetOpenUrl(hInet,        // ���ͳ� ������ �ڵ�

			pszUrl,                        // URL

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
