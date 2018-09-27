#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

WP* get_head() {
  return head;
}

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

void print_wp() {
  WP* busy = head, *free = free_;
  while(busy != NULL) {
	printf("watchpoint %d: %s\n", busy->NO, busy->content);
	busy = busy->next;
  }
  while(free != NULL) {
	printf("free %d, %s\n", free->NO, free->content);
	free = free->next;
  }
  return;
}

//Add new using watch point to the head
WP* new_wp() {
  if(free_ == NULL) {
    printf("There is no free watchpoint!\n");
	return NULL;
  }
  else {
	WP* free = NULL;	
	if(head == NULL) {	
	  head = free = free_;
	  free_ = free_->next;
	  head->next = NULL;
	}
	else {
	//find the right position to insert the new watchpoint
	  WP *tmp = head;
	  if(tmp->NO > free_->NO) {
		free = free_;
		free_ = free_->next;
		free->next = head;
		head = free;
	  }
	  else {
	    while(tmp->next != NULL && tmp->next->NO < free_->NO) {
		  tmp = tmp->next;
	    }
		free = free_;
		free_ = free_->next;
		free->next = tmp->next;
		tmp->next = free;
	   }
	}
    free->status = true;
	print_wp();
	return free;
  }
}

bool free_wp(int num) {
  WP* tmp = head;
  if(head == NULL) {
	printf("There is no using watchpoint!\n");
	return false;
  }
  //find the target watchpoint and store in 'tmp'
  if(tmp->NO == num) {
	head = head->next;
	tmp->next = NULL;
  }
  else {
    while(tmp->next != NULL) {
  	  if(tmp->next->NO == num) 
	    break;
	  else
        tmp = tmp->next;
	}
    if(tmp->next == NULL) {
  	  printf("Can't find the target watchpoint!\n");
	  return false;
    }
	WP* m = tmp;
	tmp = tmp->next;
	m->next = m->next->next;
  }
  printf("before find the pos\n");
  //find the right position to insert the free watchpoint
  WP* free = free_;
  if(free->NO > tmp->NO) {
	tmp->next = free_;
	free_ = tmp;
  }
  else {
	while(free->next != NULL && free->next->NO < num) {
      free = free->next;
	}
	tmp->next = free->next;
	free->next = tmp;
  }
  print_wp();
  return true;
}

bool en_wp(int num) {
  WP* tmp = head;
  while(tmp != NULL) {
	if(tmp->NO == num) {
	  tmp->status = true;
	  break;
	}
	tmp = tmp->next;
  }
  if(tmp == NULL) {
    printf("This watchpoint is not being used!\n");
	return false;
  }
  return true;
}

bool dis_wp(int num) {
  WP* tmp = head;
  while(tmp != NULL) {
	if(tmp->NO == num) {
	  tmp->status = false;
	  break;
	}
	tmp = tmp->next;
  }
  if(tmp == NULL) {
    printf("This watchpoint is not being used!\n");
	return false;
  }
  return true;

}
