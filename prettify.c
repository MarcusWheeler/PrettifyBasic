#define  _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#define MAX_LINE_LENGTH 512
#define MAX_FILENAME_SIZE 256
#define DEFAULT_SPACES 2
int main(int argc,char* argv[]){
  if(argc < 3){
    printf("Please provide one file to process and the name of the output\n");
    return -1;
  }
  if(strcmp(argv[1], argv[2]) == 0){
    printf("Source filename can't be the same as destination filename rip\n");
    return -1;
  }
  /*
  So, first things first I just want to process the file line by line and print it out for now
  After I know that I can read and print my file, I'll worry about changing it.
  */
  int number_of_spaces = DEFAULT_SPACES;
  char *newline = "\n";
  char buffer[MAX_LINE_LENGTH];
  char *current_line;
  bool found_this_iteration = false;
  bool cleared_whitespace = false;
  size_t length;
  int number_of_layers_deep = 0;  
  printf("%s\n", argv[2]);
  FILE *prettify_me = fopen(argv[1], "r");
  FILE *prettified = fopen(argv[2], "w+");
  if(!prettify_me){
    printf("File failed to open to be read\n");
  }
  if(!prettified){
    printf("File failed to open to be written\n");
    return 0;
  }
  
  //Now that I have my file open, I can read it while it's not failing.
  while(getline(&current_line, &length, prettify_me )!= -1){
    cleared_whitespace = false;
    found_this_iteration = false;
    for(int i = 0; i < strlen(current_line); ++i){
        if(current_line[i] == '{'){
          number_of_layers_deep++;
          found_this_iteration = true;
        }
      if(current_line[i] == '}'){
        number_of_layers_deep--;
        if(found_this_iteration){
          found_this_iteration = false;
        }
      }
    }
    //This portion (soon to be function TODO) adds the needed spaces
    //If I've found an opening brace this loop, subtract two spaces.
    if(argv[3]){
      number_of_spaces = atoi(argv[3]);
    }
    //printf("NUMBER OF SPACES: %s", number_of_spaces); 
    for(int i = 0; i < number_of_spaces*( number_of_layers_deep - found_this_iteration); ++i){
      buffer[i] = ' ';
    }
    int buffer_index = 0;
    while(!cleared_whitespace){
      if(current_line[buffer_index] != ' ' && current_line[buffer_index] != '\t'){
        strcpy(&buffer[number_of_spaces * (number_of_layers_deep - found_this_iteration)], &current_line[buffer_index]);
        strcat(buffer, newline);
        cleared_whitespace = true;
      }
      buffer_index++;
    }
    fwrite(buffer, sizeof(char), strlen(buffer) - 1, prettified);
    
  }
  free(current_line);
  fclose(prettify_me);
  fclose(prettified);
  
  return 0;
}
