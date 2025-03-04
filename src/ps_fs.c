#include "ps_fs.h"

void
check_abspath (char *dest, const char *path)
{
  if (path && *path != '/')
    {
      dest = realpath (path, NULL) ? realpath (path, NULL) : NULL;
    }
  snprintf (dest, sizeof (path), "%s", path);
}

void
dir_initialize (const char *p, int rec, const char *base_dir)
{
  DIR *d;
  char *file_abs = malloc (sizeof (char) * 256);
  char *base_abs = malloc (sizeof (char) * 256);

  check_abspath (file_abs, p);
  check_abspath (base_abs, base_dir);

  d = opendir (file_abs);

  if (d != NULL && file_abs && base_abs)
    {
      ps_scandir (file_abs, d, rec, base_abs);
    }

  free (file_abs);
  free (base_abs);
  closedir (d);
}

void
perform_dir_recursive (char *dest, const char *path, dirent *file,
                       const char *base, int rec)
{
  if (file->d_type == DT_DIR && rec)
    {
      snprintf (dest, sizeof (dest), "%s/%s", path, file->d_name);
      dir_initialize (dest, rec, base);
    }
}

void
ps_scandir (const char *p, DIR *d, int rec, const char *base)
{
  dirent *file;

  do
    {
      char *new_p;
      file = readdir (d);

      if (file == NULL)
        break;

      if (strcmp (file->d_name, ".") == 0 || strcmp (file->d_name, "..") == 0)
        {
          continue;
        }
      perform_dir_recursive (new_p, p, file, base, rec);
      ps_rename (p, file, rec, base);
    }
  while (file);
}

void
get_stats (const char *path, statf *buf, tm *time, char *extension,
           dirent *file)
{
  buf = malloc (sizeof (statf));
  lstat (path, buf);
  time = localtime (&buf->st_mtime);
  extension = strrchr (file->d_name, '.');
}

void
prepare_string (char *dst, const char *str, const char *path, int y, int m,
                int d, int h, int min, int s, dirent *file, const char *regex,
                const char *ext)
{
  if (path && file)
    snprintf (dst, sizeof (dst), str, path, y, m, d, y, m, d, h, min, s,
              file->d_name);
  else if (path && regex && ext)
    snprintf (dst, sizeof (dst), str, path, y, m, d, y, m, d, h, min, s, regex,
              ext);
  else if (path && !regex && ext)
    snprintf (dst, sizeof (dst), str, path, y, m, d, y, m, d, h, min, s, ext);
  else
    snprintf (dst, sizeof (dst), str, y, m, d, h, min, s);
}

void
ps_rename (const char *p, dirent *file, int rec, const char *base_dir)
{
  int match;
  statf *buf;
  tm *time;
  char path[500];
  char naming[500];
  char *regmatch;
  char check[500];
  char *extension;
  char date[18] = "%d-%d-%d_%d:%d:%d";
  char path_name[33] = "%s/%d/%d/%d/%d-%d-%d_%d:%d:%d_%s";

  regmatch = malloc (sizeof (char) * 256);
  snprintf (path, sizeof (path), "%s/%s", p, file->d_name);

  get_stats (path, buf, time, extension, file);

  prepare_string (check, date, NULL, time->tm_year + 1900, time->tm_mon + 1,
                  time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec,
                  NULL, NULL, NULL);

  match = ps_test_naming (file->d_name, regmatch);

  if (strstr (path, check) == NULL && (match == 0 || match == 2 || match == 3)
      && extension != NULL)
    {
      if (match == 0)
        {
          prepare_string (naming, path_name, rec ? base_dir : p,
                          time->tm_year + 1900, time->tm_mon + 1,
                          time->tm_mday, time->tm_hour, time->tm_min,
                          time->tm_sec, file, NULL, NULL);
        }

      if (match == 2)
        {
          prepare_string (naming, path_name, rec ? base_dir : p,
                          time->tm_year + 1900, time->tm_mon + 1,
                          time->tm_mday, time->tm_hour, time->tm_min,
                          time->tm_sec, NULL, regmatch, extension);
        }

      free (regmatch);

      if (match == 3)
        {
          prepare_string (naming, path_name, rec ? base_dir : p,
                          time->tm_year + 1900, time->tm_mon + 1,
                          time->tm_mday, time->tm_hour, time->tm_min,
                          time->tm_sec, NULL, NULL, extension);
        }

      if (strstr (path, ".jpg") != NULL || strstr (path, ".jpeg") != NULL
          || strstr (path, ".png") != NULL || strstr (path, ".tiff") != NULL)
        {

          ps_create (p, time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);

          if (rename (path, naming) != 0)
            {
              printf ("Error while renaming.\n");
            }
        }

      free (buf);
    }
}

int
ps_create (const char *p, int y, int m, int d)
{
  char new_dir[500];

  snprintf (new_dir, sizeof (new_dir), "%s/%d/", p, y);
  mkdir (new_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  snprintf (new_dir, sizeof (new_dir), "%s/%d/%d", p, y, m);
  mkdir (new_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  snprintf (new_dir, sizeof (new_dir), "%s/%d/%d/%d", p, y, m, d);

  return mkdir (new_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

int
ps_test_naming (const char *p, char *buf)
{
  const char *pattern
      = "^[0-9]{4}([-_.][0-9]{1,2}){0,2}([-:_][0-9]{0,2}){0,4}_([^_.]+)\\..+$";
  regex_t regex;
  regmatch_t matches[4];
  int reti;

  reti = regcomp (&regex, pattern, REG_EXTENDED);
  if (reti)
    {
      fprintf (stderr, "Could not compile regex\n");
      return 1;
    }

  reti = regexec (&regex, p, 4, matches, 0);
  if (reti == REG_NOMATCH)
    {
      return 0;
    }
  else
    {
      int start = matches[3].rm_so;
      int end = matches[3].rm_eo;
      if (start != -1 && end != -1)
        {
          char result[256];
          strncpy (result, p + start, end - start);
          result[end - start] = '\0';
          printf ("For '%s', the part after the pattern is: %s\n", p, result);
          snprintf (buf, sizeof (buf), "%s", result);
          return 2;
        }
      else
        {
          printf ("There is no explicit naming in %s.\n", p);
          return 3;
        }
    }

  regfree (&regex);
}
