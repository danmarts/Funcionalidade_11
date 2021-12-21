#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string.h>

using namespace std;

#define pipe "|"
#define trash "$"

#define size_cabecalho 17
#define size_status sizeof(char)
#define size_topoLista sizeof(long int)
#define size_nro_Est sizeof(int)
#define size_nro_Pares sizeof(int)

#define size_removido sizeof(char)
#define size_tamRegistro sizeof(int)
#define size_proxLista sizeof(long int)
#define size_codEstacao sizeof(int)
#define size_codLinha sizeof(int)
#define size_codProxEst sizeof(int)
#define size_distProxEst sizeof(int)
#define size_codLinhaInteg sizeof(int)
#define size_codEstInteg sizeof(int)

// definindo a struct registro
typedef struct {
  int CodEstacao;
  char *NomeEstacao;
  int CodLinha;
  char *NomeLinha;
  int CodProxEst;
  int DistanciaProxEst;
  int CodLinhaInteg;
  int CodEstacaoInteg;
} registro;

void scan_quote_string(char *str) {

  /*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

  char R;

  while ((R = getchar()) != EOF && isspace(R))
    ; // ignorar espaços, \r, \n...

  if (R == 'N' || R == 'n')
  { // campo NULO
    getchar();
    getchar();
    getchar();       // ignorar o "ULO" de NULO.
    strcpy(str, ""); // copia string vazia
  }
  else if (R == '\"')
  {
    if (scanf("%[^\"]", str) != 1)
    { // ler até o fechamento das aspas
      strcpy(str, "");
    }
    getchar(); // ignorar aspas fechando
  }
  else if (R != EOF)
  { // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
    str[0] = R;
    scanf("%s", &str[1]);
  }
  else
  { // EOF
    strcpy(str, "");
  }
}

class No {
  private:
    string NomeProxEstacao;
    vector<string> NomesLinhas;
    int DistanciaProxEst;
    No* prox;

  public:
    No(string NomeProxEstacao, string NovaLinha, int DistanciaProxEst) {
        this->NomeProxEstacao = NomeProxEstacao;
        this->DistanciaProxEst = DistanciaProxEst;
        this->prox = NULL;
        insereNovaLinha(NovaLinha);
    }

    void insereNovaLinha(string NovaLinha) {
        if (NovaLinha.length() == 0) {
            return;
        }

        if (this->NomesLinhas.empty()) {
            this->NomesLinhas.push_back(NovaLinha);
            return;
        }

        int i;
        for (i = 0; i < this->NomesLinhas.size(); i++) {
            if (this->NomesLinhas[i] > NovaLinha) {
                this->NomesLinhas.insert(this->NomesLinhas.begin() + i, NovaLinha);
                return;
            }
        }
        this->NomesLinhas.insert(this->NomesLinhas.begin() + i, NovaLinha);
    }

    string getNomeProxEstacao() {
        return NomeProxEstacao;
    }

    No* getProx() {
        return prox;
    }

    void setProx(No *prox) {
        this->prox = prox;
    }

    vector<string> getNomesLinhas() {
        return NomesLinhas;
    }

    int getDistanciaProxEst() {
        return DistanciaProxEst;
    }

    void imprime_NomesLinhas() {
        int i;
        for (i = 0; i < NomesLinhas.size() - 1; i++) {
            cout << NomesLinhas[i] << ' ';
        }
        cout << NomesLinhas[i];
    }

};

class Lista {
  private:
    No* raiz;

  public:
    Lista() {
        raiz = NULL;
    }

    void setRaiz(No* no) {
        this->raiz = no;
    }

    No* getRaiz() {
        return raiz;
    }
    
    bool lista_vazia() {
        return (raiz == NULL);
    }
    
    void imprime_lista() {
        No* aux = raiz;
        bool flag;

        if (lista_vazia()) {
            cout << "Lista vazia" << endl;
        }

        else {
            while (aux) {
            flag = false;
                if (aux->getNomeProxEstacao().length() > 0) {
                    cout << ' ' << aux->getNomeProxEstacao();
                    flag = true;
                }
                if (aux->getDistanciaProxEst() != -1) {
                    cout << ' ' << aux->getDistanciaProxEst() << ' ';
                    flag = true;
                }
                if (flag) {
                    aux->imprime_NomesLinhas();
                }
                aux = aux->getProx();
            }
            cout << endl;
        }
    }
    
    No* insere_lista(string NomeProxEstacao, string NovaLinha, int DistanciaProxEst) {
        No *aux = raiz;
        while(aux != NULL) {
            if (aux->getNomeProxEstacao() == NomeProxEstacao) { // ja tem
                aux->insereNovaLinha(NovaLinha);
                return raiz;
            }
            aux = aux->getProx();
        }
        
        No *novo_no = new No(NomeProxEstacao, NovaLinha, DistanciaProxEst);

        if (lista_vazia()) {
            raiz = novo_no;
        }

        else {
            No *ant, *atual = raiz;
            while (atual != NULL && atual->getNomeProxEstacao() < NomeProxEstacao) {
                ant = atual;
                atual = atual->getProx();
            }
            if (atual == raiz) {
                novo_no->setProx(atual);
                raiz = novo_no;
            }
            else {
                novo_no->setProx(atual);
                ant->setProx(novo_no);
            }    
        }

        return raiz;
    }
};

class Estacao {
    private:
        string nome_estacao;
        Lista* lista_encadeada = new Lista();
    
    public:
        Estacao(string nome_estacao, string NomeProxEstacao, string NomeLinha, int DistanciaProxEst) {
            this->nome_estacao = nome_estacao;
            lista_encadeada->insere_lista(NomeProxEstacao, NomeLinha, DistanciaProxEst);
        }

        string getNomeEstacao() {
            return nome_estacao;
        }

        Lista* getListaEncadeada() {
            return lista_encadeada;
        }
};

class Caminhos {
    private:
        int num_estacoes;
        int distancia_total;
        vector<string> estacoes;

    public:
        Caminhos(int num, int dist, vector<string> est) {
            num_estacoes = num;
            distancia_total = dist;
            estacoes = est;
        }

        int getDistancia() {
            return distancia_total;
        }

        int getNumEstacoes() {
            return num_estacoes;
        }

        vector<string> getEstacoes() {
            return estacoes;
        }

        void imprime_caminho() {
            cout << "Numero de estacoes que serao percorridas: " << num_estacoes << endl;
            cout << "Distancia que sera percorrida: " << distancia_total << endl;
            int i;
            for (i = 0; i < estacoes.size() - 1; i++) {
                cout << estacoes[i] << ", ";
            }
            cout << estacoes[i] << endl << endl;
        }
};

class Grafo {
    public:
        vector<Estacao> vetor;
        map<string, bool> vertices;
        vector<Caminhos> todos_caminhos;

    public:
        void insere_caminho(int numero_estacoes, int distancia_estacoes, vector<string> nomes_estacoes) {
            Caminhos novo_caminho(numero_estacoes, distancia_estacoes, nomes_estacoes);

            if (todos_caminhos.empty()) {
                todos_caminhos.push_back(novo_caminho);
                return;
            }

            int i, j;
            for (i = 0; i < todos_caminhos.size(); i++) {
                if (todos_caminhos[i].getNumEstacoes() > numero_estacoes) {
                    todos_caminhos.insert(todos_caminhos.begin() + i, novo_caminho);
                    return;
                }
                if (todos_caminhos[i].getNumEstacoes() == numero_estacoes) {
                    for (j = 1; j < todos_caminhos[i].getEstacoes().size(); j++) {
                        if (todos_caminhos[i].getEstacoes()[j] > nomes_estacoes[j]) {
                            todos_caminhos.insert(todos_caminhos.begin() + i, novo_caminho);
                            return;
                        }
                    }
                }
            }
            todos_caminhos.insert(todos_caminhos.begin() + i, novo_caminho);
        }

        int getNumVertices() {
            return vertices.size();
        }

        void inserir_estacao(string NomeEstacao, string NomeProxEstacao, string NomeLinha, int DistanciaProxEst) {
            int i;
            bool flag1 = true;
            bool flag2 = true;
            for (auto valor: vertices) {
                if (valor.first == NomeEstacao) {
                    flag1 = false;
                }
                if (valor.first == NomeProxEstacao) {
                    flag2 = false;
                }
                if (!flag1 && !flag2) {
                    break;
                }
            }

            if (flag1) {
                vertices.insert(pair<string, bool>(NomeEstacao, false));
            }
            if (flag2) {
                vertices.insert(pair<string, bool>(NomeProxEstacao, false));
            }

            for (i = 0; i < vetor.size(); i++) {
                if (vetor[i].getNomeEstacao() == NomeEstacao) {
                    No *auxiliar = vetor[i].getListaEncadeada()->insere_lista(NomeProxEstacao, NomeLinha, DistanciaProxEst);
                    vetor[i].getListaEncadeada()->setRaiz(auxiliar);
                    return;
                }
            }

            Estacao nova_estacao(NomeEstacao, NomeProxEstacao, NomeLinha, DistanciaProxEst);
            if (vetor.empty()) {
                vetor.push_back(nova_estacao);
                return;
            }

            for (i = 0; i < vetor.size(); i++) {
                if (vetor[i].getNomeEstacao() > NomeEstacao) {
                    vetor.insert(vetor.begin() + i, nova_estacao);
                    return;
                }
            }
            vetor.insert(vetor.begin() + i, nova_estacao);
        }

        void imprime_grafo() {
            int i;
            for (i = 0; i < vetor.size(); i++) {
                cout << vetor[i].getNomeEstacao(); vetor[i].getListaEncadeada()->imprime_lista();
            }
        }

        int getTam() {
            return vetor.size();
        }

        void AcharTodosCaminhosRecursivo(string origem, string destino, map<string, bool> visited, vector<string> caminho, int &caminho_index, vector<int> distancia, int valor_distancia) {
            int i;
            int distancia_total = 0;
            No *aux;
            visited[origem] = true;
            caminho[caminho_index] = origem;
            distancia[caminho_index] = valor_distancia;
            vector<string> estacoes;

            caminho_index++;
			
			if (origem.size() == 0) {
                caminho_index--;
                visited[origem] = false;
				return;
			}

            if (origem == destino) {
                for (i = 0; i < caminho_index; i++) {
                    distancia_total += distancia[i];
                    estacoes.push_back(caminho[i]);
                }
                insere_caminho(caminho_index - 1, distancia_total, estacoes);
            }

            else {
                for (i = 0; i < vertices.size(); i++) {
                    if (vetor[i].getNomeEstacao() == origem) {
                        aux = vetor[i].getListaEncadeada()->getRaiz();
                        while (aux != NULL) {
                            if (!visited[aux->getNomeProxEstacao()]) {
                                AcharTodosCaminhosRecursivo(aux->getNomeProxEstacao(), destino, visited, caminho, caminho_index, distancia, aux->getDistanciaProxEst());
                            }
                            aux = aux->getProx();
                        }
                        break;
                    }
                }
                
            }
            caminho_index--;
            visited[origem] = false;
        }

       void AcharTodosCaminhos(string origem, string destino) {
           vector<int> distancia(vertices.size(), 0);
           vector<string> caminho(vertices.size());
           int caminho_index = 0;
           int valor_distancia = 0;

           AcharTodosCaminhosRecursivo(origem, destino, vertices, caminho, caminho_index, distancia, valor_distancia);
           
           if (todos_caminhos.size() == 0) {
               cout << "Não existe caminho entre as estações solicitadas." << endl;
            }
           else {
                int i;
                for (i = 0; i < todos_caminhos.size(); i++) {
                    todos_caminhos[i].imprime_caminho();
                }
            }
        }
};

void ler_Reg_arquivo(FILE **arq_leitura, registro *Reg) {
  /* Esta funcao realiza a leitura dos campos de 1 registro presentes no arquivo. Ela le os campos
    fixos e faz o tratamento dos campos variavies para que nao seja lido nenhum lixo de memoria. */

  int i;
  char caracter;

  // leitura dos campos fixos:
  fread(&Reg->CodEstacao, size_codEstacao, 1, *arq_leitura);
  fread(&Reg->CodLinha, size_codLinha, 1, *arq_leitura);
  fread(&Reg->CodProxEst, size_codProxEst, 1, *arq_leitura);
  fread(&Reg->DistanciaProxEst, size_distProxEst, 1, *arq_leitura);
  fread(&Reg->CodLinhaInteg, size_codLinhaInteg, 1, *arq_leitura);
  fread(&Reg->CodEstacaoInteg, size_codEstInteg, 1, *arq_leitura);

  // leitura dos campos variaveis:

  i = 0; // i eh um contador que vai guardar o tamanho de cada campo variavel
  fread(&caracter, sizeof(char), 1, *arq_leitura);
  while (caracter != '|')
  { // lendo caracteres do campo nomeEstacao ate chegar no pipe '|'
    i++;
    fread(&caracter, sizeof(char), 1, *arq_leitura);
  }
  fseek(*arq_leitura, -(i + 1), SEEK_CUR);

  Reg->NomeEstacao = (char *)malloc(sizeof(char) * (i + 1)); // alocando memoria suficiente para o campo
  if (Reg->NomeEstacao == NULL)
  {
    printf("Erro de alocacao de memoria.\n");
    exit(1);
  }

  fread(Reg->NomeEstacao, sizeof(char), i, *arq_leitura); // fazendo a leitura do campo variavel
  Reg->NomeEstacao[i] = '\0';

  fread(&caracter, sizeof(char), 1, *arq_leitura); // leitura o pipe '|'

  // segue a mesma logica para o campo nomeLinha:
  i = 0;
  fread(&caracter, sizeof(char), 1, *arq_leitura);
  while (caracter != '|')
  {
    i++;
    caracter = fgetc(*arq_leitura);
  }
  fseek(*arq_leitura, -(i + 1), SEEK_CUR);

  Reg->NomeLinha = (char *)malloc(sizeof(char) * (i + 1));
  if (Reg->NomeLinha == NULL)
  {
    printf("Erro de alocacao de memoria.\n");
    exit(1);
  }

  fread(Reg->NomeLinha, sizeof(char), i, *arq_leitura);
  Reg->NomeLinha[i] = '\0';

  fread(&caracter, sizeof(char), 1, *arq_leitura); // le o pipe

  // realizando a verificacao de lixo no final do registro para posicionar o cursor do arquivo na posicao correta:
  if (fread(&caracter, sizeof(char), 1, *arq_leitura))
  {
    while (caracter == '$')
    {
      fread(&caracter, sizeof(char), 1, *arq_leitura);
    }

    fseek(*arq_leitura, -1, SEEK_CUR);
  }
}

