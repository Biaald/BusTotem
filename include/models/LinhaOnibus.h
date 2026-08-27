#pragma once
#include <string>
#include <vector>
#include "Parada.h"

// SRP - Responsabilidade Única: LinhaOnibus gerencia apenas os dados e paradas de UMA linha.
// Antes, Node1 misturava: dados da linha + ponteiro de lista encadeada
// + instancia de DoubleList + logica de insercao — multiplas responsabilidades.
//
// OCP - Open/Closed Principle: ao usar std::vector<Parada>, e facil estender comportamentos
// (ex: ordenar paradas, filtrar) sem modificar esta classe.
class LinhaOnibus {
private:
    int numero;
    std::string companhia;
    std::vector<Parada> paradas;

public:
    LinhaOnibus(int num, const std::string& comp)
        : numero(num), companhia(comp) {}

    int getNumero() const { return numero; }
    const std::string& getCompanhia() const { return companhia; }
    void setCompanhia(const std::string& c) { companhia = c; }

    // Retorna todas as paradas (somente leitura)
    const std::vector<Parada>& getParadas() const { return paradas; }

    bool vazia() const { return paradas.empty(); }
    int  quantidade() const { return (int)paradas.size(); }

    void adicionarParada(const Parada& p, int posicao = -1) {
        if (posicao < 0 || posicao >= (int)paradas.size()) {
            paradas.push_back(p);
        } else {
            paradas.insert(paradas.begin() + posicao, p);
        }
    }

    bool removerParada(int indice) {
        if (indice < 1 || indice > (int)paradas.size()) return false;
        paradas.erase(paradas.begin() + indice - 1);
        return true;
    }

    bool alterarCidade(int indice, const std::string& novaCidade) {
        if (indice < 1 || indice > (int)paradas.size()) return false;
        paradas[indice - 1].cidade = novaCidade;
        return true;
    }

    bool alterarChegada(int indice, const std::string& novoHorario) {
        if (indice < 1 || indice > (int)paradas.size()) return false;
        paradas[indice - 1].horarioChegada = novoHorario;
        return true;
    }

    bool alterarSaida(int indice, const std::string& novoHorario) {
        if (indice < 1 || indice > (int)paradas.size()) return false;
        paradas[indice - 1].horarioSaida = novoHorario;
        return true;
    }

    void listar() const {
        for (int i = 0; i < (int)paradas.size(); i++) {
            std::cout << (i + 1) << " - " << paradas[i].cidade
                      << "; " << paradas[i].horarioChegada
                      << "; " << paradas[i].horarioSaida << std::endl;
        }
    }
};
