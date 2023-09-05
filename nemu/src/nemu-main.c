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

#include <common.h>
#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/mman.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
word_t expr(char *e, bool *success);
char buf_res[20], buf_expr[65536];
int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* Start engine. */

  /* Expression Automatic Testing*/
  /*
    FILE *fp;
    uint32_t len;
    char *buffer;

    fp = fopen ( "input" , "rb" );
    if( !fp ) perror("input"),exit(1);

    fseek( fp , 0L , SEEK_END);
    len = ftell( fp );
    rewind( fp );


    buffer = calloc( 1, len + 1 );
    if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);


    if( 1!=fread( buffer , len, 1 , fp) )
      fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);


 // Log("%s", buffer);
  bool suc = false;
  FILE *vlog = fopen( "expr_verification.txt", "w");
  for (int i = 1 ; i <= 10000 ; i++)
    { 
      memset(buf_res, 0, sizeof(buf_res));
      memset(buf_expr, 0, sizeof(buf_expr));
      int len = 0;
      while(*buffer != ' ') buf_res[len++] = *(buffer++);
      buffer++; //To avoid ' '
      len = 0;
      while(*buffer != '\n') buf_expr[len++] = *(buffer++);
      buffer++; //To avoid '\n'
      uint32_t result = strtol(buf_res, NULL, 10);
      Log("expression is %s\n", buf_expr);
      uint32_t res = expr(buf_expr, &suc);
      Assert(suc == true, "Failed to load the expression");
      if(result == res)
      {
        Log("Pass!!! The difftest result is %"PRIx32" and expr result is %"PRIx32, result, res);
        fprintf(vlog, "Pass!!! The difftest resultis %"PRIx32" and expr result is %"PRIx32"\n", result, res);
      }
      else
      {
        Log("Wrong Answer!!! The difftest result is %"PRIx32" and expr result is %"PRIx32, result, res);
        fprintf(vlog, "Wrong Answer!!! The difftest result is %"PRIx32" and expr result is %"PRIx32"\n", result, res);
      }
    }
    fclose(fp);
    fclose(vlog);
  */
  engine_start();

  return is_exit_status_bad();
 // return 0;
}
