#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

void usage() { printf("Usage: picsorter <filepath>"); }

int main(int argc, char **argv) {
  DIR *dir;
  char *path;
  int count = 1;
  struct dirent *file;

  if (argc <= 1) {
    usage();
  }
  if (argc == 2) {
    path = malloc(sizeof(*(argv + 1)));

    if (path)
      path = *(argv + 1);

    dir = opendir(path);

    if (dir) {
      file = readdir(dir);

      while (file) {
        printf("Transforming: %s\n", count++, file->d_name);
        file = readdir(dir);
      }
      closedir(dir);
    }
  }
}
