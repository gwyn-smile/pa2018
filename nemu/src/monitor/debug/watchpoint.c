#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

//Add new using watch point to the head
WP* new_wp() {
  if(free_== NULL) {
    printf("There is no free watchpoint!\n");
	assert(0);
  }
  else {	
	if(head == NULL) {	
	  head = free_;
	}
	else {
	  free_->next = head;
	  head = free_;
	}
    free_ = free_->next;
	return head;
  }
}

void free_wp(WP *wp) {
  WP* tmp = NULL;
  tmp = head;
  if(tmp -> NO == wp -> NO) {
	head = head -> next;
	tmp -> next = free_;
	free_ = tmp;
  }
  while(tmp->next != NULL) {
	if(tmp->next->NO == wp->NO) {
	  tmp->next->next = free_;
	  free_ = tmp->next;

	  tmp->next = tmp->next->next;
	  break;
	}
	tmp = tmp->next;
  }
}

