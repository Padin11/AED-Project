#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

struct Filme {
    string nome;
    string categoria;
    vector<int> similares;  // Índices dos filmes similares
};

void carregarFilmes(vector<Filme>& filmes) {
    ifstream file("filmes.txt");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo de filmes!" << endl;
        return;
    }
    
    string linha;
    while (getline(file, linha)) {
        Filme filme;
        size_t pos = 0;
        
        pos = linha.find(",");
        filme.nome = linha.substr(0, pos);
        linha.erase(0, pos + 1);
        
        pos = linha.find(",");
        filme.categoria = linha.substr(0, pos);
        linha.erase(0, pos + 1);
        
        while ((pos = linha.find(",")) != string::npos) {
            filme.similares.push_back(stoi(linha.substr(0, pos)));
            linha.erase(0, pos + 1);
        }
        filme.similares.push_back(stoi(linha)); // Último índice

        filmes.push_back(filme);
    }

    file.close();
}

void salvarFilmes(const vector<Filme>& filmes) {
    ofstream file("filmes.txt");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo de filmes!" << endl;
        return;
    }

    for (const auto& filme : filmes) {
        file << filme.nome << "," << filme.categoria;
        for (int similar : filme.similares) {
            file << "," << similar;
        }
        file << endl;
    }

    file.close();
}

void imprimirFilmes(const vector<Filme>& filmes, const map<string, int>& vistos) {
    for (int i = 0; i < filmes.size(); ++i) {
        if (vistos.find(filmes[i].nome) == vistos.end()) {  // Só imprime filmes que não foram vistos
            cout << i + 1 << ": " << filmes[i].nome << " (" << filmes[i].categoria << ")" << endl;
        }
    }
}

void recomendarFilmes(const vector<Filme>& filmes, int index, const map<string, int>& vistos) {
    cout << "Filmes recomendados baseados em sua escolha:" << endl;
    int recomendacoes = 0;
    for (int similar : filmes[index].similares) {
        if (vistos.find(filmes[similar].nome) == vistos.end() && filmes[similar].categoria == filmes[index].categoria) {  // Recomenda apenas filmes não vistos e da mesma categoria
            cout << "- " << filmes[similar].nome << endl;
            recomendacoes++;
        }
        if (recomendacoes == 5) break;  // Limita a 5 recomendações
    }
}

void adicionarFilme(vector<Filme>& filmes) {
    Filme novoFilme;
    string input;

    cout << "Digite o nome do filme: ";
    cin.ignore(); // Limpa o buffer do cin
    getline(cin, novoFilme.nome);

    cout << "Digite a categoria do filme: ";
    getline(cin, novoFilme.categoria);

    cout << "Digite os índices dos filmes similares separados por espaço: ";
    getline(cin, input);
    stringstream ss(input);
    int index;
    while (ss >> index) {
        novoFilme.similares.push_back(index);
    }

    filmes.push_back(novoFilme);
    salvarFilmes(filmes);
    cout << "Filme adicionado com sucesso!" << endl;
}

int main() {
    vector<Filme> filmes;
    carregarFilmes(filmes);

    map<string, int> vistos;  // Mapa para guardar os filmes vistos e suas notas

    int escolha;
    int nota;

    do {
        cout << "Por favor, escolha uma opção:" << endl;
        cout << "1. Escolher um filme para avaliar" << endl;
        cout << "2. Adicionar um novo filme" << endl;
        cout << "3. Sair" << endl;
        cin >> escolha;

        if (escolha == 1) {
            do {
                cout << "Escolha um filme para avaliar (digite o número):" << endl;
                imprimirFilmes(filmes, vistos);

                cin >> escolha;
                escolha -= 1;  // Ajuste para base zero

                if (escolha < 0 || escolha >= filmes.size() || vistos.find(filmes[escolha].nome) != vistos.end()) {
                    cout << "Escolha inválida ou filme já visto! Por favor, tente novamente." << endl;
                    continue;
                }

                cout << "Você escolheu: " << filmes[escolha].nome << endl;
                cout << "Por favor, dê uma nota de 0 a 5 para o filme:" << endl;
                cin >> nota;

                if (nota < 0 || nota > 5) {
                    cout << "Nota inválida! Por favor, insira uma nota entre 0 e 5." << endl;
                    continue;
                }

                // Adiciona o filme e a nota ao mapa de vistos
                vistos[filmes[escolha].nome] = nota;

                if (nota >= 4) {
                    recomendarFilmes(filmes, escolha, vistos);
                    break;
                } else {
                    cout << "Você deu uma nota menor que 4. Vamos tentar outro filme!" << endl;
                }

            } while (true);
        } else if (escolha == 2) {
            adicionarFilme(filmes);
        } else if (escolha == 3) {
            break;
        } else {
            cout << "Opção inválida! Por favor, tente novamente." << endl;
        }
    } while (true);

    return 0;
}
