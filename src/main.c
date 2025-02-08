#include "ps_fs.h"
#include "ps_smb.h"

void usage() { printf("Usage: picsorter [-r] <filepath> [-s] <samba dir>"); }

int main(int argc, char **argv) {
  char *directory = NULL;
  int c, rec, smb;

  while ((c = getopt(argc, argv, "r:s:")) != -1) {
    switch (c) {
    case 'r': {
      rec = 1;
      directory = optarg;
      if (directory) {
        dir_initialize(directory, rec, directory);
      }
    } break;

    case 's': {
      if (!smb) {
        ps_samba_start();
      }
      list_dir(optarg);
    } break;

    case '?':
      usage();
      break;
    }
  }

  if(argc == optind)
    usage();

  for (int i = optind; i < argc; i++) {
    rec = 0;
    dir_initialize(argv[i], rec, argv[i]);
  }
}
