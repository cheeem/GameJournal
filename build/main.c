#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <assert.h>

#define PATH_BUF_CAP 512
#define FILE_BUF_CAP 4096

char file_path_in[PATH_BUF_CAP];
char file_buf[FILE_BUF_CAP];

int main(int argc, char** argv) {
  char* dir_path = "markdown";
  char* file_path_out = "public/content.js";

  if(argc > 1) {
    dir_path = argv[1];
  }
  if(argc > 2) {
    file_path_out = argv[2];
  }

  FILE* out = fopen(file_path_out, "w");
  assert(out != NULL);

  fwrite("const content = [", 1, 17, out);

  DIR *dir = opendir(dir_path);
  assert(dir != NULL);

  struct dirent *entry;
  do {
    entry = readdir(dir);
    if(entry == NULL) {
      break;
    }
    if(entry->d_name[0] == '.') {
      continue;
    }

    snprintf(file_path_in, PATH_BUF_CAP, "%s/%s", dir_path, entry->d_name);

    FILE* in = fopen(file_path_in, "r");
    assert(in != NULL);

    fwrite("`", 1, 1, out);
    size_t bytes_in;
    do {
      bytes_in = fread(file_buf, 1, FILE_BUF_CAP, in);
      assert(bytes_in >= 0);
      if(bytes_in == 0) {
        break;
      }
      size_t bytes_out = fwrite(file_buf, 1, bytes_in, out);
      assert(bytes_out > 0);
    } while(1);
    fwrite("`,", 1, 2, out);

  } while(1);

  closedir(dir);
  fwrite("];", 1, 2, out);
  fflush(out);
  fclose(out);
}
