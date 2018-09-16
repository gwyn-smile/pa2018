#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args) {
  int steps_num = 0;
  char *arg = strtok(NULL, " ");
  if(arg == NULL)
	  steps_num = 1;
  else
  {
    steps_num = atoi(arg);
  }
  if(steps_num >= 0)
    cpu_exec(steps_num);
  else
	printf("the number of step should >= 0\n");
  return 0;
}

static int cmd_info(char *args) {
  char *arg = strtok(NULL, " ");
  if(arg == NULL)
    printf("there should be a second argument\n");
  else if (*arg == 'r') {
    printf("%%eax: %x\n",reg_l(R_EAX));
    printf("%%ebx: %x\n",reg_l(R_EBX));
    printf("%%ecx: %x\n",reg_l(R_ECX));
    printf("%%edx: %x\n",reg_l(R_EDX));
    printf("%%esp: %x\n",reg_l(R_ESP));
    printf("%%ebp: %x\n",reg_l(R_EBP));
    printf("%%esi: %x\n",reg_l(R_ESI));
    printf("%%edi: %x\n",reg_l(R_EDI));
  }
  else if(*arg == 'w') {

  }
  else 
    printf("Unkown second argument(should be r/w)"); 
  return 0;
}

static int cmd_x(char *args) {
  return 0;
}

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  { "si", "Step into//For assembly instructions", cmd_si },
  { "info", "Print registers/watchpoints", cmd_info },
  { "x", "Print the value of memory", cmd_x }
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
