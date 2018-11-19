#include "token-list.h"
extern int token;
extern int linenum;
extern char *tokenstr[NUMOFTOKEN + 1];
extern int tabnum;
int is_begin_line = 1;//現在のtokenが文の先頭なら1．
int next_token()
{//TODO 現在のタブ数を格納する変数
  //タブ数の計算 → タブの挿入 → 文字の表示
  int before_token = token;
  int token_num = scan();
  if(token_num == -1)
    return token_num;
  else if(token_num == TPROCEDURE)
  {
    tabnum = 1;
  }
  else if(token_num == TEND)
  {
    printf("\n");
    is_begin_line = 1;
    tabnum--;
  }

  if(is_begin_line)
  {
    is_begin_line = 0;
    for(int i = 0;i < tabnum * 4;i++)
      printf(" ");
  }



//文の頭のタブ問題の解決
//;無しで改行→1段下がる

  if(token_num != TNAME && token_num != TNUMBER && token_num != TSTRING)
  {
    printf("%s",tokenstr[token_num]);
  }
  else
    printf("%s",string_attr);

  if(token_num == TBEGIN)
  {
    tabnum++;
    is_begin_line = 1;
    printf("\n");
  }
  if(token_num == TSEMI)
  {
    printf("\n");
    is_begin_line = 1;
  }

  return token_num;
}
int parse_program() {
  if(token != TPROGRAM) return(error("Keyword 'program' is not found"));
  token = next_token();
  if(token != TNAME) return(error("Program name is not found"));
  token = next_token();
  if(token != TSEMI) return(error("Semicolon is not found"));
  token = next_token();
  //printf("token = %d\n",token);
  if(block() == ERROR) return(ERROR);
  if(token != TDOT) return(error("Period is not found at the end of program"));
  token = next_token();
  return(NORMAL);
}
int block()//ブロック{ 変数宣言部 | 副プログラム宣言 } 複合文
{
  //printf("block\n");
  //printf("token = %d\n",token);
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
  //printf("start variable_declaration\n");
  if(token != TVAR) return(error("var is not found"));
  token = next_token();
  if(variable_names() == ERROR) return(ERROR);
  //token = next_token();
  if(token != TCOLON) return(error(": is not found"));
  token = next_token();
  if(type() == ERROR) return(ERROR);
  //token = next_token();
  if(token != TSEMI) return(error("; is not found"));
  token = next_token();



  while(token == TNAME){
    if(variable_names() == ERROR) break;
    //token = next_token();
    if(token != TCOLON) return(error(": is not found"));
    token = next_token();
    if(type() == ERROR) return(ERROR);
    //token = next_token();
    if(token != TSEMI) return(error("; is not found"));
    token = next_token();
  }

  //printf("end variable_declaration\n");
  return(NORMAL);
}
int variable_names()//変数名の並び
{
  if(variable_name() == ERROR) return(ERROR);

  while(token == TCOMMA)
  {
    token = next_token();
    if(variable_name() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int variable_name()
{
  if(token != TNAME) return(error("name is not found"));
  token = next_token();
  return(NORMAL);
}
int type()//型
{
  if(token == TINTEGER || token == TBOOLEAN || token == TCHAR)
  {
    //token = next_token();
    //printf("standard_type\n");
    if(standard_type() == ERROR) return(ERROR);
  }
  else if(token == TARRAY)
  {
    //token = next_token();
    //printf("array_type\n");
    if(array_type() == ERROR) return(ERROR);
  }
  else
    return(error("type error"));
  //if(standard_type() == ERROR)
  //{
  //  if(array_type() == ERROR)
  //  {
  //    return(error("type error!"));
  //  }
  //}

  return(NORMAL);
}
int standard_type()//標準型
{
  if(token == TINTEGER || token == TBOOLEAN || token == TCHAR)
  {
    token = next_token();
    return(NORMAL);
  }
  else return(error("standard_type is not found"));
}
int array_type()//配列型
{
  //printf("start array_type\n");
  if(token != TARRAY) return(error("array is not found"));
  token = next_token();
  if(token != TLSQPAREN) return(error("[ is not found"));
  token = next_token();
  if(token != TNUMBER) return(error("number is not found"));
  token = next_token();
  if(token != TRSQPAREN) return(error("] is not found"));
  token = next_token();
  if(token != TOF) return(error("of in not found"));
  token = next_token();
  if(standard_type() == ERROR) return(ERROR);
  //printf("end array_type\n");
  return(NORMAL);
}
int subprogram_declaration()//副プログラム宣言:"procedure" 手続き名 [ 仮引数部 ] ";" [ 変数宣言部 ] 複合文 ";"
{
  //printf("START subprogram_declaration\n");
  if (token != TPROCEDURE) return(error("procedure is not found."));
  token = next_token();
  //if (token != TNAME) return(error("procedure name is keyward."));
  if(procedure_name() == ERROR) return(ERROR);
  //token = next_token();
  if(token == TLPAREN)
  {
    if(formal_parameters() == ERROR) return(ERROR);
    token = next_token();

  }
  if(token != TSEMI) return(error("semicolon is not found"));
  token = next_token();
  if(token == TVAR)
  {
    if(variable_declaration() == ERROR) return(ERROR);
    //token = next_token();
  }
  if(compound_statement() == ERROR) return(ERROR);

  if(token != TSEMI) return(error("semicolon is not found"));
  token = next_token();
  /*
  printf("token : %d\n",token);
  if(token != TCOLON) return(error("colon is not found"));
  token = next_token();
  if(token == TVAR)
  {
    if(variable_declaration() == ERROR) return(ERROR);
    token = next_token();
  }

  if(compound_statement() == ERROR) return(ERROR);
  token = next_token();
  if(token != TSEMI) return(error("semicolon is not found"));
  token = next_token();
  */
  //printf("END subprogram_declaration\n");
  return(NORMAL);
}
int procedure_name()//手続き名:"名前"
{
  if(token != TNAME) return(error("Don't name"));
  token = next_token();
  return(NORMAL);
}
int formal_parameters()//仮引数部:"(" 変数名の並び ":" 型 { ";" 変数名の並び ":" 型 } ")"
{
  //printf("start formal_parameters\n");
  if(token != TLPAREN) return(error("( is not found"));
  token = next_token();
  if(variable_names() == ERROR) return(ERROR);
  //token = next_token();
  if(token != TCOLON) return(error(": is not found"));
  token = next_token();
  if(type() == ERROR) return(ERROR);
  //token = next_token();
  while (token != TRPAREN)
  {
    if(token != TSEMI) return(error("; is not found"));
    token = next_token();
    if(variable_names() == ERROR) return(ERROR);
    //token = next_token();
    if(token != TCOLON) return(error(": is not found"));
    token = next_token();
    if(type() == ERROR) return(ERROR);
    //token = next_token();
  }
  //printf("end formal_parameters\n");
  return(NORMAL);
}
int compound_statement()//複合文:"begin" 文 { ";" 文 } "end"
{
  //printf("START compound_statement\n");
  if(token != TBEGIN) return(error("begin is not found"));
  token = next_token();
  if(statement() == ERROR) return(ERROR);
  //token = next_token();
  /*
  while(token != TEND)
  {
    if(token != TSEMI) return(error("semicolon is not found"));
    token = next_token();
    if(statement() == ERROR) return(ERROR);
    //token = next_token();
  }
  */
  while (token == TSEMI) {
    token = next_token();
    //printf("compound_statement -> statement\n");
    if(statement() == ERROR) return(ERROR);
  }
  //printf("token = %d\n",token);
  //printf("@@@@@@@@@@@@\n");
  if(token != TEND) return(error("end is not found"));
  token = next_token();
  //printf("token = %d\n",token);
  //printf("linenum = %d\n",linenum);
  //printf("END compound_statement\n");
  return(NORMAL);
}


int statement()//文:代入文 | 分岐文 | 繰り返し文 | 脱出文 | 手続き呼び出し文 | 戻り文 | 入力文 | 出力文 | 複合文 | 空文
{
  //代入文 "名前" | 分岐文 "if" | 繰り返し文 "while" | 脱出文 "break" | 手続き呼び出し文 "call" | 戻り文 "return" | 入力文 "read" or "readln" | 出力文 "write" or "writeln" |複合文 "begin" | 空文 ""
  //printf("statement token = %d\n",token);
  //printf("statement linenum = %d\n",linenum);
  //printf("@@@@@@@@@@@@@@@@@@@@@@@\n");
  switch (token) {
    case TNAME://代入文
      //printf("文->代入文\n");
      if(assignment_statement() == ERROR) return(ERROR);
      break;
    case TIF://分岐文
      //printf("文->分岐文\n");
      if(condition_statement() == ERROR) return(ERROR);
      break;
    case TWHILE://繰り返し文
      //printf("文->繰り返し文\n");
      if(iteration_statement() == ERROR) return(ERROR);
      break;
    case TBREAK://脱出文
      //printf("文->脱出文\n");
      if(exit_statement() == ERROR) return(ERROR);
      break;
    case TCALL://手続き呼出文
      //printf("文->手続き呼び出し文\n");
      if(call_statement() == ERROR) return(ERROR);
      break;
    case TRETURN://戻り文
      //printf("文->戻り文\n");
      if(return_statement() == ERROR) return(ERROR);
      break;
    case TREAD:
    case TREADLN://入力文
      //printf("文->入力文\n");
      if(input_statement() == ERROR) return(ERROR);
      break;
    case TWRITE:
    case TWRITELN://出力文
      //printf("文->出力文\n");
      if(output_statement() == ERROR) return(ERROR);
      break;
    case TBEGIN://複合文
      //printf("文->複合文\n");
      if(compound_statement() == ERROR) return(ERROR);
      break;
    //TODO case empty statement
    default: empty_statement();break;
    //default: printf("\nstatement token = %d\n",token);return(error("statement error"));
  }
  return(NORMAL);
}
int condition_statement()//分岐文: "if" 式 "then" 文 [ "else" 文 ]
{
    if(token != TIF) return(error("if is not found"));
    token = next_token();
    if(expression() == ERROR) return(ERROR);
    if(token != TTHEN) return(error("then is not fuond"));
    token = next_token();
    if(statement() == ERROR) return(ERROR);
    if(token == TELSE)
    {
      token = next_token();
      if(statement() == ERROR)return(ERROR);
    }
    return(NORMAL);
}
int iteration_statement()//繰り返し文:"while" 式 "do" 文
{
  //printf("start iteration_statement\n");
  if(token != TWHILE) return(error("while is not found"));
  token = next_token();
  if(expression() == ERROR) return(ERROR);
  if(token != TDO) return(error("do is not found"));
  token = next_token();
  if(statement() == ERROR) return(ERROR);
  //printf("end iteration_statement\n");
  return(NORMAL);
}
int exit_statement()//脱出文:TBREAK
{
  if(token != TBREAK) return(error("break is not found"));
  token = next_token();
  return(NORMAL);
}
int call_statement()//手続き呼び出し文:"call" 手続き名 [ "(" 式の並び ")" ]
{
  //printf("start call_statement\n");
  if(token != TCALL) return(error("call is not found"));
  token = next_token();
  if(procedure_name() == ERROR) return(ERROR);
  if(token == TLPAREN)
  {
    token = next_token();
    if(expressions() == ERROR) return(ERROR);
    if(token != TRPAREN) return(error(") is not found"));
    token = next_token();
  }
  //printf("end call_statement\n");
  return(NORMAL);
}
int expressions()//式の並び:式 { "," 式 }
{
  if(expression() == ERROR) return(ERROR);
  while (token == TCOMMA) {
    if(token != TCOMMA) return(error("commma is not found"));
    token = next_token();
    if(expression() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int return_statement()//戻り文:"return"
{
  if(token != TRETURN) return(error("return is not found"));
  token = next_token();
  return(NORMAL);
}
int assignment_statement()//代入文:左辺部 ":=" 式
{
  if(left_part() == ERROR) return(ERROR);
  //printf("token = %d\n",token);
  if(token != TASSIGN) return(error(":= is not found"));
  token = next_token();
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
  if(token == TLSQPAREN)
  {
    token = next_token();
    if(expression() == ERROR) return(ERROR);
    if(token != TRSQPAREN) return(error("] is not found"));
    token = next_token();
  }
  return(NORMAL);
}
int expression()//式;単純式 { 関係演算子 単純式 }
{
  if(simple_expression() == ERROR) return(ERROR);
  //while (relational_operator() == NORMAL) {
  while(token >= TEQUAL && token <= TGREQ)
  {
    token = next_token();
    if(simple_expression() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int simple_expression()//単純式[ "+" | "-" ] 項 { 加法演算子 項 }
{
  if(token == TPLUS || token == TMINUS)
  {
    token = next_token();
  }
  if(term() == ERROR) return(ERROR);
  //while (additive_operator() == NORMAL) {
  while(token == TPLUS || token == TMINUS || token == TOR)
  {
    token = next_token();
    if(term() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int term()//項: 因子 { 乗法演算子 因子 }
{
  if(factor() == ERROR) return(ERROR);

  //while (multiplicative_operator() == NORMAL) {//TODO while文で関数がNORMALかどうかでやるのはよくないかも
  //printf("token = %d\n",token);
  while (token == TSTAR || token == TDIV || token == TAND)
  {
    //token = next_token();
    //printf("token = %d\n",token);
    if(multiplicative_operator() == ERROR) return(ERROR);
    if(factor() == ERROR) return(ERROR);
  }
  return(NORMAL);
}
int factor()//因子:変数 |定数 | "("式 ")" | "not"因子 |標準型 "("式 ")"
{
  //名前 or 数字 or "(" or "not" or "integer" | "boolean" | "char"
  //printf("start factor\n");
  //printf("factor token = %d\n",token);
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
      token = next_token();
      if(expression() == ERROR) return(ERROR);
      if(token != TRPAREN) return(error(") is not found"));
      token = next_token();
      break;
    case TNOT:
      if(token != TNOT) return(error("not is not found"));
      token = next_token();
      if(factor() == ERROR) return(ERROR);
      break;
    case TINTEGER:
    case TBOOLEAN:
    case TCHAR:
      if(standard_type() == ERROR) return(ERROR);
      if(token != TLPAREN) return(error("( is not found"));
      token = next_token();
      if(expression() == ERROR) return(ERROR);
      if(token != TRPAREN) return(error(") is not found"));
      token = next_token();
      break;
    default: return(error("factor error"));break;
  }
  //printf("end factor\n");
  return(NORMAL);
}
int constant()//定数
{
  switch (token) {
    case TNUMBER:
      if(token != TNUMBER) return(error("number is not found"));
      token = next_token();
      break;
    case TFALSE:
      if(token != TFALSE) return(error("false is not found"));
      token = next_token();
      break;
    case TTRUE:
      if(token != TTRUE) return(error("true is not found"));
      token = next_token();
      break;
    case TSTRING:
      if(token != TSTRING) return(error("string is not found"));
      token = next_token();
      break;
    default: return(error("constant error"));break;
  }
  return(NORMAL);
}
int multiplicative_operator()//乗法演算子
{
  //printf("token = %d\n",token);
  switch (token) {
    case TSTAR:
    if(token != TSTAR) return(error("* is not found"));
    token = next_token();
      break;
    case TDIV:
      if(token != TDIV) return(error("div is not found"));
      token = next_token();
      break;
    case TAND:
      if(token != TAND) return(error("and is not found"));
      token = next_token();
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
      token = next_token();
      break;
    case TMINUS:
      if(token != TMINUS) return(error("- is not found"));
      token = next_token();
      break;
    case TOR:
      if(token != TOR) return(error("or is not found"));
      token = next_token();
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
       token = next_token();
      break;
    case TNOTEQ:
      if(token != TNOTEQ) return(error("<> is not found"));
      token = next_token();
      break;
    case TLE:
      if(token != TLE) return(error("< is not found"));
      token = next_token();
      break;
    case TLEEQ:
      if(token != TLEEQ) return(error("<= is not found"));
      token = next_token();
      break;
    case TGR:
      if(token != TGR) return(error("> is not found"));
      token = next_token();
      break;
    case TGREQ:
      if(token != TGREQ) return(error(">= is not found"));
      token = next_token();
      break;
    default: return(error("relational_operator error"));break;
   }
   return(NORMAL);
}
int input_statement()//入力文:("read" | "readln") [ "(" 変数 { "," 変数 } ")" ]
{
  //printf("start input_statement\n");
  if(token == TREAD)
  {
    token = next_token();
  }
  else if(token == TREADLN)
  {
    token = next_token();
  }
  else
  {
    return( error("read or readln is not found"));
  }

  if(token == TLPAREN)
  {

    token = next_token();
    if(variable() == ERROR) return(ERROR);

    while (token == TCOMMA) {
      token = next_token();
      if(variable() == ERROR) return(ERROR);

    }
    if(token != TRPAREN) return(error(") is not found"));
    token = next_token();
  }
  //printf("end input_statement\n");
  return(NORMAL);
}
int output_statement()//出力文:("write" | "writeln") [ "(" 出力指定 { "," 出力指定 } ")" ]
{
  //printf("start output_statement\n");
  if(token == TWRITE)
  {
    token = next_token();
  }
  else if(token == TWRITELN)
  {
    token = next_token();
  }
  else
  {
    return( error("write or writeln is not found"));
  }
  //printf("writeの識別まで終わった地点\n");
  if(token == TLPAREN)
  {
    token = next_token();
    if(output_formal() == ERROR)return(ERROR);
    //printf("TSTRINGのところまで\n");
    while (token == TCOMMA) {
      token = next_token();
      if(output_formal() == ERROR) return(ERROR);
    }
    if(token != TRPAREN) return(error(") is not found"));
    token = next_token();
  }
  //printf("end output_statement\n");
  return(NORMAL);
}
int output_formal()//出力指定: 式 [ ":" "符号なし整数" ] | "文字列"
{
  //printf("start output_formal\n");
  if(token == TSTRING)
  {
    token = next_token();
  }
  else if(expression() == NORMAL)
  {
    if(token == TCOLON)
    {
      token = next_token();
      if(token != TNUMBER) return(error("number is not found"));
      token = next_token();
    }
  }
  else
  {
    return(error("output_formal error"));
  }
  //printf("end output_formal\n");
  return(NORMAL);
}
int empty_statement()
{
  token = next_token();
  return(NORMAL);
}
