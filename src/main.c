#include "ps_fs.h"

void usage() { printf("Usage: picsorter [-r] <filepath>"); }

int main(int argc, char **argv) {
  struct dirent *file;

  if (argc <= 1) {
    usage();
  }
  else if (argc == 2) {
    if (strstr(argv[1],"-r") != NULL){
      usage();
    }
    dir_initialize(argv[1]);
  }
  else if (argc == 3){
    dir_initialize(argv[2]);
    //TODO: In ps_fs.c the recursive logic has to be implemented!
  }
}
