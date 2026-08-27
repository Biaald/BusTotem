#include <iostream>
#include <string>
#include "RepositorioLinhas.h"
#include "BuscaService.h"
#include "MenuAdmin.h"
#include "Validator.h"

static std::string trim(const std::string& s) {
    std::string r = s;
    while (!r.empty() && (r.front() == ' ' || r.front() == '\r')) r.erase(r.begin());
    while (!r.empty() && (r.back()  == ' ' || r.back()  == '\r')) r.pop_back();
    return r;
}

int main() {
    // DIP: main depende de IRepositorioLinhas via RepositorioLinhas concreto.
    // Para trocar por um banco de dados, basta trocar esta linha.
    RepositorioLinhas repositorio("arquivo.txt");
    repositorio.carregar();

    BuscaService buscaService(repositorio);
    MenuAdmin    menuAdmin(repositorio);

    char decisao;
    do {
        std::cout << "\nOla, seja bem-vindo ao BusTotem!" << std::endl;
        std::cout << "<-------------------------------------------------->" << std::endl;
        std::cout << "|  1 - Encontrar linhas de onibus                  |" << std::endl;
        std::cout << "|  2 - Manutencao (Administrador)                  |" << std::endl;
        std::cout << "|  3 - Sair                                        |" << std::endl;
        std::cout << "<-------------------------------------------------->" << std::endl;
        std::cout << "> ";
        std::cin >> decisao;
        std::cin.ignore();

        switch (decisao) {
            case '1': {
                std::string origem, destino, horario;

                std::cout << "De onde voce esta partindo? (Enter para qualquer origem): ";
                std::getline(std::cin, origem);
                origem = trim(origem);

                std::cout << "Para onde deseja ir? ";
                std::getline(std::cin, destino);
                destino = trim(destino);

                std::cout << "A partir de que horario deseja partir? (HH:MM): ";
                std::getline(std::cin, horario);
                horario = trim(horario);

                if (destino.empty()) {
                    std::cout << "Destino nao pode ser vazio." << std::endl;
                } else if (!Validator::horarioValido(horario)) {
                    // mensagem ja impressa por Validator
                } else {
                    ResultadoBusca resultado = buscaService.buscar(destino, horario, origem);
                    buscaService.exibirResultado(resultado);
                }
                break;
            }

            case '2': {
                std::string senha;
                std::cout << "Senha de administrador: ";
                std::getline(std::cin, senha);
                if (senha == "1") {
                    menuAdmin.executar();
                } else {
                    std::cout << "Senha incorreta." << std::endl;
                }
                break;
            }

            case '3':
                std::cout << "Obrigado por usar o BusTotem. Ate a proxima!" << std::endl;
                break;

            default:
                std::cout << "Opcao invalida." << std::endl;
        }

    } while (decisao != '3');

    return 0;
}
