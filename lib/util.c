#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h> 

/**
 * Check directory if it not exists it will be created
*/
int chkDir(const char* path){
  struct stat sb;
  if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
    return 1;
  }else {
    return 0;
  }   
}

/**
 * Create Directory on path
*/
void createDir(const char* path){
  mkdir(path, 0777);
}

/**
 * Concat String S1 and s2
*/
char* concat(const char *s1, const char *s2)
{
  char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
  // in real code you would check for errors in malloc here
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

/**
 * Verify id string is a number.
*/
int isNumber(char *text){
  int j;
  j = strlen(text);
  while(j--){
    if(text[j] > 47 && text[j] < 58)
      continue;
    return 0;
  }
  return 1;
}
