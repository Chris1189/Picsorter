#include "ps_fs.h"

void path_free(const char *p) { free((void *)p); }

char *path_new(const char *p) {
  char *pd = malloc(sizeof(p));
  if (pd != NULL) {
    return pd;
  } else {
    return NULL;
  }
}

void dir_initialize(const char *p) {
  DIR *d;
  struct dirent *file;

  d = opendir(p);

  if (d != NULL) {
    file = readdir(d);
    while (file) {
      printf("Transforming: %s\n", file->d_name);
      file = readdir(d);
    }
    closedir(d);
  }
}
