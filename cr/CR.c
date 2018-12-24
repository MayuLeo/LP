#include "token-list.h"
extern char current_proce_name[MAXSTRSIZE];
extern char string_attr[MAXSTRSIZE];
struct TYPE{
  int ttype; /* TPINT TPCHAR TPBOOL TPARRAY TPARRAYINT TPARRAYCHAR TPARRAYBOOL TPPROC */
  int arraysize; /* size of array, if TPARRAY */
  struct TYPE *etp; /* pointer to element type if TPARRAY */
  struct TYPE *paratp; /* pointer to parameter's type list if ttype is TPPROC */
};
struct LINE{
  int reflinenum;
  struct LINE *nextlinep;
};
struct ID{
  char *name;
  char *procname; /* procedure name within which this name is defined */ /* NULL if global name */
  struct TYPE *itp;
  int ispara; /* 1:formal parameter, 0:else(variable) */
  int deflinenum;//宣言された行数
  struct LINE *irefp;
  struct ID *nextp;
} * globalidroot, *localidroot; /* Pointers to root of global & local symbol tables */
void init_globalcr()
{ /* Initialise the table */
  globalidroot = NULL;
}
void init_localcr()
{ /* Initialise the table */
  localidroot = NULL;
}

struct ID *search_globalcr(char *np)
{ /* search the name pointed by np */
  struct ID *p;
  for (p = globalidroot; p != NULL; p = p->nextp)
  {
    if (strcmp(np, p->name) == 0)
      return (p);
  }
  return (NULL);
}
struct ID *search_localcr(char *np)
{ /* search the name pointed by np */
  struct ID *p;

