#include "token-list.h"
FILE *fp;
char cbuf;
int linenum;
int num_attr;
char string_attr[MAXSTRSIZE];
extern char *tokenstr[NUMOFTOKEN + 1];
extern key key_keyword[KEYWORDSIZE];
extern key key_symbol[KEYWORDSIZE];
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
  if(cbuf < 0)return -1;
  linenum = 1;
  init_idtab();
  return 0;
}
int scan(void){
  int i = 0;
  char token[MAXSTRSIZE];

  memset(token, 0, sizeof(token));

  if(cbuf < 0)return -1;
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
      else if(cbuf < 0)
        return -1;
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
      if(cbuf < 0)return -1;

      if (!((cbuf >= 65 && cbuf <= 90) || (cbuf >= 97 && cbuf <= 122) || (cbuf >= 48 && cbuf <= 57)))
      {//文字でも数字でもなければ
        
        //snprintf(string_attr,MAXSTRSIZE,"%s",token);

        for(i = 0;i < KEYWORDSIZE;i++)
        {
          if (strcmp(token, key_keyword[i].keyword) == 0)
            return key_keyword[i].keytoken;
        }
        memset(string_attr, 0, sizeof(string_attr));
        snprintf(string_attr, MAXSTRSIZE, "%s", token);
        id_countup(string_attr);
        return TNAME;
        break;
      }
    }
  }
  else if(cbuf >= 48 && cbuf <= 57)//数字
  {
    while(1){
      snprintf(token, MAXSTRSIZE, "%s%c", token, cbuf);
      cbuf = fgetc(fp);
      if(cbuf < 0)return -1;
      if (!(cbuf >= 48 && cbuf <= 57))//数字でなくなったら
      {
        num_attr = atoi(token);
        memset(string_attr, 0, sizeof(string_attr));
        snprintf(string_attr, MAXSTRSIZE, "%s", token);
        return TNUMBER;
        break;
      }
    }
  }
  else if ((cbuf >= 40 && cbuf <= 46) || (cbuf >= 58 && cbuf <= 62) || cbuf == 91 || cbuf == 93)//記号
  {
    snprintf(token, MAXSTRSIZE, "%s%c", token, cbuf);
    cbuf = fgetc(fp);
    if(cbuf < 0)return -1;
    if(cbuf == 61 || cbuf == 62)//<> or <= or >=
    {
      snprintf(token, MAXSTRSIZE, "%s%c", token, cbuf);
      cbuf = fgetc(fp);
      if(cbuf < 0)return -1;
      for(i = 0;i < SYMBOLSIZE;i++)
      {
        if(strcmp(token,key_symbol[i].keyword) == 0)
        {
          return key_symbol[i].keytoken;
        }
      }
    }
    else
    {
      for (i = 0; i < SYMBOLSIZE; i++)
      {
        if (strcmp(token, key_symbol[i].keyword) == 0)
        {
          return key_symbol[i].keytoken;
        }
      }
    }
  }
  else if(cbuf == 39)//string 改行が来るまで読み込んだ方がいいかも
  {
    while(1)
    {
      cbuf = fgetc(fp);
      if(cbuf < 0)return -1;

      if (cbuf == 39){
        cbuf = fgetc(fp);
        if(cbuf < 0)return -1;
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
      if(cbuf < 0)return -1;
      if (cbuf == 125)
      {
        cbuf = fgetc(fp);
        if(cbuf < 0)return -1;
        return 0;
      }
    }
  }
  else if(cbuf == 47)// /
  {
    cbuf = fgetc(fp);
    if(cbuf < 0)return -1;
    if(cbuf == 42)// *
    {
      while (1)
      {
        cbuf = fgetc(fp);//コメント文中身
        if(cbuf < 0)return -1;
        if (cbuf == 42) // *
        {
          cbuf = fgetc(fp);
          if(cbuf < 0)return -1;
          if(cbuf == 47)// /
          {
            return 0;
          }
        }
      }
    }
  }
  return -1;
}

int get_linenum()
{
  return linenum;
}
void end_scan(void){
  fclose(fp);
}
