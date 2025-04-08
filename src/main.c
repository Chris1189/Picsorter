#include "ps_fs.h"
#include "ps_smb.h"

void
usage ()
{
  printf ("Usage: picsorter [-r] <filepath> [-s] <samba dir>");
}

int
main (int argc, char **argv)
{
  char *directory = NULL;
  char *share_path = NULL;
  int c, rec;
  int smb = 0;
  int debug = 0;
  struct stat buf;

  while ((c = getopt (argc, argv, "r:s:d")) != -1)
    {
      switch (c)
        {
        case 'r':
          {
            rec = 1;
            directory = optarg;
            if (directory)
              {
                dir_initialize (directory, rec, directory, smb, share_path);
              }
          }
          break;

        case 'd':
          debug = 1;
          break;

        case 's':
          {
            if (!smb)
              {
                ps_samba_start (debug);
              }
            share_path = optarg;
            if (test_dir (share_path))
              exit (1);
          }
          break;

        case '?':
          usage ();
          break;
        }
    }

  if (argc == optind)
    usage ();

  for (int i = optind; i < argc; i++)
    {
      rec = 0;
      dir_initialize (argv[i], rec, argv[i], smb, share_path);
    }
}
