#include <iostream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

class Metro {
private:
    vector<vector <int>> matriz; // [ [0,0,0,0], [0,0,0,0], [0,0,0,0], [0,0,0,0] ]
    vector<vector<pair <int, int>>> lista;
    vector<int> estacoes; // [ 4 , 2, 3, 1 ]
public:
    Metro();
    void insereEstacao(int numEstacao);
    void removeEstacao(int numEstacao);
    void adicionaLigacao(int estacaoA, int estacaoB, int tempo);
    void removeLigacao(int estacaoA, int estacaoB);
    void dfs(int primeiraEstacao, int qualUsar);
    void dfsMatriz(int estacaoAtual, vector<bool>& visitados, vector<int>& ordemFinal);
    void dfsLista(int estacaoAtual, vector<bool>& visitados, vector<int>& ordemFinal);
    void bfs(int primeiraEstacao, int qualUsar);
    void bfsMatriz(vector<bool>& explorados, int estacaoInicial, queue <int>& visitados);
    void bfsLista(vector<bool>& explorados, int estacao, queue <int>& visitados);
    void caminhoQualquer(int primeiraEstacao, int estacaoFinal);
    void caminhoQualquerFunc(int estacaoAtual, vector<bool>& visitados, vector<int>& ordemFinal, int estacaoFinal, bool& achou);
    void imprimeMatriz();
    void imprimeLista();
};

Metro::Metro() {}

void Metro::insereEstacao(int numEstacao) {
    // Implementacao para matriz de adj
    // Valores da matriz da nova estação
    vector<int> tempEstacao;
    
    for (int i = 0; i < this->estacoes.size(); i++) {
        if (numEstacao == this->estacoes[i]) {
            cout << "A estacao ja existe dentro do Grafo" << endl;
            return;
        }
    }

    this->estacoes.push_back(numEstacao);

    // Cria um vector para colocar dentro da matriz ja com a quantidade de valores esperado
    for (int i = 0; i < this->estacoes.size(); i++) {
        tempEstacao.push_back(0);
    }

    for (int i = 0; i < this->matriz.size(); i++) {
        this->matriz[i].push_back(0);
    }

    this->matriz.push_back(tempEstacao);
    
    // Implementacao para lista de adj
    this->lista.push_back({  });
}

void Metro::removeEstacao(int numEstacao) {
    // Implementacao para matriz de adj
    int posEstacao;
    for (int i = 0; i < this->estacoes.size(); i++) {
        if (numEstacao == this->estacoes[i]) {
            posEstacao = i;
        }
    }

    for (int i = 0; i < this->matriz.size(); i++) {
        // Loopar pela estação para remover as adjacencias dentro das demais estacoes
        this->matriz[i].erase(this->matriz[i].begin() + posEstacao);
    }
    this->estacoes.erase(this->estacoes.begin() + posEstacao);
    this->matriz.erase(this->matriz.begin() + posEstacao);
    
    // Implementação para lista de adj
    this->lista.erase(this->lista.begin() + posEstacao);

    for (int i = 0; i < this->lista.size(); i++) {
        for (int j = 0; j < this->lista[i].size(); j++) {
            if (this->lista[i][j].first == numEstacao) {
                this->lista[i].erase(this->lista[i].begin() + j);
            }
        }
    }
}

void Metro::adicionaLigacao(int estacaoA, int estacaoB, int tempo) {
    // Implementacao para matriz de adj
    int posEstacaoA, posEstacaoB;
    for (int i = 0; i < this->estacoes.size(); i++) {
        if (estacaoA == this->estacoes[i]) {
            posEstacaoA = i;
        }
        else if (estacaoB == this->estacoes[i]) {
            posEstacaoB = i;
        }
    }
    this->matriz[posEstacaoA][posEstacaoB] = tempo;
    this->matriz[posEstacaoB][posEstacaoA] = tempo;
    // Implementacao para lista de adj
    pair <int, int> parTemporarioA(estacaoB, tempo);
    pair <int, int> parTemporarioB(estacaoA, tempo);
    this->lista[posEstacaoA].push_back(parTemporarioA);
    this->lista[posEstacaoB].push_back(parTemporarioB);

}

