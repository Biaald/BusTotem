#pragma once
#include <string>
#include <iostream>
#include "IRepositorio.h"
#include "RepositorioLinhas.h"
#include "TimeUtils.h"

// SRP - Responsabilidade Única: BuscaService tem uma unica responsabilidade — realizar buscas de rotas.
// Antes, toda a logica de busca estava dentro de ForwardList, que ja era
// responsavel por gerenciar a lista encadeada e a persistencia.
//
// DIP - Dependency Inversion Principle: recebe IRepositorioLinhas por referencia — depende da abstracao,
// nao da implementacao concreta RepositorioLinhas.
struct ResultadoBusca {
    bool encontrou = false;
    std::string companhia;
    int         numeroLinha = 0;
    std::string cidadeEmbarque;
    std::string horarioSaida;
    std::string cidadeDestino;
    std::string horarioChegada;
    int         duracaoMinutos = 0;
    const LinhaOnibus* linha = nullptr;
};

class BuscaService {
private:
    const RepositorioLinhas& repositorio;

public:
    // DIP: depende da interface abstrata, nao de ForwardList diretamente
    explicit BuscaService(const RepositorioLinhas& repo) : repositorio(repo) {}

    // Busca a melhor opcao de embarque para o destino a partir do horario,
    // opcionalmente restringindo a uma cidade de embarque especifica.
    ResultadoBusca buscar(const std::string& destino,
                      const std::string& horarioMinimo,
                      const std::string& origemDesejada = "") const {
    double target = TimeUtils::paraDecimal(horarioMinimo);
    double melhorSaida = 25.0;
    ResultadoBusca melhor;

    for (const LinhaOnibus& linha : repositorio.getLinhas()) {
        const auto& paradas = linha.getParadas();
        
        for (int i = 0; i < (int)paradas.size(); i++) {
            
            // 1. Verifica se esta parada eh a origem (ou se aceita qualquer origem)
            if (origemDesejada.empty() || paradas[i].cidade == origemDesejada) {
                int idxEmbarque = i;

                // 2. Verifica se o horário desta parada específica serve
                double saidaNum = TimeUtils::paraDecimal(paradas[idxEmbarque].horarioSaida);
                if (saidaNum < target) continue; // Cedo demais para ESTA parada

                // 3. Se a origem serve, procura o destino APÓS esta parada
                for (int j = idxEmbarque + 1; j < (int)paradas.size(); j++) {
                    if (paradas[j].cidade == destino) {
                        int idxDestino = j;

                        // 4. Se encontrou um destino válido, vê se é o melhor horário global
                        if (saidaNum < melhorSaida) {
                            melhorSaida = saidaNum;
                            melhor.encontrou      = true;
                            melhor.companhia      = linha.getCompanhia();
                            melhor.numeroLinha    = linha.getNumero();
                            melhor.cidadeEmbarque = paradas[idxEmbarque].cidade;
                            melhor.horarioSaida   = paradas[idxEmbarque].horarioSaida;
                            melhor.cidadeDestino  = destino;
                            melhor.horarioChegada = paradas[idxDestino].horarioChegada;
                            melhor.duracaoMinutos = TimeUtils::duracaoMinutos(
                                melhor.horarioSaida, melhor.horarioChegada);
                            melhor.linha = &linha;
                        }
                        // Encontrou o destino para esta origem, pode parar de procurar destino
                        break; 
                        }
                    }
                }
            }
        }
    return melhor;
}

    void exibirResultado(const ResultadoBusca& r) const {
        if (!r.encontrou) {
            std::cout << "Nao ha horarios disponiveis para o trecho solicitado." << std::endl;
            return;
        }
        std::cout << "\n=== Melhor opcao encontrada ===" << std::endl;
        std::cout << "Embarque em   : " << r.cidadeEmbarque << " as " << r.horarioSaida   << std::endl;
        std::cout << "Desembarque em: " << r.cidadeDestino  << " as " << r.horarioChegada << std::endl;
        std::cout << "Duracao       : " << TimeUtils::formatar(r.duracaoMinutos)           << std::endl;
        std::cout << "Linha         : " << r.numeroLinha    << " - " << r.companhia        << std::endl;
        std::cout << "\nParadas da linha:" << std::endl;
        r.linha->listar();
    }
};
