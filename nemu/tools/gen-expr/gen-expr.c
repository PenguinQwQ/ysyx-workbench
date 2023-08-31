/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = (unsigned int) 1 * %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";
int choose(int n)
{
  return rand() % n;
}
int top;//top of the buf
const int MAX_LEN = 50000;
bool next_not_zero = false;
void gen_num() //num is 
{
  bool insert_blank = rand() % 10 > 5;
  if(insert_blank) buf[top++] = ' ';
  buf[top] = rand() % 10 + '0';
  if(next_not_zero == false && buf[top] == '0') 
    {
      top++;
      return;
    } //just summon a zero
  
  if(next_not_zero) 
    {
      buf[top] = (buf[top] == '0') ? (buf[top] + 1) : buf[top];//Not begin with 0
      next_not_zero = false;
    }
  top++;
  int len = rand() % 8;//len is 0 to 7
  while(len--) buf[top++] = rand() % 10 + '0'; 
    if(insert_blank) buf[top++] = ' ';
}
void gen(char ch)
{
  bool insert_blank = rand() % 10 > 5;
  if(insert_blank) buf[top++] = ' ';
  buf[top++] = ch;
  if(insert_blank) buf[top++] = ' ';
}

void gen_rand_op()
{
  bool insert_blank = rand() % 10 > 5;
  if(insert_blank) buf[top++] = ' ';
  int op = rand() % 4;
  switch (op)
  {
  case 0:
    buf[top++] = '+';
    break;
  case 1:
    buf[top++] = '-';
    break;
  case 2:
    buf[top++] = '*';
    break;
  default:
    buf[top++] = '/';
    next_not_zero = true;
    break;
  }
  if(insert_blank) buf[top++] = ' ';
}

static void gen_rand_expr() {
  //at least 2 length
  bool insert_blank = rand() % 10 > 5;
  if(insert_blank) buf[top++] = ' ';
   switch (choose(3)) {
    case 0: gen_num(); break;
    case 1: 
        if(top <= MAX_LEN)
        {
          gen('('); 
          gen_rand_expr(); 
          gen(')');
        } 
        else gen_num();
        break;
    default: 
        if(top <= MAX_LEN)
        {
          gen_rand_expr(); 
          gen_rand_op(); 
          gen_rand_expr(); 
        }
        else gen_num();
      break;
  }
  if(insert_blank) buf[top++] = ' ';
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    memset(buf, 0, sizeof(buf));
    top = 0;
    gen_rand_expr(); //just summon 0~2000 length expression
    buf[++top] = '\0';
    sprintf(code_buf, code_format, buf);
    
    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
