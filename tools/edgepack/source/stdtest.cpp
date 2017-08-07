#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine,
                     int nCmdShow)
{


  int hConHandle;
  long lStdHandle;
  FILE *fp;
  int iVar;

  // Try to attach to a console
  if (!AttachConsole (-1)) {
    MessageBox (NULL,"Could not attach to a console.\n"
                "Start program from a console.",
                "Message Box",MB_OK);
    ExitProcess (1);
  }

  // redirect unbuffered STDOUT to the console
  lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "w" );
  *stdout = *fp;
  setvbuf( stdout, NULL, _IONBF, 0 );

  // redirect unbuffered STDIN to the console
  lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "r" );
  *stdin = *fp;
  setvbuf( stdin, NULL, _IONBF, 0 );

  // redirect unbuffered STDERR to the console
  lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
  hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
  fp = _fdopen( hConHandle, "w" );
  *stderr = *fp;
  setvbuf( stderr, NULL, _IONBF, 0 );

  // test stdio
  fprintf(stdout, "Test output to stdout\n");
  fprintf(stderr, "Test output to stderr\n");
  fprintf(stdout, "Enter an integer to test stdin: ");
  fscanf(stdin,"%d", &iVar);
  printf("You entered %d\n", iVar);
  getch();


  return 0;

}
