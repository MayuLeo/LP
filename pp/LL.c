#include "token-list.h"
extern int token;
int parse_program() {
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
  if(variable_name() == ERROR) return(ERROR);

  while(token == TCOMMA)
  {
    token = scan();
    if(variable_name() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int variable_name()
{
  if(token != TNAME) return(error("name is not found"));
  token = scan();
  return(NORMAL);
}
int type()//型
{
  if(standard_type() == ERROR)
  {
    if(array_type() == ERROR)
    {
      return(error("type error!"));
    }
  }

  return(NORMAL);
}
int standard_type()//標準型
{
  if(token == TINTEGER || token == TBOOLEAN || token == TCHAR)
  {
    token = scan();
    return(NORMAL);
  }
  else return(error("standard_type is not found"));
}
int array_type()//配列型
{
  if(token != TARRAY) return(error("array is not found"));
  token = scan();
  if(token != TLSQPAREN) return(error("[ is not found"));
  token = scan();
  if(token != TNUMBER) return(error("number is not found"));
  token = scan();
  if(token != TRSQPAREN) return(error("] is not found"));
  token = scan();
  if(token != TOF) return(error("of in not found"));
  if(standard_type() == ERROR) return(ERROR);
  return(NORMAL);
}
int subprogram_declaration()//副プログラム宣言:"procedure" 手続き名 [ 仮引数部 ] ";" [ 変数宣言部 ] 複合文 ";"
{
  printf("subprogram_declaration\n");
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
int procedure_name()//手続き名:"名前"
{
  if(token != TNAME) return(error("Don't name"));
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
int compound_statement()//複合文:"begin" 文 { ";" 文 } "end"
{
  printf("START compound_statement\n");
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
  printf("END compound_statement\n");
  return(NORMAL);
}


int statement()//文:代入文 | 分岐文 | 繰り返し文 | 脱出文 | 手続き呼び出し文 | 戻り文 | 入力文 | 出力文 | 複合文 | 空文
{
  //  switch (token) {
  //    case /* value */:
  //  }
  return(NORMAL);
}
int condition_statement()//分岐文: "if" 式 "then" 文 [ "else" 文 ]
{
    if(token != TIF) return(error("if is not found"));
    token = scan();
    if(expression() == ERROR) return(ERROR);
    if(token != TTHEN) return(error("then is not fuond"));
    token = scan();
    if(statement() == ERROR) return(ERRPR);
    if(token == TELSE)
    {
      token = scan();
      if(statement() == ERROR)return(ERROR);
    }
    return(NORMAL);
}
int iteration_statement()//繰り返し文:"while" 式 "do" 文
{
  if(token != TWHILE) return(error("while is not found"));
  token = scan();
  if(expression() == ERROR) return(ERROR);
  if(token != TDO) return(error("do is not found"));
  token = scan();
  if(statement() == ERROR) return(ERROR);
  return(NORMAL);
}