void libera_memoria_reg(registro *Reg) {
  /* Esta funcao libera a memoria dos campos variaveis de um registro. */

  free(Reg->NomeEstacao);
  free(Reg->NomeLinha);
}

int cria_Grafo(Grafo &grafo) {
  char *fileRead;
  char removido;
  char status;
  int tamanho;
  int flag = 0;
  registro R;

  map<int, string> info_estacao;

  scanf("%ms", &fileRead); // lendo nome do arquivo

  FILE *arq = fopen(fileRead, "rb"); // abrindo ele para leitura binaria
  if (arq == NULL) {
    printf("Falha no processamento do arquivo.\n");
    return 2;
  }

  fread(&status, size_status, 1, arq);
  if (status == 1) {
    printf("Falha no processamento do arquivo.\n");
    return 2;
  }

  fseek(arq, size_cabecalho - size_status, SEEK_CUR); // passando o cabecalho

  // pegar nomes_estacoes e codigos
  while (fread(&removido, size_removido, 1, arq)) {
    fseek(arq, (size_tamRegistro + size_proxLista), SEEK_CUR);
    ler_Reg_arquivo(&arq, &R);
    info_estacao[R.CodEstacao] = R.NomeEstacao;
    libera_memoria_reg(&R);
  }

  fseek(arq, size_cabecalho, SEEK_SET);
  while (fread(&removido, size_removido, 1, arq))
  { // verifica se o registro esta removido
    if (removido == '0')
    { // se nao estiver:
      flag = 1;
      fseek(arq, (size_tamRegistro + size_proxLista), SEEK_CUR); // passando o tamanho e o indicador proxLista
      ler_Reg_arquivo(&arq, &R); // realizando a leitura do registro

      grafo.inserir_estacao(R.NomeEstacao, info_estacao[R.CodProxEst], R.NomeLinha, R.DistanciaProxEst);

      if ((info_estacao[R.CodEstacao] != info_estacao[R.CodEstacaoInteg]) && (R.CodEstacaoInteg != -1)) {
        grafo.inserir_estacao(R.NomeEstacao, info_estacao[R.CodEstacaoInteg], "Integracao", 0.0);
      }
      libera_memoria_reg(&R);                                    // liberando memoria alocada para os campos variavies
    }

    else
    {                                            // o registro esta removido
      fread(&tamanho, size_tamRegistro, 1, arq); // le o tamanho do registro
      fseek(arq, tamanho, SEEK_CUR);             // pula ele na leitura do arquivo
    }
  }

  fclose(arq);
  free(fileRead);

  return flag;
}

int main() {
    int opcao;
    cin >> opcao;

  char CampoEstacaoOrigem[18];
  char CampoEstacaoDestino[19];
  char nomeEstacaoOrigem[50];
  char nomeEstacaoDestino[50];

  Grafo grafo;
  int flag = cria_Grafo(grafo);
  
  scanf("%s ", CampoEstacaoOrigem); 
  scan_quote_string(nomeEstacaoOrigem);
  scanf("%s ", CampoEstacaoDestino);
  scan_quote_string(nomeEstacaoDestino);

  //grafo.print();
  //cout << grafo.getNumVertices();
  //cout << "All different paths from " << nomeEstacaoOrigem << " to " << nomeEstacaoDestino << ':' << endl;
  grafo.AcharTodosCaminhos(nomeEstacaoOrigem, nomeEstacaoDestino);
    
    return 0;
} 

