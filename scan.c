#include "token-list.h"
FILE *fp;
char cbuf;
int linenum ;
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
int num_attr;
char string_attr[MAXSTRSIZE];
int get_linenum(void){
  printf("%d\n",num_attr);
  return linenum;
}
void end_scan(void){
  fclose(fp);
}
