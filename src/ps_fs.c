#include "ps_fs.h"

void dir_initialize(const char *p, int rec) {
  DIR *d;
  char file_abs[500];

  if(p && *p != '/'){
    getcwd(file_abs, sizeof(file_abs));        
    strcat(file_abs, "/");
    strcat(file_abs, p);
  }
  else{
    snprintf(file_abs, sizeof(file_abs), "%s", p);
  }

  d = opendir(file_abs);

  if (d != NULL) {
    ps_scandir(file_abs, d, rec);
  }
  closedir(d);
}

void ps_scandir(const char *p, DIR *d, int rec){
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
      dir_initialize(new_p, rec);
    }
    ps_rename(p, file);
  }
  while (file);
}

void ps_rename(const char *p, struct dirent *file){
  struct stat *buf;
  struct tm *time;
  char path[500];
  char naming[500];
  char check[500];
  char *extension;

  snprintf(path, sizeof(path), "%s/%s", p, file->d_name);

  buf = malloc(sizeof(struct stat));
  lstat(path, buf);
  time = localtime(&buf->st_mtime);
  extension = strrchr(file->d_name, '.');

  snprintf(check, sizeof(check),"%d-%d-%d_%d:%d:%d_Image",
          time->tm_year+1900, time->tm_mon+1, time->tm_mday,
            time->tm_hour, time->tm_min, time->tm_sec);

  if(strstr(path, check) == NULL && extension != NULL){
    snprintf(naming, sizeof(naming),"%s/%d/%d/%d/%d-%d-%d_%d:%d:%d_Image%s",p,
            time->tm_year+1900, time->tm_mon+1, time->tm_mday,
            time->tm_year+1900, time->tm_mon+1, time->tm_mday,
            time->tm_hour, time->tm_min, time->tm_sec, extension);

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

int ps_create(const char *p, int y, int m, int d){
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
    if (WIFEXITED(status)) {
      printf("Script exited with status: %d\n", WEXITSTATUS(status));
    } else {
      printf("Script did not terminate normally.\n");
    }
  }

  return 0;
}


int ps_sort(const char *p, int y, int m, int d){
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
    if (WIFEXITED(status)) {
      printf("Script exited with status: %d\n", WEXITSTATUS(status));
    } else {
      printf("Script did not terminate normally.\n");
    }
  }

  return 0;
}
