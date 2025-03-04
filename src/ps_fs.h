#ifndef PS_FS_H
#define PS_FS_H
#include "ps_common.h"
#include <dirent.h>
#include <sys/stat.h>

typedef struct dirent dirent;
typedef struct stat statf;
typedef struct tm tm;

void dir_initialize (const char *path, int rec, const char *base_dir);
void perform_dir_recursive (char *dest, const char *path, dirent *file,
                            const char *base, int rec);
void get_stats (const char *path, statf *buf, tm *time, char *extension,
                dirent *file);
void prepare_string (char *dst, const char *str, const char *path, int y,
                     int m, int d, int h, int min, int s, dirent *file,
                     const char *regex, const char *ext);
void ps_scandir (const char *p, DIR *d, int rec, const char *base_dir);
void ps_rename (const char *p, dirent *file, int rec, const char *base_dir);
int ps_create (const char *p, int y, int m, int d);
int ps_test_naming (const char *p, char *buf);

#endif
