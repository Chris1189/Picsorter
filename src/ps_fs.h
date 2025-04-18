#ifndef PS_FS_H
#define PS_FS_H
#include "ps_common.h"
#include <dirent.h>
#include <sys/stat.h>

void dir_initialize (const char *path, int rec, const char *base_dir);
void ps_scandir (const char *p, DIR *d, int rec, const char *base_dir);
void ps_rename (const char *p, struct dirent *file, int rec,
                const char *base_dir);
int ps_create (const char *p, int y, int m, int d);
int ps_test_naming (const char *p, char *buf);

#endif
