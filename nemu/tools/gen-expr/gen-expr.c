#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536];
uint32_t buf_pos = 0;
static inline void gen(char target);
static inline void gen_num();
static inline void gen_rand_op();

static inline void gen_rand_expr() {
  switch (rand()%3) {
	case 0: gen_num(); break;
	case 1: gen('('); gen_rand_expr(); gen(')'); break;
	default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
}

static inline void gen(char target) {
  buf[buf_pos++]=target;
  buf[buf_pos]='\0';
}
static inline void gen_num() {
  buf[buf_pos++] = '0' + rand()%10;
  buf[buf_pos]='\0';
}
static inline void gen_rand_op() {
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
	buf_pos = 0;
    gen_rand_expr();
    //strcpy(buf,"10/0");
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
