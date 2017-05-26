void HttpPutFile(LPCTSTR wszServerURL, LPCTSTR wszRemoteFilePath,
	LPCTSTR wszLocalFilePath, TCHAR nPort,
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