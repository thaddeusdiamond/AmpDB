#include <string>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <stdint.h>
#include <iostream>

#define STDIN  0
#define STDOUT 1

using namespace std;

int main(int argc, char* argv[]) {
    int pid, fd[2];
    char *args[4];
    pipe(fd);
    
    // <--- FORKING ERROR
  	if ((pid = fork()) < 0) {
  	  exit(fprintf(stderr, "Pipe failed"));
  	  
  	// <--- IN CHILD PROCESS (EXECUTE COMMAND)  
  	} else if (pid == 0) {
  	  dup2(fd[0], STDIN);
  	  dup2(fd[1], STDOUT);
  	  args[0] = "../lm/lm";
  	  args[1] = "\"../test.conf\"";
      args[2] = "0";
    
      args[3] = NULL;
      execvp(args[0], args);
  	  
  	// <--- IN PARENT PROCESS (ADJUST GLOBALS)
  	} else {
  	    // <--- FORKING ERROR
      	if ((pid = fork()) < 0) {
      	  exit(fprintf(stderr, "Pipe failed"));
      	  
      	} else if (pid == 0) {
      	  dup2(fd[0], STDOUT);
      	  dup2(fd[1], STDIN);
      	  args[0] = "./TPC-C";
      	  args[1] = "\"../test.conf\"";
          args[2] = "0";
          
          args[3] = NULL;
          execvp(args[0], args);
  	    
  	    } else {
  	      while (true) 
  	        ;
        }
  	}
    
    fprintf(stderr, "Exec failed");
    exit(1); 
}
