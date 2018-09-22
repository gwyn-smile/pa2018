#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

typedef uint8_t bool;
#define true	 1
#define false	 0
// this should be enough
static char buf[65536];
uint32_t buf_pos = 0;
static inline bool gen(char target);
static inline bool gen_num();
static inline bool gen_rand_op();

static inline bool gen_rand_expr() {
  switch (rand()%3) {
	case 0: if(!gen_num()) return false; break;
	case 1: {
	  if(!gen('('))
		return false;
	  if(!gen_rand_expr())
		return false;
	  if(!gen(')'))
		return false;
	}; break;
	default: {
	  if(!gen_rand_expr())
		return false;
	  if(!gen_rand_op())
		return false;
	  if(!gen_rand_expr())
		return false;
	}; break;
  }
  return true;
}

static inline bool gen(char target) {
  if(buf_pos >= 65534)
	return false;
  buf[buf_pos++]=target;
  buf[buf_pos]='\0';
}
static inline bool gen_num() {
  if(buf_pos >= 65534)
	return false;
  buf[buf_pos++] = '0' + rand()%10;
  buf[buf_pos]='\0';
}
static inline bool gen_rand_op() {
  if(buf_pos >= 65534)
	return false;
  switch(rand()%4) {
    case 0: buf[buf_pos++]='+'; break;
	case 1: buf[buf_pos++]='-'; break;
	case 2: buf[buf_pos++]='*'; break;
	case 3: buf[buf_pos++]='/'; break;
  }
  buf[buf_pos]='\0';
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) { 
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
	buf_pos = 0;		//restore the position
    if(!gen_rand_expr()) {
	  i --;				//in order to meet the target
	  continue;
	}
    //strcpy(buf,"10/0");
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc -Werror .code.c -o .expr");
    //printf("system return value: %d\n", ret);
	if (ret != 0) {
	  i --;
	  continue;
	}

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  } 
  return 0;
}