void Metro::removeLigacao(int estacaoA, int estacaoB) {
    // Implementação para matriz
    int posEstacaoA, posEstacaoB;
    for (int i = 0; i < this->estacoes.size(); i++) {
        if (estacaoA == this->estacoes[i]) {
            posEstacaoA = i;
        }
        else if (estacaoB == this->estacoes[i]) {
            posEstacaoB = i;
        }
    }
    this->matriz[posEstacaoA][posEstacaoB] = 0;
    this->matriz[posEstacaoB][posEstacaoA] = 0;

    // Implementação para lista
    for (int i = 0; i < this->lista.size(); i++) {
        for (int j = 0; j < this->lista[i].size(); j++) {
            cout << "Chegou aqui 1" << endl;
            if ((this->lista[i][j].first == estacaoA || this->lista[i][j].first == estacaoB) && (i + 1 == estacaoA || i + 1 == estacaoB)) {
                cout << this->lista[i][j].first;
                this->lista[i].erase(this->lista[i].begin() + j);
            }
        }
    }
}

void Metro::imprimeMatriz() {
    cout << "[" << endl;
    for (int i = 0; i < this->matriz.size(); i++) {
        cout << "[";
        //cout << this->matriz[i].size();
        for (int j = 0; j < this->matriz[i].size(); j++) {
            cout << this->matriz[i][j] << " ";
        }
        cout << "]" << endl;
    }
    cout << "]" << endl;
}

void Metro::imprimeLista() {
    cout << "[" << endl;
    for (int i = 0; i < this->lista.size(); i++) {
        cout << this->estacoes[i] << " -> ";
        for (int j = 0; j < this->lista[i].size(); j++) {
            cout << "[" << this->lista[i][j].first << "," << this->lista[i][j].second << "]";
        }
        cout << endl;
    }
    cout << "]" << endl;
}

// qualUsar quer dizer se vai usar para Lista ou Matriz
void Metro::dfs(int primeiraEstacao, int qualUsar) {
    vector<bool> visitados(this->estacoes.size(), false);
    int posicaoEstacao = -1;
    vector <int> ordemFinal;

    // Verifica a posicao da estacao na matriz de estacoes
    for (int i = 0; i < this->estacoes.size(); i++) {
        if (primeiraEstacao == this->estacoes[i]) {
            posicaoEstacao = i;
            break;
        }
    }

    // Verifica se encontrou a estacao
    if (posicaoEstacao == -1) {
        cout << "Estacao nao encontrada." << endl;
        return;
    }

    if (qualUsar == 1) {
        // Chama a funcao de recursao para Matriz
        dfsMatriz(posicaoEstacao, visitados, ordemFinal);

        cout << "Estacoes visitadas: ";
        for (int i = 0; i < ordemFinal.size(); i++) {
            cout << ordemFinal[i] << " ";
        }
        cout << endl;
    }
    else if (qualUsar == 2) {
        // Chama a funcao de recursao para Lista
        dfsLista(posicaoEstacao, visitados, ordemFinal);
        cout << "Estacoes visitadas: ";
        for (int i = 0; i < ordemFinal.size(); i++) {
            cout << ordemFinal[i] << " ";
        }
        cout << endl;
    }
}

void Metro::dfsMatriz(int estacaoAtual, vector<bool>& visitados, vector<int>& ordemFinal) {
    // Marca como visitada a estacao atual
    visitados[estacaoAtual] = true;
    ordemFinal.push_back(this->estacoes[estacaoAtual]);

    // Itera sobre os nós adjacentes ao nó atual
    for (int i = 0; i < this->estacoes.size(); i++) {
        // Indice da proxima estacao a ser visitada
        int proximaEstacao = -1;
        // Loopa pelas estacoes para verificar
        for (int j = 0; j < this->estacoes.size(); j++) {
            // Coleta a posição da proxima estacao para poder testar as condicoes para a recusividade
            if (this->estacoes[i] == this->estacoes[j]) {
                proximaEstacao = j;
                break;
            }
        }

        // Condicões de recursividade
        // Ele tem que ter achado algum no adjacente ( proximaEstacao != -1 )
        // Ele tem que ter ligacao com a estacao ( this->matriz[estacaoAtual][proximaEstacao] != 0 )
        // Ele não pode ter sido visitado ainda ( !visitados[proximaEstacao] )
        if (proximaEstacao != -1 && this->matriz[estacaoAtual][proximaEstacao] != 0 && !visitados[proximaEstacao]) {
            dfsMatriz(proximaEstacao, visitados, ordemFinal);
        }
    }
}

void Metro::dfsLista(int estacaoAtual, vector<bool>& visitados, vector<int>& ordemFinal) {
    // Mesma ideia da funcao anterior, porem para listas
    // Marca como visitada a estacao atual
    visitados[estacaoAtual] = true;
    ordemFinal.push_back(this->estacoes[estacaoAtual]);
    // Loopa pelas estacoes para ver as proximas estacoes
    for (int i = 0; i < this->lista[estacaoAtual].size(); i++) {
        int proximaEstacao = -1;
        // Loopa pela lista da estacao atual
        for (int j = 0; j < this->estacoes.size(); j++) {
            // Verifica quais são as estacoes conectadas aquela estacao
            if (this->lista[estacaoAtual][i].first == this->estacoes[j]) {
                // Pega o valor da estacao para poder visitar
                proximaEstacao = j;
                break;
            }
        }
        if (proximaEstacao != -1 && !visitados[proximaEstacao]) {
            dfsLista(proximaEstacao, visitados, ordemFinal);
        }
    }
}

