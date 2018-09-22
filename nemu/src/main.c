#include<stdio.h>
#include "nemu.h"
int init_monitor(int, char *[]);
//void ui_mainloop(int);
uint32_t expr(char*, bool*);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  //int is_batch_mode = init_monitor(argc, argv);
  init_monitor(argc, argv);
  /* Receive commands from user. */
  //ui_mainloop(is_batch_mode);
  
  FILE *fp = fopen("~/ics2018/nemu/tools/gen-expr/input", "r");
  if(fp == NULL)
    perror("input not found");
  char* ex = NULL;
  uint32_t val = 0;
  bool flag = true;
  int count = 0;
  while(!feof(fp)) { 
	count++; 
    fscanf(fp, "%u %s\n", &val, ex);
    if(val == expr(ex, &flag) && flag==true)
	  printf("%d: Success! ", count);
	else
	  printf("%d: Fail! ", count);
	if(count%10 == 0)
	  printf("\n");
  }
  return 0;
}
