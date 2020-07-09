#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct Produto
{
  int codigo;
  char descricao[25];
  int qtdEstoque;
  float valor;
} p;

struct Compra
{
  int codigoCompra;
  int codigoProduto;
  int quantidade;
  int e;
} c;

FILE * fptr;

void alterarProduto();
void cadastrarProduto();
void consultarProduto();
void consultarProdutoCompra();
void cadastrarCompra();
void consultarCompra();
void exclusaoLogicaCompra();
void exclusaoFisicaCompra();
void ordenacao();
int  buscarCompra();
void alterarCompra();
void recuperacaoBackup();

void main()
{
  char opc;
  int pos;

  do
  {
    system("cls");
    printf("*******************************************");
    printf("\n*  MERCADINHO DE ESQUINA do SEBAS E TIAO  *");
    printf("\n*******************************************");    
    printf("\n* 1 - Alterar Produto                     *");
    printf("\n* 2 - Cadastrar Produtos                  *");
    printf("\n* 3 - Consultar Produtos                  *");
    printf("\n* 4 - Consultar Produto por Compra        *");
    printf("\n*******************************************");
    printf("\n* 5 - Cadastrar Compra                    *");
    printf("\n* 6 - Consultar Compra                    *");
    printf("\n* 7 - Exclusao Logica Compra              *");
    printf("\n* 8 - Exclusao Fisica Compra              *");    
    printf("\n* 0 - Ordenacao por Codigo de Compra      *");
    printf("\n*******************************************");
    printf("\n* b - Buscar pelo Codigo de Compra        *");
    printf("\n* a - Alterar Compra                      *");
    printf("\n* r - Recuperar Compra                    *");
    printf("\n*******************************************");
    printf("\n* 9 - Sair                                *");
    printf("\n*******************************************");
    printf("\nOpcao: ");
    opc = getche();

    switch (opc)
    {
    case '1':
      alterarProduto();      
      break;
    case '2':
      cadastrarProduto();      
      break;
    case '3':
      consultarProduto();
      break;
    case '4':
      consultarProdutoCompra();
      break;
    case '5':
      cadastrarCompra();
      break;
    case '6':
      consultarCompra();
      break;
    case '7':
      exclusaoLogicaCompra();
      break;
    case '8':
      exclusaoFisicaCompra();
      break;
    case '0':
      ordenacao();
      break;
    case 'b':
      ordenacao();
      pos = buscarCompra();
      if (pos == -1)
      {
        printf("\nCompra nao Encontrada");
        getch();
      }
      else
      {
        printf("\nCompra encontrada e esta no %d %c registro do arquivo", pos + 1, 167);
        getch();
      }
      break;
    case 'a':
      alterarCompra();
      break;
    case 'r':
      recuperacaoBackup();
      break;
    case '9':
      printf("\nSaindo...\n");
      system("pause");
      break;
    default:
      printf("\nOpcao %c invalida", opc);
      getch();
      break;
    }
  } while (opc != '9');
}

void cadastrarProduto()
{
  char op;
  if ((fptr = fopen("produto.dados", "rb+")) == NULL)
  {
    if ((fptr = fopen("produto.dados", "wb")) == NULL)
    {
      printf("\nERRO");
      exit(1);
    }
  }
  do
  {
    system("cls");
    printf("Entre com os dados para Cadastrar o Produto\n");    
    printf("\nCodigo Produto: ");
    scanf("%d", &p.codigo);
    printf("Descricao: ");
    setbuf(stdin, NULL);
    scanf("%[^\n]", p.descricao);
    printf("Quantidade Estoque: ");
    scanf("%d", &p.qtdEstoque);
    printf("Valor: R$");
    scanf("%f", &p.valor);
    fseek(fptr, 0, 2);
    fwrite(&p, sizeof(p), 1, fptr);
    printf("\nDeseja Cadastrar outro (S/N): ");
    op = getche();

  } while ((op != 'n') && (op != 'N'));
  fclose(fptr);
}

