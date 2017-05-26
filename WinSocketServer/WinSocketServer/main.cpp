
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <tchar.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <wininet.h>

#include <string>
#include <time.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#pragma comment(lib, "wininet.lib") 
// #pragma comment (lib, "Mswsock.lib")
using namespace std;

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "80"
#define AGENT_NAME    L"TEST APP"
#define READ_BUF_SIZE    1024*512



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
					//mp4.push_back(szBuff);
					
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

void HttpPutFile(LPCTSTR wszServerURL, LPCTSTR wszRemoteFilePath,
	LPCTSTR wszLocalFilePath, int nPort,
	LPCTSTR wszLoginUserID, LPCTSTR wszLoginPassword)
{
	/*
	TCHAR wszServerURL[100]         = L"solergy.com";
	TCHAR wszLocalFilePath[100]     = L"\Program Files\hanaro\Sign_AS\7250.bmp";
	TCHAR wszRemoteFilePath[100]  = L"/hanaro/sign/as/7250.bmp";
	TCHAR wszLoginUserID[100]       = L"";
	TCHAR wszLoginPassword[100]  = L"";
	TCHAR nPort                              = 80;
	*/
	HINTERNET hInternet, hURL, hRequest;

	// 연결
	hInternet = InternetOpen(L"HTTPFILE", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
	{
		MessageBox(NULL, L"인터넷이 연결되어 있지 않습니다.", L"Warning", MB_OK);
		return;
	}

	// url
	hURL = InternetConnect(hInternet, wszServerURL, nPort,
		wszLoginUserID, wszLoginPassword,
		INTERNET_SERVICE_HTTP, 0, 0);
	if (hURL == NULL)
	{
		InternetCloseHandle(hInternet);
		MessageBox(NULL, L"서버오류로 연결할 수 없습니다.", L"Warning", MB_OK);
		return;
	}

	INTERNET_BUFFERS        iBuf = { 0 };
	iBuf.dwStructSize = sizeof(INTERNET_BUFFERS);

	hRequest = HttpOpenRequest(hURL, L"PUT", wszRemoteFilePath, NULL, NULL, NULL, 0, 0);
	if (!hRequest)
	{
		InternetCloseHandle(hURL);
		InternetCloseHandle(hInternet);
		MessageBox(NULL, L"Request open Failed", L"Warning", MB_OK);
		return;
	}

	//  local file 오픈
	HANDLE hFile;
	hFile = CreateFile(wszLocalFilePath, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"Invalid read File", L"warning", MB_OK);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hInternet);
		InternetCloseHandle(hURL);
		return;
	}


	DWORD dwWritten;
	DWORD dwFileSize, dwFilePos = 0, dwRead;

	dwFileSize = GetFileSize(hFile, NULL);
	iBuf.dwBufferTotal = dwFileSize;

	if (!HttpSendRequestEx(hRequest, &iBuf, NULL, HSR_INITIATE, 0))
	{
		MessageBox(NULL, L"SendRequest Failed", L"warning", MB_OK);
		CloseHandle(hFile);
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hInternet);
		InternetCloseHandle(hURL);
		return;
	}

	char buf[512];
	DWORD cnt = 0;
	dwFilePos = 0;
	do
	{
		SetFilePointer(hFile, dwFilePos, NULL, FILE_BEGIN);
		if (!ReadFile(hFile, buf, sizeof(buf), &dwRead, NULL))
		{
			MessageBox(NULL, L"Local File read error", L"Warning", MB_OK);
			break;
		};

		InternetSetFilePointer(hRequest, dwFilePos, NULL, FILE_BEGIN, 0);
		if (!InternetWriteFile(hRequest, buf, dwRead, &dwWritten))
		{
			MessageBox(NULL, L"Remote File write error", L"Warning", MB_OK);
			break;
		};
		dwFilePos += dwWritten;

	} while (dwRead == sizeof(buf));


	CloseHandle(hFile);

	if (!HttpEndRequest(hRequest, NULL, 0, 0))
	{
		MessageBox(NULL, L"EndRequest Failed", L"warning", MB_OK);
	}

	InternetCloseHandle(hRequest);
	InternetCloseHandle(hInternet);
	InternetCloseHandle(hURL);

}




