#ifndef PS_SMB_H
#define PS_SMB_H

#include "ps_common.h"
#include <samba-4.0/libsmbclient.h>

void get_auth_data_fn (const char *server, const char *share, char *workgroup,
                       int wglen, char *username, int unlen, char *password,
                       int pwlen);

void browse (char *path, int scan, int indent);
void list_dir (const char *path);
void ps_samba_start ();

#endif // PS_SMB_H
