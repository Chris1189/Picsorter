#ifndef PS_FS_H
#define PS_FS_H
#include "ps_common.h"
#include <dirent.h>
#include <sys/stat.h>

void dir_initialize(const char *path, int rec);
void ps_scandir(const char *p, DIR *d, int rec);
void ps_rename(const char *p, struct dirent *file);
int ps_create(const char *p, int y, int m, int d);
int ps_sort(const char *p, int y, int m, int d);

#endif
