#include "ps_fs.h"

void usage() { printf("Usage: picsorter [-r] <filepath>"); }

int main(int argc, char **argv) {
  struct dirent *file;
  int rec;
  int start;

  if (argc <= 1) {
    usage();
  }
  else if (argc == 2) {
    if (strstr(argv[1],"-r") != NULL){
      usage();
    }
    rec = 0;
    dir_initialize(argv[1], rec);
  }
  else{
    if(strstr(argv[1], "-r") != NULL){
      rec = 1;
      start = 2;
    }else{
      rec = 0;
      start = 1;
    }
    for(int i = start; i <= argc; i++){
      dir_initialize(argv[i], rec);
    }
  }
}
