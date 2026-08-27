#pragma once
#include <string>

// SRP - Responsabilidade Única: Parada e um objeto de dados puro — representa uma parada de onibus.
// Antes, os dados de parada estavam dentro de Node, que tambem gerenciava
// ponteiros de lista encadeada, misturando dado com estrutura.
struct Parada {
    std::string cidade;
    std::string horarioChegada;
    std::string horarioSaida;

    Parada(const std::string& c, const std::string& cheg, const std::string& said)
        : cidade(c), horarioChegada(cheg), horarioSaida(said) {}
};
