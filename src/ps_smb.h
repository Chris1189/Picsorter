#ifndef PS_SMB_H
#define PS_SMB_H

#include "ps_common.h"
#include <samba-4.0/libsmbclient.h>

void ps_auth_fn(const char *server, const char *share,
                char *workgroup, int wglen, char *username,
                int unlen, char *password, int pwlen) ;
void ps_samba_start();

#endif // PS_SMB_H
