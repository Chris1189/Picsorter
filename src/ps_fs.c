#include "ps_fs.h"
#include <stdio.h>
#include <string.h>

void dir_initialize(const char *p) {
  DIR *d;
  struct dirent *file;
  struct stat *buf;

  d = opendir(p);

  if (d != NULL) {
    do {
      char path[256];
      file = readdir(d);
      if (file){
        snprintf(path, sizeof(path), "%s/%s", p, file->d_name);
        printf("Transforming %s\n", path);
      }
      else
        break;
    }
    while (file);

    closedir(d);
  }
}
