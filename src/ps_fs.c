#include "ps_fs.h"
#include <stdio.h>
#include <string.h>

void dir_initialize(const char *p) {
  DIR *d;
  struct dirent *file;
  struct stat *buf;
  struct tm *time;

  d = opendir(p);

  if (d != NULL) {
    do {
      char path[256];
      char naming[256];

      file = readdir(d);
      if (file){
        if (strcmp(file->d_name, ".") == 0 ||
        strcmp(file->d_name, "..") == 0){
          continue;
        }

        snprintf(path, sizeof(path), "%s/%s", p, file->d_name);
        printf("Transforming %s\n", path);

        buf = malloc(sizeof(struct stat));
        lstat(path, buf);
        time = localtime(&buf->st_mtime);

        snprintf(naming, sizeof(naming),"%d-%d-%d %s\n", time->tm_year+1900,
                 time->tm_mon, time->tm_mday, file->d_name);

        free(buf);
      }
      else
        break;
    }
    while (file);

    closedir(d);
  }
}
