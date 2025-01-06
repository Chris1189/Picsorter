#include "ps_smb.h"

void ps_samba_start(){
  smbc_init(ps_auth_fn, 0);

}

void ps_auth_fn(const char *server, const char *share,
                char *workgroup, int wglen, char *username,
                int unlen, char *password, int pwlen) {
  printf("Enter username: ");
  fgets(username, unlen, stdin);
  username[strcspn(username, "\n")] = 0;

  printf("Enter password: ");
  #ifdef _WIN32
      // Windows-specific code
      HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
      DWORD mode = 0;
      GetConsoleMode(hStdin, &mode);
      SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
      fgets(password, pwlen, stdin);
      SetConsoleMode(hStdin, mode);
  #else
      // Unix-specific code
      system("stty -echo");
      fgets(password, pwlen, stdin);
      system("stty echo");
  #endif
  // Remove newline character if present
  password[strcspn(password, "\n")] = 0;
}
