#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char content[32];
  bool status;
  int val;
} WP;

bool free_wp(int num);
WP* new_wp();
WP* get_head();

#endif
