#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>
#include <limits.h>
#include <dlfcn.h>
#include <signal.h>
#include <string.h>

#include "main.h"


const unsigned char key[] = {
  55,33,56,53,
  40,38,50,43,
  44,255,45,1,
  27,52,70,6,
  50,71,8,27,
  35,70,6,23,
  50,71,7,27,
  53,4,71,8,
  33,27,39,67,
  71,10,253};

void child()
{
  ptrace(PT_TRACE_ME, 0, 0);
  printf("Enter password:");
  char *password = calloc(256, 1);
  scanf("%255s", password);
  if (check(password) == 0)
    print_success();
  else
    print_fail();
}

void parent(pid_t pid)
{
  int key_pos = 0;
  struct user_regs_struct regs;
  int idx = 0;
  int status;
  while (1)
  {
    waitpid(pid, &status, 0);
    if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP)
    {
      ptrace(PTRACE_GETREGS, pid, NULL, &regs);
      switch (regs.eax)
      {
      // xor request
      case 0x69696969:
      {
        #ifdef DEBUG
        printf("xor request 0x%lx ^ 0x%lx\n", (regs.ebx & 0xff), (regs.edx & 0xff));
        #endif
        regs.eax = (regs.ebx & 0xff) ^ (regs.edx & 0xff);
        break;
      }
      
      // add request
      case 0xf0f0f0f0:
      {
        #ifdef DEBUG
        printf("add request 0x%lx + 0x%lx\n", (regs.ebx & 0xff), (regs.edx & 0xff));
        #endif
        regs.eax = (regs.ebx & 0xff) + (regs.edx & 0xff);
        break;
      }

      // sub request
      case 0xfefefefe:
      {
        #ifdef DEBUG
        printf("sub request 0x%lx - 0x%lx\n", (regs.ebx & 0xff), (regs.edx & 0xff));
        #endif
        regs.eax = (regs.ebx & 0xff) - (regs.edx & 0xff);
        break;
      }

      // key request
      case 0xcafebabe:
      {
        #ifdef DEBUG
        printf("key request\n");
        #endif
        regs.eax = key[idx++];
        break;
      }

      case 0x1337:
      {
        #ifdef DEBUG
        printf("change eip from 0x%lx to 0x%lx or 0x%lx\n", regs.eip, regs.ebx, regs.edx);
        #endif
        if (regs.eflags & (1 << 6))
          regs.eip = regs.edx;
        else
          regs.eip = regs.ebx;
        break;
      }

      // unknown request, probably debugger error
      default:
      {
        #ifdef DEBUG
        printf("Uknown request\n");
        #endif
        break;
      }
      }
      ptrace(PTRACE_SETREGS, pid, NULL, &regs);
      ptrace(PTRACE_CONT, pid, NULL, NULL);
    }
    else
      return;
  }

}

__attribute__((alwaysinline))
void print_success()
{
  printf("\
             _.~._\n\
        ,~'.~@~.`~.\n\
       / : _..._ : \\\n\
      { :,\"''\\`\".: }\n\
       `C) 9 _ 9 (--.._,-\"\"\"-.__\n\
        (  )(@)(  )             `.\n\
         `-.___.-'                \\\n\
         ,' \\ /    ,`             ;`-._,-.\n\
       ,'  ,'/   ,'           `---t.,-.   \\_\n\
     ,--.,','  ,'----.__\\         _(   \\----'\n\
   '///,`,--.,'          `-.__.--'  `.  )\n\
       '///,'                         `-`\t\tYeah dad, I've got the flag!\n");
}

__attribute__((always_inline))
void print_fail()
{
  printf("\
             _.~._\n\
        ,~'.~@~.`~.\n\
       / : _..._ : \\\n\
      { :,\"''\\`\".: }\n\
       `C) 9 _ 9 (--.._,-\"\"\"-.__\n\
        (  )(@)(  )             `.\n\
         `-.___.-'                \\\n\
         ,' \\ /    ,`             ;`-._,-.\n\
       ,'  ,'/   ,'           `---t.,-.   \\_\n\
     ,--.,','  ,'----.__\\         _(   \\----'\n\
   '///,`,--.,'          `-.__.--'  `.  )\n\
       '///,'                         `-`\t\tNo dad, I've lost :(\n");
}

__attribute__((always_inline))
void print_memo()
{
  printf("\
   ⢸⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⡷⡄\n\
   ⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠢⣀\n\
   ⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠈⠑⢦⡀\n\
   ⢸⠀⠀⠀⠀⢀⠖⠒⠒⠒⢤⠀⠀⠀⠀⡇⠀⠀⠀⠀⠙⢦⡀\t\tAre you winning, son?\n\
   ⢸⠀⠀⣀⢤⣼⣀⡠⠤⠤⠼⠤⡄⠀⠀⡇⠀⠀⠀⠀⠀⠀⠙⢄\n\
   ⢸⠀⠀⠑⡤⠤⡒⠒⠒⡊⠙⡏⠀⢀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠑⠢⡄\n\
   ⢸⠀⠀⠀⠇⠀⣀⣀⣀⣀⢀⠧⠟⠁⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⡇\n\
   ⢸⠀⠀⠀⠸⣀⠀⠀⠈⢉⠟⠓⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n\
   ⢸⠀⠀⠀⠀⠈⢱⡖⠋⠁⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n\
   ⢸⠀⠀⠀⠀⣠⢺⠧⢄⣀⠀⠀⣀⣀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n\
   ⢸⠀⠀⠀⣠⠃⢸⠀⠀⠈⠉⡽⠿⠯⡆⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n\
   ⢸⠀⠀⣰⠁⠀⢸⠀⠀⠀⠀⠉⠉⠉⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n\
   ⢸⠀⠀⠣⠀⠀⢸⢄⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n\
   ⢸⠀⠀⠀⠀⠀⢸⠀⢇⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n\
   ⢸⠀⠀⠀⠀⠀⡌⠀⠈⡆⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n\
   ⢸⠀⠀⠀⠀⢠⠃⠀⠀⡇⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸\n\
   ⢸⠀⠀⠀⠀⢸⠀⠀⠀⠁⠀⠀⠀⠀⠀⠷\n");
}

int main()
{
  print_memo();
  pid_t pid = fork();
  switch (pid)
  {
  case 0:
    child();
    break;
  case -1:
    printf("Something went wrong, try again..\n");
    break;
  default:
    parent(pid);
    break;
  }
  return 0;
}