void Metro::bfs(int primeiraEstacao, int qualUsar) {
    // qualUsar == 1 -> Usar bfsMatriz
    // qualUsar == 2 -> Usar bfsLista
    queue <int> visitados;
    vector<bool> explorados(this->estacoes.size(), false);

    for (int i = 0; i < explorados.size(); i++) {
        if (!explorados[i]) {
            if (qualUsar == 1) {
                bfsMatriz(explorados, this->estacoes[i], visitados);
            }
            else if (qualUsar == 2) {
                bfsLista(explorados, this->estacoes[i], visitados);
            }
        }
    }

    while (!visitados.empty()) {
        if (visitados.size() > 1) {
            cout << visitados.front() << " > ";
            visitados.pop();
        }
        else {
            cout << visitados.front();
            visitados.pop();
        }
    }
}

void Metro::bfsMatriz(vector<bool>& explorados, int estacao, queue <int>& visitados) {
    // Cria uma fila que define todos os que ainda vão ser visitados
    queue<int> aVisitar;
    int posEstacao = -1;

    for (int i = 0; i < this->estacoes.size(); i++) {
        if (this->estacoes[i] == estacao) {
            posEstacao = i;
        }
    }
    if (posEstacao == -1) {
        cout << "Estacao nao encontrada" << endl;
        return;
    }
    // Marca a estacao atual como visitada
    explorados[posEstacao] = true;
    aVisitar.push(posEstacao);
    int estacaoAtual;
    while (aVisitar.size()) {
        // Atribui o elemento mais antigo da fila para a variavel estacaoAtual
        estacaoAtual = aVisitar.front();
        visitados.push(aVisitar.front());
        aVisitar.pop();
        // Remove o elemento da fila
        
        vector <int> adjacencias = this->matriz[estacaoAtual - 1];
        for (int i = 0; i < adjacencias.size(); i++) {
            if (adjacencias[i] != 0 && !explorados[i]) {
                explorados[i] = true;
                aVisitar.push(this->estacoes[i]);
            }
        }

    }
}

// Implementar para Lista
void Metro::bfsLista(vector<bool>& explorados, int estacao, queue <int>& visitados) {
    // Cria uma fila que define todos os que ainda vão ser visitados
    queue<int> aVisitar;
    int posEstacao = estacao;
    // Marca a estacao atual como visitada
    for (int i = 0; i < this->estacoes.size(); i++) {
        if (this->estacoes[i] == estacao) {
            posEstacao = i;
        }
    }
    explorados[posEstacao] = true;

    aVisitar.push(posEstacao);
    int estacaoAtual;
    while (aVisitar.size()) {
        // Atribui o elemento mais antigo da fila para a variavel estacaoAtual
        estacaoAtual = aVisitar.front();
        visitados.push(aVisitar.front());
        aVisitar.pop();
        // Remove o elemento da fila

        vector <pair<int,int>> adjacencias (this->lista[estacaoAtual - 1]);
        for (int i = 0; i < adjacencias.size(); i++) {
            if (adjacencias[i].second != 0 && !explorados[i]) {
                explorados[i] = true;
                aVisitar.push(this->estacoes[i]);
            }
        }

    }
}

void Metro::caminhoQualquer(int primeiraEstacao, int estacaoFinal) {
    vector<bool> visitados(this->estacoes.size(), false);
    int posicaoEstacao = -1;
    vector <int> ordemFinal;
    bool achou=false;

    // Verifica a posicao da estacao na matriz de estacoes
    for (int i = 0; i < this->estacoes.size(); i++) {
        if (primeiraEstacao == this->estacoes[i]) {
            posicaoEstacao = i;
            break;
        }
    }

    // Verifica se encontrou a estacao
    if (posicaoEstacao == -1) {
        cout << "Estacao nao encontrada." << endl;
        return;
    }

    caminhoQualquerFunc(posicaoEstacao, visitados, ordemFinal, estacaoFinal, achou);
    cout << "Estacoes visitadas: ";
    for (int i = 0; i < ordemFinal.size(); i++) {
        cout << ordemFinal[i] << " ";
    }
    cout << endl;
}

