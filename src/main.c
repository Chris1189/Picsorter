#include "ps_fs.h"

void usage() { printf("Usage: picsorter <filepath>"); }

int main(int argc, char **argv) {
  char *path;
  struct dirent *file;

  if (argc <= 1) {
    usage();
  }
  if (argc == 2) {
    path = path_new(*(argv + 1));
    dir_initialize(path);
  }
  path_free(path);
}
