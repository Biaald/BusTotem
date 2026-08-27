#pragma once
#include <iostream>
#include <string>
#include "IRepositorio.h"
#include "Validator.h"

// SRP: MenuAdmin tem uma unica responsabilidade — interacao com o administrador.
// Antes, toda logica de menu (leitura de input, validacao, chamadas de lista)
// estava misturada dentro do main(), tornando-o com mais de 400 linhas.
//
// DIP: depende de IRepositorioLinhas, nao de ForwardList diretamente.
class MenuAdmin {
private:
    IRepositorioLinhas& repo;

    static std::string lerLinha(const std::string& prompt) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    void inserirParada() {
        repo.listarLinhas();
        int numLinha;
        std::cout << "Numero da linha: ";
        std::cin >> numLinha; std::cin.ignore();
        if (!repo.linhaExiste(numLinha)) { std::cerr << "Linha nao encontrada." << std::endl; return; }

        int pos;
        std::cout << "Inserir apos a posicao (0 = fim): ";
        std::cin >> pos; std::cin.ignore();

        std::string cidade = lerLinha("Nome da parada: ");
        if (!Validator::nomeValido(cidade)) { std::cerr << "Nome invalido." << std::endl; return; }

        std::string chegada = lerLinha("Horario de chegada (HH:MM): ");
        if (!Validator::horarioValido(chegada)) return;

        std::string saida = lerLinha("Horario de saida (HH:MM): ");
        if (!Validator::horarioValido(saida)) return;

        repo.adicionarParada(numLinha, cidade, chegada, saida, pos);
        std::cout << "Parada inserida!" << std::endl;
    }

    void listarParadas() {
        repo.listarLinhas();
        int numLinha;
        std::cout << "Numero da linha: ";
        std::cin >> numLinha; std::cin.ignore();
        repo.listarParadas(numLinha);
    }

    void alterarParada() {
        repo.listarLinhas();
        int numLinha;
        std::cout << "Numero da linha: ";
        std::cin >> numLinha; std::cin.ignore();
        if (!repo.linhaExiste(numLinha)) { std::cerr << "Linha nao encontrada." << std::endl; return; }

        repo.listarParadas(numLinha);
        int numParada;
        std::cout << "Numero da parada: ";
        std::cin >> numParada; std::cin.ignore();

        std::cout << "1-Cidade  2-Chegada  3-Saida: ";
        int op; std::cin >> op; std::cin.ignore();

        if (op == 1) {
            std::string c = lerLinha("Nova cidade: ");
            if (!Validator::nomeValido(c)) { std::cerr << "Nome invalido." << std::endl; return; }
            repo.alterarParada(numLinha, numParada, c, "", "");
        } else if (op == 2) {
            std::string h = lerLinha("Novo horario de chegada (HH:MM): ");
            if (!Validator::horarioValido(h)) return;
            repo.alterarParada(numLinha, numParada, "", h, "");
        } else if (op == 3) {
            std::string h = lerLinha("Novo horario de saida (HH:MM): ");
            if (!Validator::horarioValido(h)) return;
            repo.alterarParada(numLinha, numParada, "", "", h);
        } else {
            std::cerr << "Opcao invalida." << std::endl;
        }
    }

    void deletarParada() {
        repo.listarLinhas();
        int numLinha;
        std::cout << "Numero da linha: ";
        std::cin >> numLinha; std::cin.ignore();
        repo.listarParadas(numLinha);

        int numParada;
        std::cout << "Numero da parada a deletar: ";
        std::cin >> numParada; std::cin.ignore();
        repo.removerParada(numLinha, numParada);
        std::cout << "Parada removida." << std::endl;
    }

    void deletarLinha() {
        repo.listarLinhas();
        int numLinha;
        std::cout << "Numero da linha a excluir: ";
        std::cin >> numLinha; std::cin.ignore();
        if (repo.removerLinha(numLinha))
            std::cout << "Linha " << numLinha << " excluida." << std::endl;
        else
            std::cout << "Linha nao encontrada." << std::endl;
    }

    void inserirLinha() {
        int numLinha;
        std::cout << "Numero da linha (5 digitos): ";
        std::cin >> numLinha; std::cin.ignore();
        if (!Validator::numLinhaValido(numLinha)) {
            std::cerr << "Numero invalido. Deve ter 5 digitos." << std::endl; return;
        }
        std::string comp = lerLinha("Nome da companhia: ");
        if (!Validator::nomeValido(comp)) { std::cerr << "Nome invalido." << std::endl; return; }
        repo.adicionarLinha(numLinha, comp);
        std::cout << "Linha inserida!" << std::endl;
    }

public:
    explicit MenuAdmin(IRepositorioLinhas& r) : repo(r) {}

    void executar() {
        char opcao;
        do {
            std::cout << "\n<-- Menu Administrador -->" << std::endl;
            std::cout << "1 - Inserir parada\n2 - Listar linhas\n3 - Alterar parada\n";
            std::cout << "4 - Deletar parada\n5 - Deletar linha\n6 - Inserir linha\n";
            std::cout << "7 - Listar paradas\n8 - Voltar\n> ";
            std::cin >> opcao; std::cin.ignore();

            switch (opcao) {
                case '1': inserirParada();  break;
                case '2': listarParadas();  break;
                case '3': alterarParada();  break;
                case '4': deletarParada();  break;
                case '5': deletarLinha();   break;
                case '6': inserirLinha();   break;
                case '7': repo.listarLinhas(); break;
                case '8': std::cout << "Voltando ao menu principal." << std::endl; break;
                default:  std::cout << "Opcao invalida." << std::endl;
            }
        } while (opcao != '8');
    }
};
