#include "token-list.h"
int label_number = 0;
struct DCnode{
  char *label;
  char *value;
  struct DCnode *next;
} *DCroot;
void write_label(char *label)
{
  printf("\n$%s\n",label);
}
void LD_rr(char *r1,char *r2)
{
  printf("\tLD\t%s, %s\n",r1,r2);
}
void LD_ra(char *r,char *adr,char *x)
{
  if(x == NULL)
    printf("\tLD\t%s, %s\n", r, adr);
  else
    printf("\tLD\t%s, %s, %s\n", r, adr, x);
}
void ST(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tST\t%s, %s\n", r, adr);
  else
    printf("\tST\t%s, %s, %s\n", r, adr, x);
}
void LAD(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tLAD\t%s, %s\n", r, adr);
  else
    printf("\tLAD\t%s, %s, %s\n", r, adr, x);
}
void ADDA_rr(char *r1, char *r2)
{
  printf("\tADDA\t%s, %s\n", r1, r2);
}
void ADDA_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tADDA\t%s, %s\n", r, adr);
  else
    printf("\tADDA\t%s, %s, %s\n", r, adr, x);
}
void ADDL_rr(char *r1, char *r2)
{
  printf("\tADDL\t%s, %s\n", r1, r2);
}
void ADDL_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tADDL\t%s, %s\n", r, adr);
  else
    printf("\tADDL\t%s, %s, %s\n", r, adr, x);
}
void SUBA_rr(char *r1, char *r2)
{
  printf("\tSUBA\t%s, %s\n", r1, r2);
}
void SUBA_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tSUBA\t%s, %s\n", r, adr);
  else
    printf("\tSUBA\t%s, %s, %s\n", r, adr, x);
}
void SUBL_rr(char *r1, char *r2)
{
  printf("\tSUBL\t%s, %s\n", r1, r2);
}
void SUBL_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tSUBL\t%s, %s\n", r, adr);
  else
    printf("\tSUBL\t%s, %s, %s\n", r, adr, x);
}
void AND_rr(char *r1, char *r2)
{
  printf("\tAND\t%s, %s\n", r1, r2);
}
void AND_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tAND\t%s, %s\n", r, adr);
  else
    printf("\tAND\t%s, %s, %s\n", r, adr, x);
}
void OR_rr(char *r1, char *r2)
{
  printf("\tOR\t%s, %s\n", r1, r2);
}
void OR_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tOR\t%s, %s\n", r, adr);
  else
    printf("\tOR\t%s, %s, %s\n", r, adr, x);
}
void XOR_rr(char *r1, char *r2)
{
  printf("\tXOR\t%s, %s\n", r1, r2);
}
void XOR_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tXOR\t%s, %s\n", r, adr);
  else
    printf("\tXOR\t%s, %s, %s\n", r, adr, x);
}
void MULA_rr(char *r1, char *r2)
{
  printf("\tMULA\t%s, %s\n", r1, r2);
}
void MULA_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tMULA\t%s, %s\n", r, adr);
  else
    printf("\tMULA\t%s, %s, %s\n", r, adr, x);
}
void MULL_rr(char *r1, char *r2)
{
  printf("\tMULL\t%s, %s\n", r1, r2);
}
void MULL_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tMULL\t%s, %s\n", r, adr);
  else
    printf("\tMULL\t%s, %s, %s\n", r, adr, x);
}
void DIVA_rr(char *r1, char *r2)
{
  printf("\tDIVA\t%s, %s\n", r1, r2);
}
void DIVA_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tDIVA\t%s, %s\n", r, adr);
  else
    printf("\tDIVA\t%s, %s, %s\n", r, adr, x);
}
void DIVL_rr(char *r1, char *r2)
{
  printf("\tDIVL\t%s, %s\n", r1, r2);
}
void DIVL_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tDIVL\t%s, %s\n", r, adr);
  else
    printf("\tDIVL\t%s, %s, %s\n", r, adr, x);
}
void CPA_rr(char *r1, char *r2)
{
  printf("\tCPA\t%s, %s\n", r1, r2);
}
void CPA_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tCPA\t%s, %s\n", r, adr);
  else
    printf("\tCPA\t%s, %s, %s\n", r, adr, x);
}
void CPL_rr(char *r1, char *r2)
{
  printf("\tCPL\t%s, %s\n", r1, r2);
}
void CPL_ra(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tCPL\t%s, %s\n", r, adr);
  else
    printf("\tCPL\t%s, %s, %s\n", r, adr, x);
}
void SLA(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tSLA\t%s, %s\n", r, adr);
  else
    printf("\tSLA\t%s, %s, %s\n", r, adr, x);
}
void SRA(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tSRA\t%s, %s\n", r, adr);
  else
    printf("\tSRA\t%s, %s, %s\n", r, adr, x);
}
void SLL(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tSLL\t%s, %s\n", r, adr);
  else
    printf("\tSLL\t%s, %s, %s\n", r, adr, x);
}
void SRL(char *r, char *adr, char *x)
{
  if (x == NULL)
    printf("\tSRL\t%s, %s\n", r, adr);
  else
    printf("\tSRL\t%s, %s, %s\n", r, adr, x);
}
void JPL(char *adr, char *x)
{
  if (x == NULL)
    printf("\tJPL\t%s\n",adr);
  else
    printf("\tJPL\t%s, %s\n",adr, x);
}
void JMI(char *adr, char *x)
{
  if (x == NULL)
    printf("\tJMI\t%s\n", adr);
  else
    printf("\tJMI\t%s, %s\n", adr, x);
}
void JNZ(char *adr, char *x)
{
  if (x == NULL)
    printf("\tJNZ\t%s\n", adr);
  else
    printf("\tJNZ\t%s, %s\n", adr, x);
}
void JZE(char *adr, char *x)
{
  if (x == NULL)
    printf("\tJZE\t%s\n", adr);
  else
    printf("\tJZE\t%s, %s\n", adr, x);
}
void JOV(char *adr, char *x)
{
  if (x == NULL)
    printf("\tJOV\t%s\n", adr);
  else
    printf("\tJOV\t%s, %s\n", adr, x);
}
void JUMP(char *adr, char *x)
{
  if (x == NULL)
    printf("\tJUMP\t%s\n", adr);
  else
    printf("\tJUMP\t%s, %s\n", adr, x);
}
void PUSH(char *adr, char *x)
{
  if (x == NULL)
    printf("\tPUSH\t%s\n", adr);
  else
    printf("\tPUSH\t%s, %s\n", adr, x);
}
void POP(char *r)
{
  printf("\tPOP\t%s\n", r);
}
void CALL(char *adr, char *x)
{
  if (x == NULL)
    printf("\tCALL\t%s\n", adr);
  else
    printf("\tCALL\t%s, %s\n", adr, x);
}
void RET()
{
  printf("\tRET\n");
}
void SVC(char *adr, char *x)
{
  if (x == NULL)
    printf("\tSVC\t%s\n", adr);
  else
    printf("\tSVC\t%s, %s\n", adr, x);
}
void NOP()
{
  printf("\tNOP\n");
}
char *next_calllabel()
{
  char *result;
  result = (char *)malloc(sizeof(char) * 6);
  label_number++;
  snprintf(result, 6, "L%04d", label_number);
  return(result);
}
void init_DCList()
{
  DCroot = NULL;
}
void add_DCList(char *l)
{
  struct DCnode *node,*n,*before_n;
  if ((node = (struct DCnode *)malloc(sizeof(struct DCnode))) == NULL)
  {
    printf("can not malloc in add_DCnode\n");
    return;
  }
  printf("1");
  fflush(stdout);
  if ((node->label = (char *)malloc(MAXSTRSIZE)) == NULL)
    printf("can not malloc add_DCnode\n");
  else
    strcpy(node->label, l);
  printf("2");
  fflush(stdout);
  if ((node->value = (char *)malloc(MAXSTRSIZE)) == NULL)
    printf("can not malloc add_DCnode\n");
  else
    strcpy(node->value, string_attr);
  printf("3");
  fflush(stdout);
  node->next = NULL;
  printf("4");
  fflush(stdout);
  if(DCroot == NULL)
    DCroot = node;
  else
  {
    for(n = DCroot;n != NULL;n = n->next)
    {
      before_n = n;
    }
    before_n->next = node;
  }
}
void output_DCList()
{
  struct DCnode *node;

  for(node = DCroot;node != NULL;node = node->next)
  {
    printf("AAAAAAAAAAAAAAAAAAA\n");
    printf("%s\tDC\t%s\n",node->label,node->value);
  }
}