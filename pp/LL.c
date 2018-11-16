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
int exit_statement()//脱出文:TBREAK
{
  if(token != TBREAK) return(error("break is not found"));
  token = scan();
  return(NORMAL);
}
int call_statement()//手続き呼び出し文:"call" 手続き名 [ "(" 式の並び ")" ]
{
  if(token != TCALL) return(error("call is not found"));
  token = scan();
  if(procedure_name() == ERROR) return(ERROR);
  if(token == TLPAREN)
  {
    token = scan();
    if(expressions() == ERROR) return(ERROR);
    if(token != TRPAREN) return(error(") is not found"));
    token = scan();
  }
  return(NORMAL);
}
int expressions()//式の並び:式 { "," 式 }
{
  if(expression() == ERROR) return(ERROR);
  while (token == TCOMMA) {
    if(token != TCOMMA) return(error("commma is not found"));
    token = scan();
    if(expression() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int return_statement()//戻り文:"return"
{
  if(token != TRETURN) return(error("return is not found"));
  token = scan();
  return(NORMAL);
}
int assignment_statement()//代入文:左辺部 ":=" 式
{
  if(left_part() == ERROR) return(ERROR);
  if(token != TASSIGN) return(error(":= is not found"));
  token = scan();
  if(expression() == ERROR) return(ERROR);
  return(NORMAL);
}
int left_part()//左辺部:変数
{
  if(variable() == ERROR) return(ERROR);
  return(NORMAL);
}
int variable()//変数
{
  if(variable_name() == ERROR) return(ERROR);
  return(NORMAL);
}
int expression()//式;単純式 { 関係演算子 単純式 }
{
  if(simple_expression() == ERROR) return(ERROR);
  while (relational_operator() == NORMAL) {
    if(simple_expression() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int simple_expression()//単純式[ "+" | "-" ] 項 { 加法演算子 項 }
{
  if(token == TPLUS || token == TMINUS)
  {
    token = scan();
  }
  if(term() == ERROR) return(ERROR);
  while (additive_operator() == NORMAL) {
    if(term() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int term()//項: 因子 { 乗法演算子 因子 }
{
  if(factor() == ERROR) return(ERROR);
  while (multiplicative_operator() == NORMAL) {
    if(factor() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int factor()//因子:変数 |定数 | "("式 ")" | "not"因子 |標準型 "("式 ")"
{
  //名前 or 数字 or "(" or "not" or "integer" | "boolean" | "char"
  switch (token) {
    case TNAME:
      if(variable() == ERROR) return(ERROR);
      break;
    case TNUMBER:
    case TFALSE:
    case TTRUE:
    case TSTRING:
      if(constant() == ERROR) return(ERROR);
      break;
    case TLPAREN:
      if(token != TLPAREN) return(error("( is not found"));
      token = scan();
      if(expression() == ERROR) return(ERROR);
      if(token != TRPAREN) return(error(") is not found"));
      token = scan();
      break;
    case TNOT;
      if(token != TNOT) return(error("not is not found"));
      token = scan();
      if(factor() == ERROR) return(ERROR);
      break;
    case TINTEGER:
    case TBOOLEAN:
    case TCHAR:
      if(standard_type() == ERROR) return(ERROR);
      if(token != TLPAREN) return(error("( is not found"));
      token = scan();
      if(expression() == ERROR) return(ERROR);
      if(token != TRPAREN) return(error(") is not found"));
      token = scan();
      break;
    default: return(error("factor error"));break;
  }
  return(NORMAL);
}
int constant()//定数
{
  switch (token) {
    case TNUMBER:
      if(token != TNUMBER) return(error("number is not found"));
      token = scan();
      break;
    case TFALSE:
      if(token != TFALSE) return(error("false is not found"));
      token = scan();
      break;
    case TTRUE;
      if(token != TTRUE) return(error("true is not found"));
      token = scan();
      break;
    case TSTRING:
      if(token != TSTRING) return(error("string is not found"));
      token = scan();
      break;
    default: return(error("constant error"));break;
  }
  return(NORMAL);
}
int multiplicative_operator()//乗法演算子
{
  switch (token) {
    case TSTAR:
    if(token != TSTAR) return(error("* is not found"));
    token = scan();
      break;
    case TDIV:
      if(token != TDIV) return(error("div is not found"));
      token = scan();
      break;
    case TAND:
      if(token != TAND) return(error("and is not found"));
      token = scan();
      break;
    default: return(error("multiplicative_operator error"));break;
  }
  return(NORMAL);
}
int additive_operator()//加法演算子
{
  switch (token) {
    case TPLUS:
      if(token != TPLUS) return(error("+ is not found"));
      token = scan();
      break;
    case TMINUS:
      if(token != TMINUS) return(error("- is not found"));
      token = scan();
      break;
    case TOR:
      if(token != TOR) return(error("or is not found"));
      token = scan();
      break;
    default: return(error("additive_operator error"));break;
  }
  return(NORMAL);
}
int relational_operator()//関係演算子:"=" | "<>" | "<" | "<=" | ">" | ">="
{
   switch (token) {
     case TEQUAL:
       if(token != TEQUAL) return(error("= is not found"));
       token = scan();
      break;
    case TNOTEQ:
      if(token != TNOTEQ) return(error("<> is not found"));
      token = scan();
      break;
    case TLE:
      if(token != TLE) return(error("< is not found"));
      token = scan();
      break;
    case TLEEQ:
      if(token != TLEEQ) return(error("<= is not found"));
      token = scan();
      break;
    case TGR:
      if(token != TGR) return(error("> is not found"));
      token = scan();
      break;
    case TGREQ:
      if(token != TGREQ) return(error(">= is not found"));
      token = scan();
      break;
    default: return(error("relational_operator error"));break;
   }
}
int input_statement()//入力文:("read" | "readln") [ "(" 変数 { "," 変数 } ")" ]
{
  if(token == TREAD)
  {
    token = scan();
  }
  else if(token == TREADLN)
  {
    token = scan();
  }
  else
  {
    return( error("read or readln is not found"));
  }

  if(token != TLPAREN)
  {
    token = scan();
    if(variable() == ERROR) return(ERROR);

    while (token == TCOMMA) {
      token = scan();
      if(variable() == ERROR) return(ERROR);

    }
    if(token != TRPAREN) return(") is not found");
    token = scan();
  }
  return(NORMAL);
}
int output_statement()//出力文:("write" | "writeln") [ "(" 出力指定 { "," 出力指定 } ")" ]
{
  if(token == TWRITE)
  {
    token = scan();
  }
  else if(token == TWRITELN)
  {
    token = scan();
  }
  else
  {
    return( error("write or writeln is not found"));
  }

  if(token != TLPAREN)
  {
    token = scan();
    if(output_formal() == ERROR)return(ERROR);
    while (token == TCOMMA) {
      token = scan();
      if(output_formal() == ERROR) return(ERROR);
    }
    if(token != TRPAREN) return(") is not found");
    token = scan();
  }
  return(NORMAL);
}
int output_formal()//出力指定: 式 [ ":" "符号なし整数" ] | "文字列"
{
  if(token == TSTRING)
  {
    token = scan();
  }
  else if(expression() == NORMAL)
  {
    if(token == TCOLON)
    {
      token = scan();
      if(token != TNUMBER) return(error("number is not found"));
      token = scan();
    }
    else
    {
      return(error(": is not found"));
    }
  }
  else
  {
    return(error("output_formal error"));
  }
  return(NORMAL);
}
//int empty_statement()
//{
//  
//}
