// MetroP2.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <vector>

using namespace std;

class Metro {
private:
    vector<vector <int>> matriz; // [ [0,0,0,0], [0,0,0,0], [0,0,0,0], [0,0,0,0] ]
    vector<int> estacoes; // [ 4 , 2, 3, 1 ] 
    vector<int> lista;
public:
    Metro();
    void insereEstacao(int numEstacao);
    void removeEstacao(int numEstacao);
    void imprimeMatriz();
    void imprimeLista();
};

Metro::Metro() {}

void Metro::insereEstacao(int numEstacao) {
    // Valores da matriz da nova estação
    vector<int> tempEstacao;
    
    
    for (int i = 0; i < this->estacoes.size(); i++) {
        if (numEstacao == this->estacoes[i]) {
            cout << "A estacao ja existe dentro do Grafo" << endl;
            return;
        }
    }

    this->estacoes.push_back(numEstacao);

    for (int i = 0; i < this->estacoes.size(); i++) {
        tempEstacao.push_back(0);
    }

    for (int i = 0; i < this->matriz.size(); i++) {
        this->matriz[i].push_back(0);
    }

    this->matriz.push_back(tempEstacao);
}

void Metro::removeEstacao(int numEstacao) {
    int posEstacao;
    for (int i = 0; i < this->estacoes.size(); i++) {
        if (numEstacao == this->estacoes[i]) {
            posEstacao = i;
        }
    }

    for (int i = 0; i < this->matriz.size(); i++) {
        // Loopar pela estação para 
        if (i == posEstacao) {
            this->estacoes.erase(this->estacoes.begin() + posEstacao);
            this->matriz.erase(this->matriz.begin() + posEstacao);
        } else {
            for (int j = 0; j < this->matriz[i].size(); j++) {
                this->matriz[i].erase(this->matriz[i].begin() + posEstacao);
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

int main()
{
    Metro m1;
    m1.insereEstacao(1);
    m1.insereEstacao(2);
    m1.insereEstacao(3);
    m1.insereEstacao(4);
    m1.imprimeMatriz();
    m1.removeEstacao(3);
    m1.imprimeMatriz();
    // cout << "Ola mundo!";
}