/*
int _tmain(int argc, _TCHAR* argv[])
{
	getFileFromHttp("http://10.83.32.215/mv_720_2500.mp4", "test.mp4");

	return 0;
}
*/

string* StringSplit(string strTarget, string strTok, int& nIndex)
{
	int     nCutPos;
	   nIndex = 0;
	string* strResult = new string[100];
	while ((nCutPos = strTarget.find_first_of(strTok)) != strTarget.npos)
	{
		if (nCutPos > 0)
		{
			for (int i = 0; i < nCutPos - 1; ++i)
			{
				if (strTarget[i] != -52)
				{
					strResult[nIndex++] = strTarget.substr(i, nCutPos - 1);
					break;
				}
			}
		}
		strTarget = strTarget.substr(nCutPos + 1);
	}
	int size = strTarget.length();
	if (size > 0 )
	{
		for (int i = 0; i < strTarget.length(); ++i)
		{
			if (strTarget[i] != -52)
			{
				strResult[nIndex++] = strTarget.substr(i, size+1);

			}
		}
		//strResult[nIndex++] = strTarget.substr(0, nCutPos);
	}

	cout << nIndex << endl;
	return strResult;
}

string getSendHeader(int start, int end)
{
	/*파일을 읽어옴*/
	time_t   current_time;
	time(&current_time);
	string currTime[5];
	int idx = 0;

	for (int i = 0; i < 5; ++i)
		currTime[i] = "";

	int len = strlen(ctime(&current_time));
	for (int i = 0; i < len; ++i)
	{
		if (ctime(&current_time)[i] == ' ')
		{
			idx++;
			continue;
		}
		currTime[idx] += ctime(&current_time)[i];
	}

	string sendData = "HTTP/1.1 206 Partial Content\r\n Last-Modified: ";
	sendData += currTime[0] + ", ";
	sendData += currTime[2] + " ";
	sendData += currTime[1] + " ";
	sendData += currTime[4] + " ";
	sendData += currTime[3] + " ";
	sendData += "GMT\r\n Content-Range: bytes ";
	sendData += to_string(start) + " - " + to_string(end) + "/82749\r\n Content - Length:" + to_string(end - start) + "\r\n Content - Type: video / mp4\r\n Accept-Ranges: bytes \r\n";

	cout << sendData << endl;
	
	return sendData;
}

int __cdecl main(void)
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// No longer need server socket
	closesocket(ListenSocket);

	int start = 0;
	int end = start + READ_BUF_SIZE - 1;

	// Receive until the peer shuts down the connection
	while (1)
	{

		//while(iResult = recv(ClientSocket, recvbuf, recvbuflen, 0));
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		//int len = strlen(recvbuf);
		
		int index = 0;
		string * tok = StringSplit(recvbuf, "\n", index);
		for (int i = 0; i < index; ++i)
		{
			cout << tok[i] << endl;
		}
		//recvbuf[len] = '\0';

		

		//스레드
		//getFileFromHttp("http://10.83.32.215/mv_720_2500.mp4", "test.mp4");

		//if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			// Echo the buffer back to the sender
			
			
			
			FILE * pFile;
			char buffer[READ_BUF_SIZE];

			pFile = fopen("test.mp4", "r");

			fseek(pFile, start, SEEK_CUR);

			if (pFile == NULL) 
				perror("Error opening file");
			else
			{
				//while (!feof(pFile))
				//{
					if (fgets(buffer, READ_BUF_SIZE, pFile) == NULL) break;
					//fputs(buffer, stdout);
					string sData = getSendHeader(start, end);
					start = end;
					end = end + READ_BUF_SIZE;
					int len = strlen(buffer);
					for (int i = 0; i < len; ++i)
					{
						sData += buffer[i];
					}
					//cout << sData << endl;
					iSendResult = send(ClientSocket, sData.c_str(), sData.length(), 0);
					
					//iSendResult = send(ClientSocket, buffer, READ_BUF_SIZE, 0);
				//}
				fclose(pFile);
			}

			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);


		//}
		//else if (iResult == 0)
		//	printf("Connection closing...\n");
		//else  {
		//	printf("recv failed with error: %d\n", WSAGetLastError());
		//	closesocket(ClientSocket);
		//	WSACleanup();
		//	return 1;
		//}

	}  // break가 오거나 200 OK오는 경우 break;

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}