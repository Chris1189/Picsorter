#include "ps_fs.h"

void dir_initialize(const char *p, int rec,
                    const char *base_dir) {
  DIR *d;
  char file_abs[500];
  char base_abs[500];

  if(p && *p != '/'){
    getcwd(file_abs, sizeof(file_abs));        
    strcat(file_abs, "/");
    strcat(file_abs, p);
  }
  else{
    snprintf(file_abs, sizeof(file_abs), "%s", p);
  }

  if(base_dir && *base_dir != '/'){
    getcwd(base_abs, sizeof(base_abs));
    strcat(base_abs, "/");
    strcat(base_abs, base_dir);
  }
  else{
    snprintf(base_abs, sizeof(base_abs), "%s", base_dir);
  }

  d = opendir(file_abs);

  if (d != NULL) {
    ps_scandir(file_abs, d, rec, base_abs);
  }
  closedir(d);
}

void ps_scandir(const char *p, DIR *d,
                int rec, const char *base){
  struct dirent *file;

  do {
    char new_p[500];
    file = readdir(d);

    if(file == NULL)
      break;

    if(strcmp(file->d_name, ".") == 0 ||
      strcmp(file->d_name, "..") == 0){
      continue;
    }

    if(file->d_type == DT_DIR && rec){
      snprintf(new_p, sizeof(new_p), "%s/%s", p, file->d_name);
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

  regmatch = malloc(sizeof(char)*256);
  snprintf(path, sizeof(path), "%s/%s", p, file->d_name);

  buf = malloc(sizeof(struct stat));
  lstat(path, buf);
  time = localtime(&buf->st_mtime);
  extension = strrchr(file->d_name, '.');

  snprintf(check, sizeof(check),"%d-%d-%d_%d:%d:%d",
          time->tm_year+1900, time->tm_mon+1, time->tm_mday,
            time->tm_hour, time->tm_min, time->tm_sec);

  match = ps_test_naming(file->d_name, regmatch);

  if(strstr(path, check) == NULL && (match == 0 || match == 2 || match == 3) 
    && extension != NULL){
    if (match == 0){
      snprintf(naming, sizeof(naming),"%s/%d/%d/%d/%d-%d-%d_%d:%d:%d_%s",
              rec ? base_dir : p,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_hour, time->tm_min, time->tm_sec, file->d_name);
    }

    if (match == 2){
      snprintf(naming, sizeof(naming),"%s/%d/%d/%d/%d-%d-%d_%d:%d:%d_%s%s",
              rec ? base_dir : p,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_year+1900, time->tm_mon+1, time->tm_mday,
              time->tm_hour, time->tm_min, time->tm_sec, regmatch, extension);
      printf("%s", naming);
    }

    free(regmatch);

    if (match == 3){
      snprintf(naming, sizeof(naming),"%s/%d/%d/%d/%d-%d-%d_%d:%d:%d%s",
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
  }

  free(buf);
}

int ps_create(const char *p, int y,
              int m, int d){
  pid_t pid = fork();
  char year[5];
  char month[3];
  char day[3];

  snprintf(year, sizeof(year), "%d", y);
  snprintf(month, sizeof(month), "%d", m);
  snprintf(day, sizeof(day), "%d", d);

  if (pid == -1) {
    perror("Fork failed");
    return -1;
  } else if (pid == 0) {
    execl("/usr/bin/sh", "sh", "./bin/foldersort", "create",
          p, year, month, day, (char *) NULL);
    perror("execl failed");
    exit(EXIT_FAILURE);
  } else {
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
      printf("Script did not terminate normally.\n");
    }
  }

  return 0;
}


int ps_sort(const char *p, int y,
            int m, int d){
  char year[5];
  char month[3];
  char day[3];

  pid_t pid = fork();
  snprintf(year, sizeof(year), "%d", y);
  snprintf(month, sizeof(month), "%d", m);
  snprintf(day, sizeof(day), "%d", d);

  if (pid == -1) {
    perror("Fork failed");
    return -1;
  } else if (pid == 0) {
    execl("/usr/bin/sh", "sh", "./bin/foldersort", "move",
          p, year, month, day, (char *) NULL);
    perror("execl failed");
    exit(EXIT_FAILURE);
  } else {
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
      printf("Script did not terminate normally.\n");
    }
  }

  return 0;
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
