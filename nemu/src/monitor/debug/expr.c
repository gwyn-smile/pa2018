#include "nemu.h"
#include <stdlib.h>
#include <string.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

/* TODO: Add more token types */
enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM, TK_NOTEQ, TK_AND,
  TK_LE, TK_GE, TK_RSHIFT, TK_LSHIFT, TK_DEREF, TK_XNUM, TK_REG
};

static struct op_priority {
  int type;
  int rank;
} op_prio_list[] = {
  
  {TK_DEREF, 1}, {'*', 3}, {'/', 3}, {'+', 4}, {'-', 4},
  {TK_LSHIFT, 5}, {TK_RSHIFT, 5},
  {TK_GE, 6}, {TK_LE, 6},
  {TK_EQ, 7}, {TK_NOTEQ, 7},
  {TK_AND, 11}

};

#define OP_NUM (sizeof(op_prio_list) / sizeof(op_prio_list[0]) )

static struct rule { 
  char *regex;
  int token_type;
} rules[] = {

   /* TODO:  Add more rules.  
   * Pay attention to the precedence level of different rules.
   */
  {"\\(", '('},			// left bracket
  {"\\)", ')'},			// right bracket
  {"\\*", '*'},			// multiply && get the address
  {"\\/", '/'},			// divide
  {"\\-", '-'},			// minus   
  
  {"0x([1-9a-fA-F][0-9a-fA-F]*|0)", TK_XNUM},	//%x numbers
  {"[1-9][0-9]*|0", TK_NUM},	//%d numbers
  {"\\%e(ax|cx|dx|bx|sp|bp|si|di)", TK_REG},	// registers
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"!=", TK_NOTEQ},		// not equal
  {"&&", TK_AND},
  {"<=", TK_LE},
  {">=", TK_GE},
  {"<<", TK_LSHIFT},
  {">>", TK_RSHIFT}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

Token tokens[65536];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
	for (i = 0; i < NR_REGEX; i ++) {
       if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        //Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //  i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
          */

         switch (rules[i].token_type) {

           case '+':case '*':case '-':case '/':case '(':case ')':case TK_EQ:case TK_AND:case TK_RSHIFT:case TK_LSHIFT:case TK_NOTEQ:case TK_GE:case TK_LE: {
			  tokens[nr_token++].type = rules[i].token_type;
		   }; break;
		   
		   case TK_REG: {
		     tokens[nr_token].type = rules[i].token_type;
			 strncpy(tokens[nr_token].str, substr_start, substr_len);
			 nr_token++;
		   }; break;

	 	   case TK_NUM: case TK_XNUM: {
			  tokens[nr_token].type = rules[i].token_type;
			  // if str_len > 32 then cut off the rest
              if (substr_len <= 32)
				  strncpy(tokens[nr_token].str, substr_start, substr_len);
			  else
				  strncpy(tokens[nr_token].str, substr_start + substr_len - 32, 32);
			  nr_token++;

		   }; break;

		   default: {
			 printf("Can't find token_type!\n");
			 assert(0);
		   }; break;
         }
         break;
       }
    }

    if (i == NR_REGEX) {
	  printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
     } 
   }  

  return true;
}

uint32_t eval(int p, int q);
bool check_parentheses(int p, int q);

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  else
	*success = true;

  /* TODO: Insert codes to evaluate the expression. */
  for(int i = 0;i <= nr_token - 1;i++) {
    if(tokens[i].type == '*' && (i==0||(tokens[i - 1].type!=TK_NUM&&tokens[i-1].type!=')'))) {
	  tokens[i].type = TK_DEREF;
	}
  }
  uint32_t val = eval(0, nr_token - 1);
  //printf("the expr val is %u\n", val);
  return val;
}

bool check_parentheses(int p, int q) {
  bool flag = (tokens[p].type == '(') && (tokens[q].type == ')');
  int count=0;
  for(int tmp = p; tmp != q; tmp ++) {
    if(tokens[tmp].type == '(')
	  count++;
	else if(tokens[tmp].type == ')')
      count--;
	if(count <= 0)
      return false;
  }
  if(count == 1 && flag)
    return true;
  else
    return false;
}

uint32_t eval(int p, int q) {
  //printf("each time: %d %d\n", p, q);

  if(p > q) {
	//printf("%s %s\n", p, q);
	assert(0);
    return false;
  }
  else if(p == q) {
	if(tokens[p].type == TK_NUM)
      return atoi(tokens[p].str);
	else if(tokens[p].type == TK_XNUM)
	  return strtol(tokens[p].str, NULL, 16);
	else if(tokens[p].type == TK_REG) {
	  if(strcmp(tokens[p].str, "\%eax")==0)
		return cpu.eax;
	  else if(strcmp(tokens[p].str, "\%ecx")==0)
	    return cpu.ecx;
	  else if(strcmp(tokens[p].str, "\%edx")==0)
	    return cpu.edx;
	  else if(strcmp(tokens[p].str, "\%ebx")==0)
	    return cpu.ebx;
	  else if(strcmp(tokens[p].str, "\%esp")==0)
	    return cpu.esp;
	  else if(strcmp(tokens[p].str, "\%ebp")==0)
	    return cpu.ebp;
	  else if(strcmp(tokens[p].str, "\%esi")==0)
	    return cpu.esi;
	  else if(strcmp(tokens[p].str, "\%edi")==0)
	    return cpu.edi;
      else
		printf("Can't find the target register!\n");
	}
  }
  else if(check_parentheses(p,q) == true) {
	return eval(p + 1, q - 1);
  }
  else {
	int priority = -1, flag = 0;
	int op = p, tmp;
	for(tmp = p; tmp <= q; tmp++) {
	  if(tokens[tmp].type == '(') {
		flag++;
	  } 
	  else if(tokens[tmp].type == ')') {
		flag--;
	  } 
	  else if(flag == 0) {
	    for(int i = 0; i <= OP_NUM - 1; i++) {
	      if(tokens[tmp].type == op_prio_list[i].type) {
		    if(priority <= op_prio_list[i].rank) {
              op = tmp;
		      priority = op_prio_list[i].rank;
		     }
		    break;
           }
        } 
	  }  
	}  
	int val1 = 0, val2 = 0;
	if(tokens[op].type != TK_DEREF) {
	  val1 = eval(p, op - 1);
	  val2 = eval(op + 1, q);
	}
	else 
      val1 = eval(op + 1, q);
	switch (tokens[op].type) {
      case '-': return val1 - val2; break;
	  case '+': return val1 + val2; break;
	  case '*': return val1 * val2; break;
	  case '/': return val1 / val2; break;
	  case TK_EQ: return val1 == val2; break;
	  case TK_NOTEQ: return val1 != val2; break;
	  case TK_AND: return val1 && val2; break;
	  case TK_GE: return val1 >= val2; break;
	  case TK_LE: return val1 <= val2; break;
	  case TK_LSHIFT: return val1 << val2; break;
	  case TK_RSHIFT: return val1 >> val2; break;
	  case TK_DEREF: return *((int*)((long)val1)); break;
	  default: assert(0);
	}
  }
  return 0;
}