void consultarProduto()
{
  if ((fptr = fopen("produto.dados", "rb")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }

  system("cls");
  printf("Consulta de Todos Produtos do Mercado\n");
  while (fread(&p, sizeof(p), 1, fptr))
  {
    printf("\nCodigo: %d", p.codigo);
    printf("\nDescricao: %s", p.descricao);
    printf("\nQuantidade no Estoque: %d", p.qtdEstoque);
    printf("\nValor: R$%.2f\n", p.valor);
  }
  getch();
  fclose(fptr);
}

void alterarProduto()
{
  int codigoProduto;
  float valorCompra;
  if ((fptr = fopen("produto.dados", "rb+")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  
  system("cls");
  printf("Entre com o Codigo do Produto a ser alterado: ");
  scanf("%d", &codigoProduto);

  while (fread(&p, sizeof(p), 1, fptr))
  {
    if (p.codigo == codigoProduto)
    {
      fseek(fptr, ftell(fptr) - sizeof(p), 0);
      printf("Nova Descricao: ");
      setbuf(stdin, NULL);
      scanf("%[^\n]", p.descricao);
      printf("Nova Quantidade no Estoque: ");
      scanf("%d", &p.qtdEstoque);
      printf("Novo Valor: R$");
      scanf("%f", &p.valor);
      fwrite(&p, sizeof(p), 1, fptr);
      fseek(fptr, 0, 2);
    }    
  }
  valorCompra = c.quantidade * p.valor;
  printf("\nValor Total da Compra: R$%.2f", valorCompra);
  printf("\nProduto alterado com Sucesso.\n");
  getch();
  fclose(fptr);  
}

void consultarProdutoCompra()
{
  FILE *fptrCompra;
  if ((fptr = fopen("produto.dados", "rb")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  if ((fptrCompra = fopen("compra.dados", "rb")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  while (fread(&p, sizeof(p), 1, fptr))
  {
    printf("\n%d ", p.codigo);
    printf(" %s ", p.descricao);
    
    fseek(fptrCompra, 0, 0);
    while (fread(&c, sizeof(c), 1, fptrCompra))
    {
      if (c.codigoProduto == p.codigo)
      {
        if (c.e == 0)
        {
          printf("\n   Cod.Compra:%d ", c.codigoCompra);
          printf(" Qtd:%d ", c.quantidade);        
        }
      }
    }
  }
  getch();
  fclose(fptr);
  fclose(fptrCompra);
}

void cadastrarCompra()
{
  char op;
  float valorCompra;

  if ((fptr = fopen("compra.dados", "rb+")) == NULL)
  {
    if ((fptr = fopen("compra.dados", "wb")) == NULL)
    {
      printf("\nERROR");
      exit(1);
    }    
  }
  system("cls");
  printf("Entre com os dados para Efetuar uma Compra\n");
  printf("\nCodigo da Compra: ");
  scanf("%d", &c.codigoCompra);
  printf("Codigo do Produto: ");
  scanf("%d", &c.codigoProduto);
  printf("Quantidade: ");
  scanf("%d", &c.quantidade);
  c.e = 0;
  fseek(fptr, 0, 2);
  fwrite(&c, sizeof(c), 1, fptr);  
     
  fclose(fptr);

  if ((fptr = fopen("produto.dados", "rb+")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }

  while (fread(&p, sizeof(p), 1, fptr))
  {
    if (p.codigo == c.codigoProduto)
    {
      if (c.e == 0){
        fseek(fptr, ftell(fptr) - sizeof(p), 0);
        p.qtdEstoque = p.qtdEstoque + c.quantidade;
        fwrite(&p, sizeof(p), 1, fptr);
        fseek(fptr, 0, 2);
      }
    }
  }

  valorCompra = c.quantidade * p.valor;
  printf("\nValor Total da Compra: R$%.2f", valorCompra);
  printf("\nCompra Realizada com Sucesso.\n");
  op = getche(); 
  fclose(fptr);
}

void alterarCompra()
{
  int codigoCompra;
  int antigaQtdCompra;
  if ((fptr = fopen("compra.dados", "rb+")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  
  system("cls");
  printf("Entre com o Codigo da Compra a ser alterada: ");
  scanf("%d", &codigoCompra);

  while (fread(&c, sizeof(c), 1, fptr))
  {
    if (c.codigoCompra == codigoCompra)
    {
      fseek(fptr, ftell(fptr) - sizeof(c), 0);
      printf("Novo Codigo de Produto: ");
      scanf("%d", &c.codigoProduto);
      printf("Nova Quantidade: ");
      antigaQtdCompra = c.quantidade;
      scanf("%d", &c.quantidade);
      fwrite(&c, sizeof(c), 1, fptr);
      fseek(fptr, 0, 2);
    }
  }
  fclose(fptr);

  if ((fptr = fopen("produto.dados", "rb+")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  
  while (fread(&p, sizeof(p), 1, fptr))
  {
    if (p.codigo == c.codigoProduto)
    {
      fseek(fptr, ftell(fptr) - sizeof(p), 0);
      p.qtdEstoque = p.qtdEstoque - antigaQtdCompra;
      p.qtdEstoque = p.qtdEstoque + c.quantidade;
      fwrite(&p, sizeof(p), 1, fptr);
      fseek(fptr, 0, 2);      
    }
  }
  printf("\nCompra alterado com Sucesso.\n");
  getch();
  fclose(fptr);  
}

void consultarCompra()
{
  if ((fptr = fopen("compra.dados", "rb")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  system("cls");
  printf("Consulta de Todas Compras realizadas pelo Mercado\n");
  while (fread(&c, sizeof(c), 1, fptr))
  {
    if (c.e == 0)
    {
      printf("\nCodigo da Compra: %d", c.codigoCompra);
      printf("\nCodigo do Produto: %d", c.codigoProduto);
      printf("\nQuantidade Comprada: %d\n", c.quantidade);
    }    
  }
  getch();
  fclose(fptr);
}

void exclusaoLogicaCompra()
{
  int codigoCompra;
  int antigaQtdCompra;
  if ((fptr = fopen("compra.dados", "rb+")) == NULL)
  {
    printf("\nErro");
    exit(1);
  }
  system("cls");
  printf("Entre com o Codigo da Compra a ser excluido logicamente: ");
  scanf("%d", &codigoCompra);

  while (fread(&c, sizeof(c), 1, fptr))
  {
    if (c.codigoCompra == codigoCompra)
    {
      if (c.e == 0)
      {
        fseek(fptr, ftell(fptr) - sizeof(c), 0);
        c.e = 1;
        antigaQtdCompra = c.quantidade;
        fwrite(&c, sizeof(c), 1, fptr);
        fseek(fptr, 0, 2);
      }
    }
  }
  fclose(fptr);

  if ((fptr = fopen("produto.dados", "rb+")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  
  while (fread(&p, sizeof(p), 1, fptr))
  {
    if (p.codigo == c.codigoProduto)
    {
      fseek(fptr, ftell(fptr) - sizeof(p), 0);
      p.qtdEstoque = p.qtdEstoque - antigaQtdCompra;
      fwrite(&p, sizeof(p), 1, fptr);
      fseek(fptr, 0, 2);
    }
  }
  printf("Compra exlcuida com sucesso.\n");
  getch();
  fclose(fptr);
}

void exclusaoFisicaCompra()
{
  FILE * fptrback, * fptraux;

  if ((fptr = fopen("compra.dados", "rb")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  if ((fptrback = fopen("back.dados", "rb+")) == NULL)
  {
    if ((fptrback = fopen("back.dados", "wb")) == NULL)
    {
      printf("\nERRO");
      exit(1);
    } 
  }
  if ((fptraux = fopen("auxiliar.dados", "wb")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  
  while (fread(&c, sizeof(c), 1, fptr))
  {
    if (c.e == 0)
    {
      fseek(fptraux, 0, 2);
      fwrite(&c, sizeof(c), 1, fptraux);
    }
    else
    {
      fseek(fptrback, 0, 2);
      fwrite(&c, sizeof(c), 1, fptrback);
    }    
  }
  printf("\nCompra excluida fisicamente com sucesso, foi criado o arquivo de backup.\n");
  getch();
  fclose(fptr);
  fclose(fptraux);
  fclose(fptrback);
  remove("compra.dados");
  rename("auxiliar.dados", "compra.dados");
}

void ordenacao()
{
  struct Compra ci, cj;
  int i, j, n;

  if ((fptr = fopen("compra.dados", "rb+")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  fseek(fptr, 0, 2);
  n = ftell(fptr) / sizeof(c);

  for (i = 0; i < n; i++)
  {
    for (j = i + 1; j < n; j++)
    {
      fseek(fptr, i * sizeof(c), 0);
      fread(&ci, sizeof(c), 1, fptr);
      fseek(fptr, j * sizeof(c), 0);
      fread(&cj, sizeof(c), 1, fptr);
      if (ci.codigoCompra > cj.codigoCompra)
      {
        fseek(fptr, i * sizeof(c), 0);
        fwrite(&cj, sizeof(c), 1, fptr);
        fseek(fptr, j * sizeof(c), 0);
        fwrite(&ci, sizeof(c), 1, fptr);
      }      
    }    
  }
  fclose(fptr);
}

int buscarCompra()
{
  int codigo;
  int low, high, mid, n;
  if ((fptr = fopen("compra.dados", "rb")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  system("cls");
  printf("Entre com o Codigo da Compra a ser encontrada: ");
  scanf("%d", &codigo);
  fseek(fptr, 0, 2);
  n = ftell(fptr) / sizeof(c);
  low = 0;
  high =  n - 1;
  while (low <= high)
  {
    mid = (low + high) / 2;
    fseek(fptr, mid * sizeof(c), 0);
    fread(&c, sizeof(c), 1, fptr);
    if (c.codigoCompra > codigo)
    {
      high = mid - 1;
    }
    else if (c.codigoCompra < codigo)
    {
      low = mid + 1;
    }
    else
    {
      return mid;
    }       
  }
  return -1;
}

void recuperacaoBackup()
{
  FILE *fptrback, *fptraux;
  char op;
  int codigo;

  if ((fptrback = fopen("back.dados", "rb")) == NULL)
  {
    printf("\nERRO");
    exit(1);
  }
  if ((fptr = fopen("compra.dados", "rb+")) == NULL)
  {
    if((fptr = fopen("compra.dados", "wb")) == NULL){
      printf("\nERRO");
      exit(1);
    }
  }
  do
  {
    system("cls");
    printf("1 - Recuperar por Codigo");
    printf("\n2 - Recuperar Todos");
    printf("\n9 - Voltar Menu");
    printf("\nOpcao: ");
    op = getche();

    switch (op)
    {
    case '1':
      system("cls");
      printf("Digite o Codigo da Compra a ser Recuperado: ");
      scanf("%d", &codigo);
      while (fread(&c, sizeof(c), 1, fptrback))
      {
        if (c.codigoCompra == codigo)
        {
          if (c.e == 1)
          {
            c.e = 0;
            fseek(fptr, 0, 2);
            fwrite(&c, sizeof(c), 1, fptr);
          }
        }
      }

      if ((fptraux = fopen("produto.dados", "rb+")) == NULL)
      {
        printf("\nERRO");
        exit(1);
      }

      while (fread(&p, sizeof(p), 1, fptraux))
      {
        if (p.codigo == c.codigoProduto)
        {
          fseek(fptraux, ftell(fptraux) - sizeof(p), 0);
          p.qtdEstoque = p.qtdEstoque + c.quantidade;
          fwrite(&p, sizeof(p), 1, fptraux);
          fseek(fptraux, 0, 2);
        }
      }
      printf("\nCompra Recuperada com Sucesso!\n");
      getch();
      break;
    case '2':
      while (fread(&c, sizeof(c), 1, fptrback))
      {
        if (c.e == 1)
        {
          c.e = 0;
          fseek(fptr, 0, 2);
          fwrite(&c, sizeof(c), 1, fptr);
        }
      }      
      printf("\nCompras Recuperada com Sucesso!\n");
      getch();
      break;
    case '9':
      getch();
      break;
    default:
      printf("\nOpcao %c invalida", op);
      getch();
      break;
    }
  } while (op != '9');
  
  fclose(fptr);
  fclose(fptraux);
  fclose(fptrback);
  remove("back.dados");
}