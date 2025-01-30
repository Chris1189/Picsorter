#ifndef PS_SMB_H
#define PS_SMB_H

#define BUFFERSIZE 4096
#define STRINGSIZE 1024

#include "ps_common.h"
#include <samba-4.0/libsmbclient.h>

void get_auth_data_fn (const char *server, const char *share, char *workgroup,
                       int wglen, char *username, int unlen, char *password,
                       int pwlen);

void browse (char *path, int scan, int indent);
void list_dir (const char *path);
void ps_samba_start ();

int test_dir (char *dst_url);
int ps_copy_file (const char *file, const char *dest);
int ps_create_smb (const char *share, int y, int m, int d);

#endif // PS_SMB_H
