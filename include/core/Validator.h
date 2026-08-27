#pragma once
#include <string>
#include <iostream>

// SRP - Responsabilidade Única: Validator tem uma unica responsabilidade — validar entradas do usuario.
// Antes, horarioValido() e paradaValida() viviam dentro de DoubleList,
// misturando logica de validacao com logica de estrutura de dados.
class Validator {
public:
    static bool horarioValido(const std::string& horario) {
        if (horario.length() != 5) {
            std::cout << "Erro: Horario deve ter 5 caracteres (HH:MM)" << std::endl;
            return false;
        }
        for (int i = 0; i < 5; i++) {
            if (i == 2) {
                if (horario[i] != ':') {
                    std::cout << "Erro: Separador de horas invalido!" << std::endl;
                    return false;
                }
            } else if (!std::isdigit(horario[i])) {
                std::cout << "Erro: Caractere nao numerico no indice " << i << std::endl;
                return false;
            }
        }
        int horas   = std::stoi(horario.substr(0, 2));
        int minutos = std::stoi(horario.substr(3, 2));
        if (horas < 0 || horas > 23 || minutos < 0 || minutos > 59) {
            std::cout << "Erro: Horario fora dos limites" << std::endl;
            return false;
        }
        return true;
    }

    static bool nomeValido(const std::string& nome) {
        for (char c : nome)
            if (!std::isalpha(c) && !std::isspace(c)) return false;
        return true;
    }

    static bool numLinhaValido(int linha) {
        return linha >= 10000 && linha <= 99999;
    }
};
