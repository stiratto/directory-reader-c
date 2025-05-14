#include <dirent.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

// explore directories using recursivity
// when it finds a subdirectory, execs exploreDirectories() again,
// executing a new while loop exclusively for that execution, when the
// readdir(dir) is null, returns 0, going back to the previous
// directory
//
// exploreDirectory(a)
// exploredirectory(a) finds a subdirectory called b
// executes exploreddirectory(b)
// in directory b no subdirectories left
// returns 0 (readdir() returned null)
// goes back to exploredirectory(a) while loop, if directories left in
// directory a, explore those directories, it works like that

int exploreDirectories(const char *dirStr) {
  // open the musci directory
  DIR *dir = opendir(dirStr);
  // if dir == null, something is wrong with the directory
  if (dir == NULL) {
    printf("Couldn't read directory %s, maybe doesn't exists?\n", dirStr);
    return 0;
  }

  struct dirent *dp;

  // while readdir isn't null, it hasn't reached the end of the
  // directory stream
  while ((dp = readdir(dir)) != NULL) {
    // if the current directory is a dir and not a file or smth else
    if (dp->d_type == DT_DIR) {

      // $ ls -A would give smth like this:
      // . .. dir1/ dir2/ dir3/ file1.sh file2.sh
      //
      if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..") != 0) {

        char buffer[1024];
        // format the new directory into a string, buffer now has the
        // formatted string
        int j = snprintf(buffer, sizeof(buffer), "%s/%s", dirStr, dp->d_name);
        printf("DIRECTORIOOOOO %s\n", buffer);

        // explore the current directory to see if we find a
        // subdirectory, if not, returns 0 in that while loop and we come back
        // again here
        exploreDirectories(buffer);
      }
    } else {
      printf("%s/%s\n", dirStr, dp->d_name);
    }
  }
  closedir(dir);
  return 0;
}

int checkFormat(char path[]) {
  regex_t regex;
  // check if path given is a real directory
  const char *pattern = "^/[a-zA-Z0-9._/-]+$";

  int reti = regcomp(&regex, pattern, REG_EXTENDED);

  // 1 if compile failed
  if (reti) {
    printf("Couldn't compile the regex pattern");
    return 1;
  }

  // 0 if match
  // reg_nomatch if no match
  reti = regexec(&regex, path, 0, NULL, 0);

  if (!reti) {
    return 0;
  } else if (reti == REG_NOMATCH) {
    printf("Wrong format, correct format: /home/$USER/Music (example))\n");
    return 1;
  } else {
    printf("Error.\n");
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc > 0 && argc <= 2) {
    if (!checkFormat(argv[1])) {
      exploreDirectories(argv[1]);
    } else {
      printf("Errors ocurred.");
      return 1;
    }
  }

  exploreDirectories(argv[1]);
  return 0;
}
