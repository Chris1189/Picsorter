#include "ps_fs.h"

void usage() { printf("Usage: picsorter <filepath>"); }

int main(int argc, char **argv) {
  struct dirent *file;

  if (argc <= 1) {
    usage();
  }
  if (argc == 2) {
    dir_initialize(argv[1]);
  }
}
