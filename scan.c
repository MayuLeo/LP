#include "token-list.h"
FILE *fp;
char cbuf;
int linenum;
int num_attr;
char string_attr[MAXSTRSIZE];
int init_scan(char *filename)
{
  fp = fopen(filename,"r");
  if (fp == NULL)
  {
    //失敗と表示し終了
    printf("ファイルオープン失敗\n");
    return -1;
  }
  cbuf = fgetc(fp);
  linenum = 0;
  init_idtab();
  return 0;
}
int scan(void){
  //printf("cbuf : %c\n", cbuf);
  //printf("cbuf : %d\n", cbuf);
  //cbuf = fgetc(fp);
  int i = 0;
  printf("####################scan関数\n");
  char token[MAXSTRSIZE];

  memset(token, 0, sizeof(token));


  char *tokenstr[NUMOFTOKEN + 1] = {
      "",
      "NAME", "program", "var", "array", "of", "begin", "end", "if", "then",
      "else", "procedure", "return", "call", "while", "do", "not", "or",
      "div", "and", "char", "integer", "boolean", "readln", "writeln", "true",
      "false", "NUMBER", "STRING", "+", "-", "*", "=", "<>", "<", "<=", ">",
      ">=", "(", ")", "[", "]", ":=", ".", ",", ":", ";", "read", "write", "break"};

  if(cbuf < 0)
    return -1;
  if(cbuf <= 32){
    while(1)
    {

      if ((cbuf == 13) || (cbuf == 10))
      {
        cbuf = fgetc(fp);
        if ((cbuf == 13) || (cbuf == 10))
        {
          cbuf = fgetc(fp);//CRLF
        }
        linenum++;
      }
      else
        cbuf = fgetc(fp);
      if (cbuf >= 39)
        break;
    }
  }

  //1文字目がcbufに入ってる
  if((cbuf >= 65 && cbuf <= 90) || (cbuf >= 97 && cbuf <= 122))//アルファベット
  {
    while(1){
      //strcat(token,cbuf);
      snprintf(token,MAXSTRSIZE,"%s%c",token,cbuf);

      cbuf = fgetc(fp);

      if (!((cbuf >= 65 && cbuf <= 90) || (cbuf >= 97 && cbuf <= 122) || (cbuf >= 48 && cbuf <= 57)))
      {//文字でも数字でもなければ
        memset(string_attr, 0, sizeof(string_attr));
        snprintf(string_attr,MAXSTRSIZE,"%s",token);

        if(strcmp(string_attr,"program")==0)  return TPROGRAM;
        else if(strcmp(string_attr,"var")==0)  return TVAR;
        else if(strcmp(string_attr,"array")==0) return TARRAY;
        else if(strcmp(string_attr,"of")==0) return TOF;
        else if(strcmp(string_attr,"begin")==0) return TBEGIN;
        else if(strcmp(string_attr,"end")==0) return TEND;
        else if(strcmp(string_attr,"if")==0) return TIF;
        else if(strcmp(string_attr,"then")==0) return TTHEN;
        else if(strcmp(string_attr,"else")==0) return TELSE;
        else if(strcmp(string_attr,"procedure")==0) return TPROCEDURE;
        else if(strcmp(string_attr,"return")==0) return TRETURN;
        else if(strcmp(string_attr,"call")==0) return TCALL;
        else if(strcmp(string_attr,"while")==0) return TWHILE;
        else if(strcmp(string_attr,"do")==0) return TDO;
        else if(strcmp(string_attr,"not")==0) return TNOT;
        else if(strcmp(string_attr,"or")==0) return TOR;
        else if(strcmp(string_attr,"div")==0) return TDIV;
        else if(strcmp(string_attr,"and")==0) return TAND;
        else if(strcmp(string_attr,"char")==0) return TCHAR;
        else if(strcmp(string_attr,"integer")==0) return TINTEGER;
        else if(strcmp(string_attr,"boolean")==0) return TBOOLEAN;
        else if(strcmp(string_attr,"readln")==0) return TREADLN;
        else if(strcmp(string_attr,"writeln")==0) return TWRITELN;
        else if(strcmp(string_attr,"true")==0) return TTRUE;
        else if(strcmp(string_attr,"false")==0) return TFALSE;
        else 
        {
          id_countup(string_attr);
          return TNAME;
        }

        break;
      }
    }
  }
  else if(cbuf >= 48 && cbuf <= 57)//数字
  {
    while(1){
      snprintf(token, MAXSTRSIZE, "%s%c", token, cbuf);
      cbuf = fgetc(fp);
      if (!(cbuf >= 48 && cbuf <= 57))//数字でなくなったら
      {
        //memset(num_attr, 0, sizeof(num_attr));
        num_attr = atoi(token);
        //snprintf(num_attr, MAXSTRSIZE, "%s", token);
        return TNUMBER;
        break;
      }
    }
  }
  else if ((cbuf >= 40 && cbuf <= 46) || (cbuf >= 58 && cbuf <= 62) || cbuf == 91 || cbuf == 93)//記号
  {
    snprintf(token, MAXSTRSIZE, "%s%c", token, cbuf);
    cbuf = fgetc(fp);
    if(cbuf == 61 || cbuf == 62)//<> or <= or >=
    {
      snprintf(token, MAXSTRSIZE, "%s%c", token, cbuf);
      cbuf = fgetc(fp);
      printf("2記号 token = %s\n", token);
    }
    else
      printf("1記号 token = %s\n", token);
  }
  else if(cbuf == 39)
  {
    while(1)
    {
      printf("cbuf : %c\n", cbuf);
      printf("cbuf : %d\n", cbuf);
      cbuf = fgetc(fp);

      if (cbuf == 39){
        cbuf = fgetc(fp);
        printf("string飛ばし読み\n");
        memset(string_attr, 0, sizeof(string_attr));
        snprintf(string_attr, MAXSTRSIZE, "%s", token);
        return TSTRING;
      }

    }
  }
  else if(cbuf == 123)//{}コメント文
  {
    while(1)
    {
      cbuf = fgetc(fp);
      if (cbuf == 125)
      {
        cbuf = fgetc(fp);
        printf("コメント読み込み\n");
        return 0;
      }
    }
  }
  else if(cbuf == 47)// /
  {
    cbuf = fgetc(fp);
    if(cbuf == 42)// *
    {
      while (1)
      {
        cbuf = fgetc(fp);//コメント文中身
        if (cbuf == 42) // *
        {
          cbuf = fgetc(fp);
          if(cbuf == 47)// /
          {
            return 0;
          }
        }
      }
    }
  }

  printf("%lu\n", strlen(token));
  printf("token : %s\n", token);

  for(i = 0;i < NUMOFTOKEN + 1;i++)//トークン番号を返す．
  {
    if(strcmp(tokenstr[i],token) == 0)
    {
      break;
    }
  }
  if(i == 50)
    i = 1;
  return i;
}

int get_linenum(void){
  printf("%d\n",num_attr);
  return linenum;
}
void end_scan(void){
  fclose(fp);
}
