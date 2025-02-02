#include "ps_smb.h"
#include <samba-4.0/libsmbclient.h>
#include <stdio.h>

void ps_samba_start() {
  SMBCCTX *ctx;
  int debug = 0;

  ctx = smbc_new_context();

  if (!ctx) {
    perror("No SMBCCTX ctx created");
  }
  smbc_setDebug(ctx, debug);

  if (!smbc_init_context(ctx)) {
    smbc_free_context(ctx, 0);
    printf("Could not initialize smbc context\n");
  }
  smbc_setFunctionAuthData(ctx, get_auth_data_fn);
  smbc_set_context(ctx);
}

void list_dir(const char *path) {
  char buf[1024];
  int scan = 0;
  char *p;

  snprintf(buf, sizeof(buf), "smb://%s", path);

  if ((p = strchr(buf, '\n')) != NULL) {
    *p = '\0';
  }

  browse(buf, scan, 0);
}

void browse(char *path, int scan, int indent) {
  char *p;
  char buf[1024];
  int dir;
  struct stat st;
  struct smbc_dirent *dirent;

  if (!scan) {
    printf("Opening (%s)...\n", path);
  }

  if ((dir = smbc_opendir(path)) < 0) {
    printf("Could not open directory [%s] (%d:%s)\n", path, errno,
           strerror(errno));
    return;
  }

  while ((dirent = smbc_readdir(dir)) != NULL) {
    printf("%*.*s%-30s", indent, indent, "", dirent->name);

    switch (dirent->smbc_type) {
    case SMBC_WORKGROUP:
      printf("WORKGROUP");
      break;

    case SMBC_SERVER:
      printf("SERVER");
      break;

    case SMBC_FILE_SHARE:
      printf("FILE_SHARE");
      break;

    case SMBC_PRINTER_SHARE:
      printf("PRINTER_SHARE");
      break;

    case SMBC_COMMS_SHARE:
      printf("COMMS_SHARE");
      break;

    case SMBC_IPC_SHARE:
      printf("IPC_SHARE");
      break;

    case SMBC_DIR:
      printf("DIR");
      break;

    case SMBC_FILE:
      printf("FILE");
      break;

    case SMBC_LINK:
      printf("LINK");
      break;
    }

    printf("\n");

    if (scan && (dirent->smbc_type == SMBC_WORKGROUP ||
                 dirent->smbc_type == SMBC_SERVER)) {
      snprintf(buf, sizeof(buf), "smb://%s", dirent->name);
      browse(buf, scan, indent + 2);
    }
  }

  smbc_closedir(dir);
}

void get_auth_data_fn(const char *pServer, const char *pShare, char *pWorkgroup,
                      int maxLenWorkgroup, char *pUsername, int maxLenUsername,
                      char *pPassword, int maxLenPassword) {
  char temp[128];
  char server[256] = {'\0'};
  char share[256] = {'\0'};
  char workgroup[256] = {'\0'};
  char username[256] = {'\0'};
  char password[256] = {'\0'};

  static int krb5_set = 1;

  if (strcmp(server, pServer) == 0 && strcmp(share, pShare) == 0 &&
      *workgroup != '\0' && *username != '\0') {
    strncpy(pWorkgroup, workgroup, maxLenWorkgroup - 1);
    strncpy(pUsername, username, maxLenUsername - 1);
    strncpy(pPassword, password, maxLenPassword - 1);
    return;
  }

  if (krb5_set && getenv("KRB5CCNAME")) {
    krb5_set = 0;
    return;
  }

  fprintf(stdout, "Workgroup: [%s] ", pWorkgroup);
  fgets(temp, sizeof(temp), stdin);

  if (temp[strlen(temp) - 1] == '\n') /* A new line? */
  {
    temp[strlen(temp) - 1] = '\0';
  }

  if (temp[0] != '\0') {
    strncpy(pWorkgroup, temp, maxLenWorkgroup - 1);
  }

  fprintf(stdout, "Username: [%s] ", pUsername);
  fgets(temp, sizeof(temp), stdin);

  if (temp[strlen(temp) - 1] == '\n') /* A new line? */
  {
    temp[strlen(temp) - 1] = '\0';
  }

  if (temp[0] != '\0') {
    strncpy(pUsername, temp, maxLenUsername - 1);
  }

  fprintf(stdout, "Password: ");
  fgets(temp, sizeof(temp), stdin);

  if (temp[strlen(temp) - 1] == '\n') /* A new line? */
  {
    temp[strlen(temp) - 1] = '\0';
  }

  if (temp[0] != '\0') {
    strncpy(pPassword, temp, maxLenPassword - 1);
  }

  strncpy(workgroup, pWorkgroup, sizeof(workgroup) - 1);
  strncpy(username, pUsername, sizeof(username) - 1);
  strncpy(password, pPassword, sizeof(password) - 1);

  krb5_set = 1;
}
