#include "ps_fs.h"

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
      char check[256];
      char *extension;

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
        extension = strrchr(file->d_name, '.');

        snprintf(check, sizeof(check),"%d-%d-%d_%d:%d:%d_Image",
                time->tm_year+1900, time->tm_mon, time->tm_mday,
                 time->tm_hour, time->tm_min, time->tm_sec);

        if(strstr(path, check) == NULL && extension != NULL){
          snprintf(naming, sizeof(naming),"%s/%d-%d-%d_%d:%d:%d_Image%s",p,
                  time->tm_year+1900, time->tm_mon, time->tm_mday,
                  time->tm_hour, time->tm_min, time->tm_sec, extension);

          if(strstr(path, ".jpg") != NULL ||
          strstr(path, ".jpeg") != NULL ||
          strstr(path, ".png") != NULL ||
          strstr(path, ".tiff") != NULL){
            if (rename(path, naming) == 0){
              printf("Performed renaming successfully\n");
            }
            else{
              printf("Error while renaming.\n");
            }
          }
        }

        free(buf);
      }
      else
        break;
    }
    while (file);

    closedir(d);
  }
}