void Metro::caminhoQualquerFunc(int estacaoAtual, vector<bool>& visitados, vector<int>& ordemFinal, int estacaoFinal, bool& achou) {
    if (achou) {
        return;
    }
    // Mesma ideia da funcao anterior, porem para listas
    // Marca como visitada a estacao atual
    visitados[estacaoAtual] = true;
    ordemFinal.push_back(this->estacoes[estacaoAtual]);
    // Loopa pelas estacoes para ver as proximas estacoes
    for (int i = 0; i < this->lista[estacaoAtual].size(); i++) {
        int proximaEstacao = -1;
        // Loopa pela lista da estacao atual
        for (int j = 0; j < this->estacoes.size(); j++) {
            // Verifica quais são as estacoes conectadas aquela estacao
            if (this->lista[estacaoAtual][i].first == this->estacoes[j]) {
                // Pega o valor da estacao para poder visitar
                proximaEstacao = this->lista[estacaoAtual][i].first;
                break;
            }
        }
        if (proximaEstacao == estacaoFinal) {
            achou = true;
            if (ordemFinal.back() != proximaEstacao) {
                ordemFinal.push_back(proximaEstacao);
                visitados[proximaEstacao - 1] = true;
            }
            break;
        }
        else {
            if (achou) {
                break;
            }

            for (int i = 0; i < this->estacoes.size(); i++) {
                if (proximaEstacao == this->estacoes[i]) {
                    proximaEstacao = i;
                }
            }
            if (proximaEstacao != -1 && !visitados[proximaEstacao]) {
                caminhoQualquerFunc(proximaEstacao, visitados, ordemFinal, estacaoFinal, achou);
            }
        }
        
    }
}

int main()
{

    Metro m1;
    int estacaoA, estacaoB, op, tempo, formato;
    while (true) {

        int op;
        cout << "[1] - Adicionar estacao" << endl << "[2] - Remover estacao" << endl << "[3] - Adicionar ligacao" << endl;
        cout << "[4] - Remover ligacao" << endl << "[5] - Imprimir sistema" << endl << "[6] - Busca de profundidade" << endl;
        cout << "[7] - Busca de largura" << endl << "[8] - Busca de um caminho qualquer" << endl;
        cout << "[0] - Sair" << endl;
        cout << "Opcao: "; cin >> op;

        system("cls");

        if (op == 1) {
            cout << "Qual o numero da estacao: ";
            cin >> estacaoA;
            m1.insereEstacao(estacaoA);
        }
        else if (op == 2) {
            cout << "Qual o numero da estacao: ";
            cin >> estacaoA;
            m1.removeEstacao(estacaoA);
        }
        else if (op == 3) {
            cout << "Qual o numero da primeira estacao: ";
            cin >> estacaoA;
            cout << "Qual o numero da segunda estacao: ";
            cin >> estacaoB;
            cout << "Qual o tempo entre as estacoes";
            cin >> tempo;
            m1.adicionaLigacao(estacaoA, estacaoB, tempo);
        }
        else if (op == 4) {
            cout << "Qual o numero da primeira estacao: " << endl;
            cin >> estacaoA;
            cout << "Qual o numero da segunda estacao: " << endl;
            cin >> estacaoB;
            m1.removeLigacao(estacaoA, estacaoB);
        }
        else if (op == 5) {
            cout << "[1] - Matriz de adjacencias" << endl << "[2] - Lista de adjacencias" << endl;
            cin >> formato;
            if (formato == 1) {
                m1.imprimeMatriz();
            }
            else if (formato == 2) {
                m1.imprimeLista();
            }
        }
        else if (op == 6) {
            cout << "Qual o numero da estacao inicial: ";
            cin >> estacaoA;
            cout << "[1] - Matriz" << endl << "[2] - Lista" << endl;
            cin >> op;
            if (op == 1) {
                m1.dfs(estacaoA, 1);
            }
            else {
                m1.dfs(estacaoA, 2);
            }
        }
        else if (op == 7) {
            cout << "Qual o numero da estacao inicial: ";
            cin >> estacaoA;
            cout << "[1] - Matriz" << endl << "[2] - Lista" << endl;
            cin >> op;
            if (op == 1) {
                m1.bfs(estacaoA, 1);
            }
            else {
                m1.bfs(estacaoA, 2);
            }
        }
        else if (op == 8) {
            cout << "Qual o numero da primeira estacao: " << endl;
            cin >> estacaoA;
            cout << "Qual o numero da segunda estacao: " << endl;
            cin >> estacaoB;
            m1.caminhoQualquer(estacaoA, estacaoB);
        }
        else if (op == 0) {
            cout << "Obrigado por usar nosso sistema";
            break;
        }
    }
}