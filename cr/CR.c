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
} * globalidroot, *localidroot, *alllocalidroot; /* Pointers to root of global & local symbol tables */
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
  if(search_globalcr(string_attr) != NULL)
  {
    printf("\n global \n");
    error("Duplicate variable");
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
void cr_localDeclaration() //変数宣言時変数だけ
{
  struct ID *newid;
  //struct TYPE *newtype;
  //struct LINE *newline;
  if ((newid = (struct ID *)malloc(sizeof(struct ID))) == NULL)
  {
    printf("can not malloc in cr_globalcountup : newid\n");
    return;
  }
  if (search_localcr(string_attr) != NULL)
  {
    printf("\n local \n");
    error("Duplicate variable");
  }
  if ((newid->name = (char *)malloc(strlen(string_attr) + 1)) == NULL)
    printf("can not malloc globalDeclaration\n");
  else
  {
    strcpy(newid->name, string_attr);
  }
  //newid->name = string_attr;
  if ((newid->procname = (char *)malloc(strlen(string_attr) + 1)) == NULL)
    printf("can not malloc globalDeclaration\n");
  else
  {
    strcpy(newid->procname, current_proce_name);
  }

  newid->itp = NULL;      //とりあえずNULLにしておく
  //ifパラメータが存在したらispraを1，ひとまず0に
  newid->ispara = 0;
  newid->deflinenum = get_linenum();
  newid->irefp = NULL;
  newid->nextp = localidroot;
  localidroot = newid;
}
void cr_procedureDeclaration()
{
  struct ID *newid;
  struct TYPE *proctype;
  //struct LINE *newline;
  if ((newid = (struct ID *)malloc(sizeof(struct ID))) == NULL)
  {
    printf("can not malloc in cr_procedureDeclaration\n");
    return;
  }
  if ((proctype = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
  {
    printf("can not malloc in cr_procedureDeclaration\n");
    return;
  }
  if (search_globalcr(string_attr) != NULL)
  {
    error("Duplicate variable");
  }
  if ((newid->name = (char *)malloc(strlen(string_attr) + 1)) == NULL)
    printf("can not malloc cr_procedureDeclaration\n");
  else
  {
    strcpy(newid->name, string_attr);
  }
  //newid->name = string_attr;
  if ((newid->procname = (char *)malloc(strlen(string_attr) + 1)) == NULL)
    printf("can not malloc cr_procedureDeclaration\n");
  else
  {
    strcpy(newid->procname, current_proce_name);
  }
  proctype->ttype = TPPROC;
  proctype->arraysize = -1;
  proctype->paratp = NULL;
  newid->itp = proctype;
  //ifパラメータが存在したらispraを1，ひとまず0に
  newid->ispara = 0;

  newid->deflinenum = get_linenum();

  
  newid->irefp = NULL;
  newid->nextp = globalidroot;
  globalidroot = newid;
}


void cr_globalsettype(int type, int is_array)
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
      if(is_array == 0)
      {
        if (type == TCHAR)
          newtype->ttype = TPCHAR;
        else if (type == TINTEGER)
          newtype->ttype = TPINT;
        else if (type == TBOOLEAN)
          newtype->ttype = TPBOOL;

        //配列でないなら
        newtype->arraysize = -1;
        newtype->etp = NULL;
      }
      else
      {
        struct TYPE *tarray;
        if ((tarray = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
        {
          printf("can not malloc in cr_globalsettype\n");
          return;
        }

        if (type == TCHAR)
        {
          tarray->ttype = TPCHAR;
          newtype->ttype = TPARRAYCHAR;
        }
        else if (type == TINTEGER)
        {
          tarray->ttype = TPINT;
          newtype->ttype = TPARRAYINT;
        }
        else if (type == TBOOLEAN)
        {
          tarray->ttype = TPBOOL;
          newtype->ttype = TPARRAYBOOL;
        }

        //配列なら
        newtype->arraysize = num_attr;
        newtype->etp = tarray;
      }
      
      //procedureでないなら
      newtype->paratp = NULL;

      p->itp = newtype;
    }
  }
}
void cr_localsettype(int type, int is_array)
{

  struct ID *p;
  for (p = localidroot; p != NULL; p = p->nextp)
  {
    if (p->itp == NULL)
    {
      struct TYPE *newtype;
      if ((newtype = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
      {
        printf("can not malloc in cr_localsettype\n");
        return;
      }
      if (is_array == 0)
      {
        if (type == TCHAR)
          newtype->ttype = TPCHAR;
        else if (type == TINTEGER)
          newtype->ttype = TPINT;
        else if (type == TBOOLEAN)
          newtype->ttype = TPBOOL;

        //配列でないなら
        newtype->arraysize = -1;
        newtype->etp = NULL;
      }
      else
      {
        struct TYPE *tarray;
        if ((tarray = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
        {
          printf("can not malloc in cr_localsettype\n");
          return;
        }

        if (type == TCHAR)
        {
          tarray->ttype = TPCHAR;
          newtype->ttype = TPARRAYCHAR;
        }
        else if (type == TINTEGER)
        {
          tarray->ttype = TPINT;
          newtype->ttype = TPARRAYINT;
        }
        else if (type == TBOOLEAN)
        {
          tarray->ttype = TPBOOL;
          newtype->ttype = TPARRAYBOOL;
        }

        //配列なら
        newtype->arraysize = num_attr;
        newtype->etp = tarray;
      }

      //procedureでないなら
      newtype->paratp = NULL;

      p->itp = newtype;
    }
  }
}
void cr_procedure_setparatp(int type,int is_array,int paracount)
{
  struct ID *l;
  
  for(l = globalidroot;l != NULL;l = l->nextp)
  {
    if(l->itp->ttype == TPPROC && strcmp(l->name,current_proce_name) == 0)
    {
      for(int i = 0;i < paracount;i++)
      {
        struct TYPE *proctype;
        if ((proctype = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL) //領域確保
        {
          printf("can not malloc in cr_procedure_setparatp\n");
          return;
        }

        if (is_array == 0)
        {
          if (type == TCHAR)
            proctype->ttype = TPCHAR;
          else if (type == TINTEGER)
            proctype->ttype = TPINT;
          else if (type == TBOOLEAN)
            proctype->ttype = TPBOOL;

          //配列でないなら
          proctype->arraysize = -1;
          proctype->etp = NULL;
        }
        else
        {
          struct TYPE *tarray;
          if ((tarray = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
          {
            printf("can not malloc in cr_globalsettype\n");
            return;
          }

          if (type == TCHAR)
          {
            tarray->ttype = TPCHAR;
            proctype->ttype = TPARRAYCHAR;
          }
          else if (type == TINTEGER)
          {
            tarray->ttype = TPINT;
            proctype->ttype = TPARRAYINT;
          }
          else if (type == TBOOLEAN)
          {
            tarray->ttype = TPBOOL;
            proctype->ttype = TPARRAYBOOL;
          }

          //配列なら
          proctype->arraysize = num_attr;
          proctype->etp = tarray;
        }

        if(i == 0)
        {
          l->itp->paratp = proctype;
          l->ispara = 1;
        }
        else
        {
          struct TYPE *t, *before_t;
          for (t = l->itp; t != NULL; t = t->paratp)
          {
            before_t = t;
          }
          before_t->paratp = proctype;
        }
      }
      return;
    }
  }
  return;
}
void cr_globalcountup()
{ /* Register and count up the name pointed by np */
  struct ID *p;
  char *cp;
  if ((p = search_globalcr(string_attr)) != NULL)
  {
    struct LINE *newl;//末尾につけるLINE
    if((newl = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)//領域確保
    {
      printf("can not malloc in cr_globalcountup\n");
      return;
    }
    newl->reflinenum = get_linenum();//行数取得
    newl->nextlinep = NULL;//次はまだ未定なのでNULL

    struct LINE *l,*before_l;
    if((l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)//領域確保
    {
      printf("can not malloc in cr_globalcountup\n");
      return;
    }
    if ((before_l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL) //領域確保
    {
      printf("can not malloc in cr_globalcountup\n");
      return;
    }
    before_l = NULL;
    for (l = p->irefp; l != NULL; l = l->nextlinep)
    {
      before_l = l;
    }

    if(p->irefp == NULL)//初回のみ
    {
      p->irefp = newl;
    }
    else
    {
      before_l->nextlinep = newl;
    }
  }
  else
  {
    printf("\nstring_attr :  %s \n",string_attr);
    error("There are undefined variables");
  }
}
void cr_localcountup()
{ /* Register and count up the name pointed by np */
  struct ID *p;
  char *cp;
  if ((p = search_localcr(string_attr)) != NULL)
  {
    struct LINE *newl;                                               //末尾につけるLINE
    if ((newl = (struct LINE *)malloc(sizeof(struct LINE))) == NULL) //領域確保
    {
      printf("can not malloc in cr_globalcountup\n");
      return;
    }
    newl->reflinenum = get_linenum(); //行数取得
    newl->nextlinep = NULL;           //次はまだ未定なのでNULL

    struct LINE *l, *before_l;
    if ((l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL) //領域確保
    {
      printf("can not malloc in cr_localcountup\n");
      return;
    }
    if ((before_l = (struct LINE *)malloc(sizeof(struct LINE))) == NULL) //領域確保
    {
      printf("can not malloc in cr_localcountup\n");
      return;
    }
    before_l = NULL;
    for (l = p->irefp; l != NULL; l = l->nextlinep)
    {
      before_l = l;
    }

    if (p->irefp == NULL) //初回のみ
    {
      p->irefp = newl;
    }
    else
    {
      before_l->nextlinep = newl;
    }
  }
  else
    error("There are undefined variables");
}
void print_globalcr()
{ /* Output the registered data */
  struct ID *p;

  for (p = globalidroot; p != NULL; p = p->nextp)
  {
      //printf("======%d==========\n",p->itp->ttype);
    if(p->itp->ttype == TPINT)
    {
      printf("%s\tinteger\t\t%d |", p->name, p->deflinenum);
    }
    else if (p->itp->ttype == TPCHAR)
    {
      printf("%s\tchar\t\t%d |", p->name, p->deflinenum);
    }
    else if (p->itp->ttype == TPBOOL)
    {
      printf("%s\tboolean\t\t%d |", p->name, p->deflinenum);
    }
    else if(p->itp->ttype == TPARRAYINT)
    {
      printf("%s\tarray[%d] of integer\t\t%d |", p->name,p->itp->arraysize, p->deflinenum);
    }
    else if(p->itp->ttype == TPARRAYCHAR)
    {
      printf("%s\tarray[%d] of char\t\t%d |", p->name, p->itp->arraysize, p->deflinenum);
    }
    else if(p->itp->ttype == TPARRAYBOOL)
    {
      printf("%s\tarray[%d] of boolean\t\t%d |", p->name, p->itp->arraysize, p->deflinenum);
    }
    else if(p->itp->ttype == TPPROC)
    {
      printf("%s\tprocedure", p->name);
      if(p->itp->paratp != NULL)
      {
        printf("(");
        struct TYPE *t;
        for(t = p->itp->paratp;t != NULL;t = t->paratp)
        {
          if(t->ttype == TPINT) 
            printf("integer");
          else if (t->ttype ==TPCHAR) 
            printf("char");
          else if (t->ttype ==TPBOOL)
            printf("boolean");
          else if (t->ttype == TPARRAY)
          {
            if (t->ttype == TPARRAYINT) 
              printf("array[%d] of integer",t->arraysize);
            else if (t->ttype == TPARRAYCHAR)
              printf("array[%d] of char", t->arraysize);
            else if (t->ttype == TPARRAYBOOL)
              printf("array[%d] of boolean", t->arraysize);
          }
        }
        printf(")");
      }
      printf("\t\t %d | ", p->deflinenum);
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
void print_localcr()
{ /* Output the registered data */
  struct ID *p;

  for (p = localidroot; p != NULL; p = p->nextp)
  {
    //printf("======%d==========\n", p->itp->ttype);
    if (p->itp->ttype == TPINT)
    {
      printf("%s:%s \tinteger\t\t%d |",p->name,p->procname, p->deflinenum);
    }
    else if (p->itp->ttype == TPCHAR)
    {
      printf("%s:%s \tchar\t\t%d |",p->name,p->procname, p->deflinenum);
    }
    else if (p->itp->ttype == TPBOOL)
    {
      printf("%s:%s \tboolean\t\t%d |",p->name,p->procname, p->deflinenum);
    }
    else if (p->itp->ttype == TPARRAYINT)
    {
      printf("%s:%s \tarray[%d] of integer\t\t%d |",p->name,p->procname, p->itp->arraysize, p->deflinenum);
    }
    else if (p->itp->ttype == TPARRAYCHAR)
    {
      printf("%s:%s \tarray[%d] of char\t\t%d |",p->name,p->procname, p->itp->arraysize, p->deflinenum);
    }
    else if (p->itp->ttype == TPARRAYBOOL)
    {
      printf("%s:%s \tarray[%d] of boolean\t\t%d |",p->name,p->procname, p->itp->arraysize, p->deflinenum);
    }
    else if (p->itp->ttype == TPPROC)
    {
      printf("%s\tprocedure", p->name);
      if (p->itp->paratp != NULL)
      {
        printf("(");
        struct TYPE *t;
        for (t = p->itp->paratp; t != NULL; t = t->paratp)
        {
          if (t->ttype == TPINT)
            printf("integer");
          else if (t->ttype == TPCHAR)
            printf("char");
          else if (t->ttype == TPBOOL)
            printf("boolean");
          else if (t->ttype == TPARRAY)
          {
            if (t->ttype == TPARRAYINT)
              printf("array[%d] of integer", t->arraysize);
            else if (t->ttype == TPARRAYCHAR)
              printf("array[%d] of char", t->arraysize);
            else if (t->ttype == TPARRAYBOOL)
              printf("array[%d] of boolean", t->arraysize);
          }
        }
        printf(")");
      }
      printf("\t\t %d | ", p->deflinenum);
    }
    struct LINE *l;
    for (l = p->irefp; l != NULL; l = l->nextlinep)
    {
      printf(" %d", l->reflinenum);
      //l = l->nextlinep;
      //if(l != NULL)
      //  printf(", ");
      //else
      //  break;
    }
    printf("\n");
  }
}
void release_globalcr()
{ /* Release tha data structure */
  struct ID *p, *q;
  struct TYPE *t, *u;
  for (p = globalidroot; p != NULL; p = q)
  {
    printf("%s\n", p->name);
    printf("======%d==========\n", p->itp->ttype);
    free(p->name);
    printf("A\n");
    free(p->procname);


    printf("B-1\n");
    for(t = p->itp; t != NULL; t = u)
    {
      if(t->ttype == TPARRAY)
        free(t->etp);
      u = t->paratp;
      free(t);
    }
    //free(p->itp->etp);
    //printf("B-2\n");
    //free(p->itp->paratp);
    //printf("B-3\n");
    //free(p->itp);
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

void copy_local()
{
  struct ID *l,*all;
  if ((l = (struct ID *)malloc(sizeof(struct ID))) == NULL) //領域確保
  {
    printf("can not malloc in cr_localcountup\n");
    return;
  }
  
  //all = alllocalidroot;//TODOケツまで移動しなくちゃ行けない→そこで初めてプログラムが動くかも
  
  for (l = localidroot;l != NULL;l = l->nextp)
  {
    for (all = alllocalidroot; all != NULL; all = all->nextp)
    {
    }

    if ((all->name = (char *)malloc(strlen(l->name) + 1)) == NULL)
    {
      printf("can not malloc in copy_local\n");
      return;
    }
    strcpy(all->name,l->name);
    if ((all->procname = (char *)malloc(strlen(l->procname) + 1)) == NULL)
    {
      printf("can not malloc in copy_local\n");
      return;
    }
    strcpy(all->procname, l->procname);

    struct TYPE *t, *alt;
    if ((alt = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
    {
      printf("can not malloc in copy_local\n");
      return;
    }
    //配列の時はその要素の型まで追う
    if (l->itp->ttype == TPARRAY)
    {
      alt = all->itp;
      alt->ttype = l->itp->ttype;
      alt->arraysize = l->itp->arraysize;
      struct TYPE *newetp;
      if ((newetp = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
      {
        printf("can not malloc in copy_local\n");
        return;
      }
      newetp->ttype = l->itp->etp->ttype;
      newetp->arraysize = l->itp->etp->arraysize;
      alt->etp = newetp;
    }
    else if (l->ispara == 1) //procedureでパラメータがあるときは全てを追って行かないといけない
    {
      //t = l->itp->paratp;        //パラメータの最初の変数を指す
      alt = all->itp;
      alt->ttype = l->itp->ttype;//TPPROCが入るはず
      alt->arraysize = l->itp->arraysize;
      
      
      
      for(t = l->itp->paratp;t != NULL;t = t->paratp)
      {
        struct TYPE *newpara;
        if ((newpara = (struct TYPE *)malloc(sizeof(struct TYPE))) == NULL)
        {
          printf("can not malloc in copy_local\n");
          return;
        }

        newpara->ttype = t->ttype;
        newpara->arraysize = t->arraysize;
        alt->paratp = newpara;
        alt = alt->paratp;
        //altの移動？
      }
    }
    else
    {
      alt->ttype = l->itp->ttype;
      alt->arraysize = l->itp->arraysize;
      all->itp = alt;
    }
    all->ispara = l->ispara;
    all->deflinenum = l->deflinenum;
    struct LINE *line,*newl;
    if ((newl = (struct LINE *)malloc(sizeof(struct LINE))) == NULL)
    {
      printf("can not malloc in copy_local\n");
      return;
    }
    all->irefp = newl;
    for(line = l->irefp;line != NULL;line = line->nextlinep)
    {
      struct LINE *tmpl;
      for(tmpl = newl;tmpl != NULL;tmpl = tmpl->nextlinep)
      {}

      tmpl->reflinenum = line->reflinenum;
      tmpl->nextlinep = line->nextlinep;
    }
    all->nextp = l->nextp;
  }
}