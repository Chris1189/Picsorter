#ifndef PS_FS_H
#define PS_FS_H
#include "ps_common.h"
#include <dirent.h>

void path_free(const char *path);

char *path_new(const char *path);

void dir_initialize(const char *path);

#endif
