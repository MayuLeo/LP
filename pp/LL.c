#include "token-list.h"
extern int token;
int parse_program() {
  printf("Token %d\n",token);
  if(token != TPROGRAM) return(error("Keyword 'program' is not found"));
  token = scan();
  if(token != TNAME) return(error("Program name is not found"));
  token = scan();
  if(token != TSEMI) return(error("Semicolon is not found"));
  token = scan();
  if(block() == ERROR) return(ERROR);
  if(token != TDOT) return(error("Period is not found at the end of program"));
  token = scan();
  return(NORMAL);
}
int block()//ブロック{ 変数宣言部 | 副プログラム宣言 } 複合文
{
  printf("block\n");
  while(token == TPROCEDURE || token == TVAR)
  {
    if(token == TPROCEDURE)
    {
      if(subprogram_declaration() == ERROR) return(ERROR);
    }
    if(token == TVAR)
    {
      if(variable_declaration() == ERROR) return(ERROR);
    }

  }
  if(compound_statement() == ERROR) return(ERROR);

  return(NORMAL);
}
int variable_declaration()//変数宣言文:"var" 変数名の並び ":" 型 ";" { 変数名の並び ":" 型 ";" }
{
  printf("variable_declaration\n");
  if(token != TVAR) return(error("var is not found"));
  token = scan();
  if(variable_names() == ERROR) return(ERROR);
  token = scan();
  if(token != TCOLON) return(error(": is not found"));
  token = scan();
  if(type() == ERROR) return(ERROR);
  token = scan();
  if(token != TSEMI) return(error("; is not found"));
  token = scan();



  while(1){
    if(variable_names() == ERROR) break;
    token = scan();
    if(token != TCOLON) return(error(": is not found"));
    token = scan();
    if(type() == ERROR) return(ERROR);
    token = scan();
    if(token != TSEMI) return(error("; is not found"));
    token = scan();
  }


  return(NORMAL);
}
int variable_names()//変数名の並び
{
  return(NORMAL);
}


int subprogram_declaration()//副プログラム宣言:"procedure" 手続き名 [ 仮引数部 ] ";" [ 変数宣言部 ] 複合文 ";"
{
  if (token != TPROCEDURE) return(error("procedure is not found."));
  token = scan();
  //if (token != TNAME) return(error("procedure name is keyward."));
  if(procedure_name() == ERROR) return(ERROR);
  //token = scan();
  if(token == TLPAREN)
  {
    if(formal_parameters() == ERROR) return(ERROR);
    token = scan();

  }
  if(token != TSEMI) return(error("semicolon is not found"));
  token = scan();
  if(token == TVAR)
  {
    if(variable_declaration() == ERROR) return(ERROR);
    //token = scan();
  }
  if(compound_statement() == ERROR) return(ERROR);

  //token = scan();
  if(token != TSEMI) return(error("semicolon is not found"));
  token = scan();
  /*
  printf("token : %d\n",token);
  if(token != TCOLON) return(error("colon is not found"));
  token = scan();
  if(token == TVAR)
  {
    if(variable_declaration() == ERROR) return(ERROR);
    token = scan();
  }

  if(compound_statement() == ERROR) return(ERROR);
  token = scan();
  if(token != TSEMI) return(error("semicolon is not found"));
  token = scan();
  */
  return(NORMAL);
}
int compound_statement()//複合文:"begin" 文 { ";" 文 } "end"
{
  if(token != TBEGIN) return(error("begin is not found"));
  token = scan();
  if(statement() == ERROR) return(ERROR);
  //token = scan();

  while(token != TEND)
  {
    if(token != TSEMI) return(error("semicolon is not found"));
    token = scan();
    if(statement() == ERROR) return(ERROR);
    token = scan();
  }
  token = scan();

  return(NORMAL);
}
int formal_parameters()//仮引数部:"(" 変数名の並び ":" 型 { ";" 変数名の並び ":" 型 } ")"
{
  printf("formal_parameters\n");
  if(token != TLPAREN) return(error("( is not found"));
  token = scan();
  if(variable_names() == ERROR) return(ERROR);
  token = scan();
  if(token != TCOLON) return(error(": is not found"));
  token = scan();
  if(type() == ERROR) return(ERROR);
  token = scan();
  while (token != TRPAREN)
  {
    if(token != TSEMI) return(error("; is not found"));
    token = scan();
    if(variable_names() == ERROR) return(ERROR);
    token = scan();
    if(token != TCOLON) return(error(": is not found"));
    token = scan();
    if(type() == ERROR) return(ERROR);
    token = scan();
  }

  return(NORMAL);
}
int procedure_name()//手続き名:"名前"
{
  if(token != TNAME) return(error("Don't name"));
  token = scan();
  return(NORMAL);
}
int statement()//文:代入文 | 分岐文 | 繰り返し文 | 脱出文 | 手続き呼び出し文 | 戻り文 | 入力文 | 出力文 | 複合文 | 空文
{
  //  switch (token) {
  //    case /* value */:
  //  }
  return(NORMAL);
}

int type()//型
{
  return(NORMAL);
}