  for (p = localidroot; p != NULL; p = p->nextp)
  {
    if (strcmp(np, p->name) == 0)
      return (p);
  }
  return (NULL);
}
void cr_globalDeclaration() //変数宣言時変数だけ
{
  struct ID *newid;
  //struct TYPE *newtype;
  //struct LINE *newline;
  if ((newid = (struct ID *)malloc(sizeof(struct ID))) == NULL)
  {
    printf("can not malloc in cr_globalcountup : newid\n");
    return;
  }
  if((newid->name = (char *)malloc(strlen(string_attr) + 1)) == NULL)
    printf("can not malloc globalDeclaration\n");
  else
  {
    strcpy(newid->name,string_attr);
  }
  //newid->name = string_attr;
  newid->procname = NULL; //副プログラムではないので
  newid->itp = NULL;//とりあえずNULLにしておく
  //ifパラメータが存在したらispraを1，ひとまず0に
  newid->ispara = 0;
  newid->deflinenum = get_linenum();
  newid->irefp = NULL;
  newid->nextp = globalidroot;
  globalidroot = newid;
}
void cr_globalsettype(int type)
{
  
  struct ID *p;
  for(p = globalidroot;p != NULL;p = p->nextp)
  {
    if(p->itp == NULL)
    {
      struct TYPE *newtype;
      if ((newtype = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
      {
        printf("can not malloc in cr_globalsettype\n");
        return;
      }
      if (type == TCHAR)
        newtype->ttype = TPCHAR;
      else if (type == TINTEGER)
        newtype->ttype = TPINT;
      else if (type == TBOOLEAN)
        newtype->ttype = TPBOOL;

      //配列でないなら
      newtype->arraysize = -1;
      newtype->etp = NULL;
      //procedureでないなら
      newtype->paratp = NULL;

      p->itp = newtype;
    }
  }
}
void cr_globalcountup()
{ /* Register and count up the name pointed by np */
  struct ID *p;
  char *cp;
  printf("A\n");
  if ((p = search_globalcr(string_attr)) != NULL)
  {
    printf("B\n");

    struct LINE *newl;//末尾につけるLINE
    if((newl = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)//領域確保
    {
      printf("can not malloc in cr_globalcountup\n");
      return;
    }
    newl->reflinenum = get_linenum();//行数取得
    newl->nextlinep = NULL;//次はまだ未定なのでNULL
    printf("C\n");

    struct LINE *l,*before_l;
    if((l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)//領域確保
    {
      printf("can not malloc in cr_globalcountup\n");
      return;
    }
    printf("D\n");
    before_l = NULL;
    for (l = p->irefp; l != NULL; l = l->nextlinep)
    {
      before_l = l;
      printf("c-1\n");
    }
    printf("E\n");
    if(p->irefp == NULL)//初回のみ
    {
      p->irefp = newl;
      printf("初回\n");
    }
    else
    {
      before_l->nextlinep = newl;
      printf("2回目以降\n");
    }
    //p->count++;
    printf("F\n");
  }
}
void cr_localcountup(char *np)
{ /* Register and count up the name pointed by np */
  struct ID *p;
  char *cp;
  if ((p = search_globalcr(np)) != NULL) //出現済みで出現行の追加
  {
    struct LINE *newl;
    if ((newl = (struct LINE *)malloc(sizeof(struct LINE))) == NULL) //領域確保
    {
      printf("can not malloc in cr_globalcountup\n");
      return;
    }
    newl->reflinenum = get_linenum(); //行数取得
    newl->nextlinep = NULL;           //次はまだ未定なのでNULL

    struct LINE *l;
    for (l = p->irefp; l->nextlinep != NULL; l = p->irefp->nextlinep)
    {
    }
    l->nextlinep = newl;
  }
  else          //新しく出現したやつ
  {
    struct ID *newid;
    struct TYPE *newtype;
    struct LINE *newline;
    if ((newid = (struct ID *)malloc(sizeof(struct ID))) == NULL)
    {
      printf("can not malloc in cr_globalcountup : newid\n");
      return;
    }
    if ((newtype = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
    {
      printf("can not malloc in cr_globalcountup : newtype\n");
      return;
    }
    if ((newline = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)
    {
      printf("can not malloc in cr_globalcountup : newline\n");
      return;
    }
    if ((cp = (char *)malloc(strlen(np) + 1)) == NULL)
    {
      printf("can not malloc-2 in id_countup\n");
      return;
    }
    //-----------TYPE-----------
    //newtype->ttype /*= TODO 型の取得 */;
    //if 型がarrayならarraysizeの取得とetpの代入

    //-----------LINE-----------
    newline->reflinenum = get_linenum();
    newline->nextlinep = NULL;

    strcpy(cp, np);
    newid->name = cp;
    //newid->procname = //TODO プロセスの名前格納
    //ifパラメータが存在したらispraを1，ひとまず0に
    newid->deflinenum = get_linenum();
    newid->irefp = newline;
    newid->nextp = globalidroot;
    globalidroot = p;
  }
}
void print_globalcr()
{ /* Output the registered data */
  struct ID *p;

  for (p = globalidroot; p != NULL; p = p->nextp)
  {
//      printf("\t\"Identifier\" \"%s\"\t%d\n", p->name, p->count);
      //printf("%s\t%d\t%d",p->name,p->itp->ttype,p->deflinenum);
      printf("======%d==========\n",p->itp->ttype);
    if(p->itp->ttype == TPINT)
    {
      printf("%s\tinteger\t\t%d |", p->name, p->deflinenum);
    }
    else if (p->itp->ttype == TPCHAR)
    {
      printf("%s\tchar\t\t%d |", p->name, p->deflinenum);
    }
    else if (p->itp->ttype == TBOOLEAN)
    {
      printf("%s\tboolean\t\t%d |", p->name, p->deflinenum);
    }
    struct LINE *l;
    for(l = p->irefp;l != NULL;l = l->nextlinep)
    {
      printf(" %d",l->reflinenum);
      //l = l->nextlinep;
      //if(l != NULL)
      //  printf(", ");
      //else
      //  break;
    }
    printf("\n");
  }
}
//void print_localcr()
//{ /* Output the registered data */
//  struct ID *p;
//
//  for (p = localidroot; p != NULL; p = p->nextp)
//  {
//    if (p->count != 0)
//      printf("\t\"Identifier\" \"%s\"\t%d\n", p->name, p->count);
//  }
//}
void release_globalcr()
{ /* Release tha data structure */
  struct ID *p, *q;

  for (p = globalidroot; p != NULL; p = q)
  {
    printf("%s\n", p->name);
    printf("======%d==========\n", p->itp->ttype);
    free(p->name);
    printf("A\n");
    free(p->procname);
    printf("B-1\n");
    free(p->itp->etp);
    printf("B-2\n");
    free(p->itp->paratp);
    printf("B-3\n");
    free(p->itp);
    printf("C\n");
    free(p->irefp);
    printf("D\n");
    q = p->nextp;
    printf("E\n");
    free(p);

  }
  init_globalcr();
}
void release_localcr()
{ /* Release tha data structure */
  struct ID *p, *q;

  for (p = localidroot; p != NULL; p = q)
  {
    free(p->name);
    free(p->procname);
    free(p->itp);
    free(p->irefp);
    q = p->nextp;
    free(p);
  }
  init_localcr();
}