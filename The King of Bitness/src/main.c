#include <stdio.h>
#include <stdlib.h>

int _check(char* password);

void print_memo()
{
  printf("\
                                    oo\n\
                                   $\"\"$o\n\
                                  $\"  $$\n\
                                   $$$$\n\
                                   o \"$o\n\
                                  o\"  \"$\n\
             oo\"$$$\"  oo$\"$ooo   o$    \"$    ooo\"$oo  $$$\"o\n\
o o o o    oo\"  o\"      \"o    $$o$\"     o o$\"\"  o$      \"$  \"oo   o o o o\n\
\"$o   \"\"$$$\"   $$         $      \"   o   \"\"    o\"         $   \"o$$\"    o$$\n\
  \"\"o       o  $          $\"       $$$$$       o          $  ooo     o\"\"\n\
     \"o   $$$$o $o       o$        $$$$$\"       $o        \" $$$$   o\"\n\
      \"\"o $$$$o  oo o  o$\"         $$$$$\"        \"o o o o\"  \"$$$  $\n\
        \"\" \"$\"     \"\"\"\"\"            \"\"$\"            \"\"\"      \"\"\" \"\n\
         \"oooooooooooooooooooooooooooooooooooooooooooooooooooooo$\n\
          \"$$$$\"$$$$\" $$$$$$$\"$$$$$$ \" \"$$$$$\"$$$$$$\"  $$$\"\"$$$$\n\
           $$$oo$$$$   $$$$$$o$$$$$$o\" $$$$$$$$$$$$$$ o$$$$o$$$\"\n\
           $\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"$\n\
           $\"                                                 \"$\n\
           $\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\n");
}


int main()
{
  print_memo();
  printf("\n");
  printf("I'm the King of bitness.\n");
  char *password = calloc(256, 1);
  if (!password)
    return -1;
  printf("Prove yourself worthy:");
  scanf("%255s", password);
  if (_check(password) == 0)
    printf("My respect. You can submit the flag\n");
  else
    printf("WRONG!\n");
  return 0;
}