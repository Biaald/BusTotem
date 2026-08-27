#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "IRepositorio.h"
#include "LinhaOnibus.h"

// SRP - Responsabilidade Única: RepositorioLinhas gerencia apenas a colecao de LinhasOnibus
//      e a persistencia em arquivo.
// DIP - Dependency Inversion Principle: implementa IRepositorioLinhas — modulos de alto nivel dependem
//      da interface, nao desta classe concreta.
//
// OCP - Open/Closed Principle: para trocar o formato de persistencia (ex: JSON, banco de dados)
//      basta criar outro IRepositorioLinhas sem modificar esta classe.
class RepositorioLinhas : public IRepositorioLinhas {
private:
    std::vector<LinhaOnibus> linhas;
    std::string nomeArquivo;

    static std::string trim(const std::string& s) {
        std::string r = s;
        while (!r.empty() && (r.front() == ' ' || r.front() == '\r' || r.front() == '\t')) r.erase(r.begin());
        while (!r.empty() && (r.back()  == ' ' || r.back()  == '\r' || r.back()  == '\t')) r.pop_back();
        return r;
    }

    static bool isNumber(const std::string& s) {
        if (s.empty()) return false;
        for (char c : s) if (!std::isdigit(c)) return false;
        return true;
    }

    static void splitParada(const std::string& linha, std::string out[3]) {
        std::string part;
        int idx = 0;
        for (char c : linha) {
            if (c == ';' && idx < 2) { out[idx++] = trim(part); part = ""; }
            else part += c;
        }
        out[idx] = trim(part);
    }

    LinhaOnibus* buscarLinha(int numero) {
        for (auto& l : linhas)
            if (l.getNumero() == numero) return &l;
        return nullptr;
    }

    const LinhaOnibus* buscarLinha(int numero) const {
        for (const auto& l : linhas)
            if (l.getNumero() == numero) return &l;
        return nullptr;
    }

public:
    explicit RepositorioLinhas(const std::string& arquivo = "arquivo.txt")
        : nomeArquivo(arquivo) {}

    // ── IRepositorioLinhas ──────────────────────────────────────────────

    void adicionarLinha(int numero, const std::string& companhia) override {
        if (buscarLinha(numero)) {
            std::cout << "Linha " << numero << " ja existe." << std::endl;
            return;
        }
        linhas.emplace_back(numero, companhia);
        salvar();
    }

    bool removerLinha(int numero) override {
        auto it = std::remove_if(linhas.begin(), linhas.end(),
            [numero](const LinhaOnibus& l){ return l.getNumero() == numero; });
        if (it == linhas.end()) return false;
        linhas.erase(it, linhas.end());
        salvar();
        return true;
    }

    void listarLinhas() const override {
        if (linhas.empty()) { std::cout << "Nenhuma linha cadastrada." << std::endl; return; }
        for (const auto& l : linhas)
            std::cout << l.getNumero() << " - " << l.getCompanhia() << std::endl;
    }

    bool linhaExiste(int numero) const override {
        return buscarLinha(numero) != nullptr;
    }

    void adicionarParada(int numLinha, const std::string& cidade,
                         const std::string& chegada, const std::string& saida,
                         int posicao) override {
        LinhaOnibus* l = buscarLinha(numLinha);
        if (!l) { std::cout << "Linha " << numLinha << " nao encontrada." << std::endl; return; }
        l->adicionarParada(Parada(cidade, chegada, saida), posicao);
        salvar();
    }

    void removerParada(int numLinha, int numParada) override {
        LinhaOnibus* l = buscarLinha(numLinha);
        if (!l) { std::cout << "Linha " << numLinha << " nao encontrada." << std::endl; return; }
        if (!l->removerParada(numParada))
            std::cout << "Parada " << numParada << " nao encontrada." << std::endl;
        else salvar();
    }

    void alterarParada(int numLinha, int numParada,
                       const std::string& cidade,
                       const std::string& chegada,
                       const std::string& saida) override {
        LinhaOnibus* l = buscarLinha(numLinha);
        if (!l) { std::cout << "Linha " << numLinha << " nao encontrada." << std::endl; return; }
        bool ok = false;
        if (!cidade.empty())  ok = l->alterarCidade(numParada, cidade);
        if (!chegada.empty()) ok = l->alterarChegada(numParada, chegada);
        if (!saida.empty())   ok = l->alterarSaida(numParada, saida);
        if (!ok) std::cout << "Parada " << numParada << " nao encontrada." << std::endl;
        else salvar();
    }

    void listarParadas(int numLinha) const override {
        const LinhaOnibus* l = buscarLinha(numLinha);
        if (!l) { std::cout << "Linha " << numLinha << " nao encontrada." << std::endl; return; }
        if (l->vazia()) { std::cout << "A linha esta vazia." << std::endl; return; }
        l->listar();
    }

    int quantidadeParadas(int numLinha) const override {
        const LinhaOnibus* l = buscarLinha(numLinha);
        return l ? l->quantidade() : 0;
    }

    // ── Persistencia ────────────────────────────────────────────────────

    void salvar() override {
        std::ofstream f(nomeArquivo, std::ios::trunc);
        for (const auto& l : linhas) {
            f << l.getNumero() << "\n" << l.getCompanhia() << "\n";
            for (const auto& p : l.getParadas())
                f << p.cidade << "; " << p.horarioChegada << "; " << p.horarioSaida << "\n";
        }
    }

    void carregar() override {
        std::ifstream f(nomeArquivo);
        if (!f.is_open()) { std::cout << "Arquivo nao encontrado. Iniciando vazio." << std::endl; return; }

        enum Estado { NUM, NOME, PARADA } estado = NUM;
        int numeroAtual = 0;
        std::string linha;

        while (std::getline(f, linha)) {
            linha = trim(linha);
            if (linha.empty()) continue;

            switch (estado) {
                case NUM:
                    if (isNumber(linha)) { numeroAtual = std::stoi(linha); estado = NOME; }
                    break;
                case NOME:
                    if (!isNumber(linha)) {
                        linhas.emplace_back(numeroAtual, linha);
                        estado = PARADA;
                    } else { numeroAtual = std::stoi(linha); }
                    break;
                case PARADA:
                    if (isNumber(linha)) { numeroAtual = std::stoi(linha); estado = NOME; }
                    else if (linha.find(';') != std::string::npos) {
                        std::string p[3];
                        splitParada(linha, p);
                        LinhaOnibus* l = buscarLinha(numeroAtual);
                        if (l) l->adicionarParada(Parada(p[0], p[1], p[2]));
                    }
                    break;
            }
        }
    }

    // Acesso direto para o BuscaService
    const std::vector<LinhaOnibus>& getLinhas() const { return linhas; }
};
