#include "ps_fs.h"

void dir_initialize(const char *p, int rec,
                    const char *base_dir) {
  DIR *d;
  char *file_abs = malloc(sizeof(char)*256);
  char *base_abs = malloc(sizeof(char)*256);

  if (p && *p != '/'){
    file_abs = realpath(p, NULL)
      ? realpath(p, NULL):
      NULL;
  } else {
    strcpy(file_abs, p);
  }

  if (base_dir && *base_dir != '/') {
    base_abs = realpath(base_dir, NULL)
      ? realpath(base_dir, NULL)
      : NULL;
  } else {
    strcpy(base_abs, base_dir);
  }

  d = opendir(file_abs);

  if (d != NULL && file_abs && base_abs) {
    ps_scandir(file_abs, d, rec, base_abs);
  }
  free(file_abs);
  free(base_abs);
  closedir(d);
}

void ps_scandir(const char *p, DIR *d,
                int rec, const char *base){
  struct dirent *file;

  do {
    char *new_p;
    file = readdir(d);

    if(file == NULL)
      break;

    if(strcmp(file->d_name, ".") == 0 ||
      strcmp(file->d_name, "..") == 0){
      continue;
    }

    if(file->d_type == DT_DIR && rec){
      snprintf(new_p, sizeof(new_p),
               "%s/%s", p, file->d_name);
      dir_initialize(new_p, rec, base);
    }
    ps_rename(p, file, rec, base);
  }
  while (file);
}

void ps_rename(const char *p, struct dirent *file,
               int rec, const char *base_dir){
  int match;
  struct stat *buf;
  struct tm *time;
  char path[500];
  char naming[500];
  char *regmatch;
  char check[500];
  char *extension;
  char date[18] = "%d-%d-%d_%d:%d:%d";
  char path_name[33] = "%s/%d/%d/%d/%d-%d-%d_%d:%d:%d_%s";

  regmatch = malloc(sizeof(char)*256);
  snprintf(path, sizeof(path),
           "%s/%s", p, file->d_name);

  buf = malloc(sizeof(struct stat));
  lstat(path, buf);
  time = localtime(&buf->st_mtime);
  extension = strrchr(file->d_name, '.');

  snprintf(check, sizeof(check), date,
          time->tm_year+1900, time->tm_mon+1, time->tm_mday,
            time->tm_hour, time->tm_min, time->tm_sec);

  match = ps_test_naming(file->d_name, regmatch);

  if(strstr(path, check) == NULL && (match == 0 || match == 2 || match == 3) 
    && extension != NULL){
    if (match == 0){
      snprintf(naming, sizeof(naming), path_name,
              rec ? base_dir : p,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_hour, time->tm_min, time->tm_sec, file->d_name);
    }

    if (match == 2){
      snprintf(naming, sizeof(naming), path_name,
              rec ? base_dir : p,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_hour, time->tm_min, time->tm_sec, regmatch, extension);
    }

    free(regmatch);

    if (match == 3){
      snprintf(naming, sizeof(naming), path_name,
              rec ? base_dir: p,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_hour, time->tm_min, time->tm_sec, extension);
    }

    if(strstr(path, ".jpg") != NULL ||
    strstr(path, ".jpeg") != NULL ||
    strstr(path, ".png") != NULL ||
    strstr(path, ".tiff") != NULL){

      ps_create(p, time->tm_year+1900, time->tm_mon+1, time->tm_mday);

      if (rename(path, naming) != 0){
        printf("Error while renaming.\n");
      }
    }

    free(buf);
  }
}

int ps_create (const char *p, int y,
               int m, int d) {
  char new_dir[500];

  snprintf(new_dir, sizeof(new_dir), "%s/%d/", p,
           y);
  mkdir (new_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  snprintf(new_dir, sizeof(new_dir), "%s/%d/%d", p,
           y, m);
  mkdir (new_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  snprintf(new_dir, sizeof(new_dir), "%s/%d/%d/%d", p,
           y, m, d);

  return mkdir (new_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

int ps_test_naming(const char *p, char *buf){
  const char *pattern = "^[0-9]{4}([-_.][0-9]{1,2}){0,2}([-:_][0-9]{0,2}){0,4}_([^_.]+)\\..+$";
  regex_t regex;
  regmatch_t matches[4];
  int reti;

  reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    return 1;
  }

  reti = regexec(&regex, p, 4, matches, 0);
  if (reti == REG_NOMATCH) {
    return 0;
  } else {
    int start = matches[3].rm_so;
    int end = matches[3].rm_eo;
    if (start != -1 && end != -1) {
      char result[256];
      strncpy(result, p + start, end - start);
      result[end - start] = '\0';
      printf("For '%s', the part after the pattern is: %s\n", p, result);
      snprintf(buf, sizeof(buf), "%s", result);
      return 2;
    } else {
      printf("There is no explicit naming in %s.\n", p);
      return 3;
    }
  }
  
  regfree(&regex);
}
