#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <cstring>

#pragma comment(lib, "wininet.lib")

int main()
{
    int choice;

    std::cout << "Select request type:\n";
    std::cout << "1. GET request\n";
    std::cout << "2. POST request\n";
    std::cout << "Choice: ";
    std::cin >> choice;

    // ---------------------------------------------------------
    // Initialize WinINet
    // ---------------------------------------------------------

    HINTERNET hInternet = InternetOpenA(
        "MyWinInetClient/1.0",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0
    );

    if (!hInternet)
    {
        std::cerr << "InternetOpenA failed: "
                  << GetLastError() << '\n';
        return 1;
    }

    const char* host;
    const char* path;

    // ---------------------------------------------------------
    // Configure request
    // ---------------------------------------------------------

    if (choice == 1)
    {
        // GET Google
        host = "www.google.com";
        path = "/";
    }
    else if (choice == 2)
    {
        // POST to httpbin
        host = "httpbin.org";
        path = "/post";
    }
    else
    {
        std::cerr << "Invalid choice.\n";
        InternetCloseHandle(hInternet);
        return 1;
    }

    // ---------------------------------------------------------
    // Connect to server
    // ---------------------------------------------------------

    HINTERNET hConnect = InternetConnectA(
        hInternet,
        host,
        INTERNET_DEFAULT_HTTPS_PORT,
        NULL,
        NULL,
        INTERNET_SERVICE_HTTP,
        0,
        0
    );

    if (!hConnect)
    {
        std::cerr << "InternetConnectA failed: "
                  << GetLastError() << '\n';

        InternetCloseHandle(hInternet);
        return 1;
    }

    // ---------------------------------------------------------
    // Create HTTP request
    // ---------------------------------------------------------

    const char* method = (choice == 1) ? "GET" : "POST";

    HINTERNET hRequest = HttpOpenRequestA(
        hConnect,
        method,
        path,
        NULL,
        NULL,
        NULL,
        INTERNET_FLAG_SECURE,
        0
    );

    if (!hRequest)
    {
        std::cerr << "HttpOpenRequestA failed: "
                  << GetLastError() << '\n';

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // ---------------------------------------------------------
    // Send request
    // ---------------------------------------------------------

    BOOL success;

    if (choice == 1)
    {
        // GET request
        success = HttpSendRequestA(
            hRequest,
            NULL,
            0,
            NULL,
            0
        );
    }
    else
    {
        // POST request
        const char* postData =
            "name=Heisenberg&message=Hello+from+WinINet";

        const char* headers =
            "Content-Type: application/x-www-form-urlencoded\r\n";

        success = HttpSendRequestA(
            hRequest,
            headers,
            -1L,
            (LPVOID)postData,
            static_cast<DWORD>(strlen(postData))
        );
    }

    if (!success)
    {
        std::cerr << "HttpSendRequestA failed: "
                  << GetLastError() << '\n';

        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return 1;
    }

    // ---------------------------------------------------------
    // Read response
    // ---------------------------------------------------------

    std::cout << "\n----- Server Response -----\n\n";

    char buffer[4096];
    DWORD bytesRead = 0;

    while (InternetReadFile(
        hRequest,
        buffer,
        sizeof(buffer),
        &bytesRead))
    {
        if (bytesRead == 0)
            break;

        std::cout.write(buffer, bytesRead);
    }

    std::cout << "\n\n---------------------------\n";

    // ---------------------------------------------------------
    // Cleanup
    // ---------------------------------------------------------

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return 0;
}
