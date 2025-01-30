#include "ps_fs.h"
#include "ps_smb.h"

void usage() { printf("Usage: picsorter [-r] <filepath> [-s] <samba dir>"); }

int main(int argc, char **argv) {
  char *directory = NULL;
  char *share_path = NULL;
  int c, rec, smb;
  struct stat buf;

  while ((c = getopt(argc, argv, "r:s:")) != -1){
    switch (c) {
      case 'r':{
        rec = 1;
        directory = optarg;
        if (directory){
          dir_initialize(directory, rec, directory, smb);
        }
      }
        break;

      case 's':{
        if (!smb){
          ps_samba_start();
        }
        //list_dir(optarg);
        share_path = optarg;
        if (!test_dir(share_path))
          //TODO: Implement logic to copy to share
      }
        break;

      case '?':
        usage();
      break;

      default:{
        for (int i = optind; i < argc; i++){
          rec = 0;
          dir_initialize(argv[i], rec, argv[i], smb);
        }
      }
    }
    for (int i = optind; i < argc; i++) {
      if (rec){
        dir_initialize(argv[i], rec, argv[i], smb);
      }
    }
  }
}
