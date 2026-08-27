#pragma once
#include <string>

// DIP - Dependency Inversion Principle: modulos de alto nivel (BuscaService, MenuAdmin) nao dependem
// diretamente de ForwardList. Dependem desta interface abstrata.
//
// ISP - Interface Segregation Principle: a interface e enxuta — define apenas o contrato minimo que
// qualquer repositorio de linhas precisa cumprir, sem forcar implementacoes
// a depender de metodos que nao usam.
class IRepositorioLinhas {
public:
    virtual ~IRepositorioLinhas() = default;

    virtual void adicionarLinha(int numero, const std::string& companhia) = 0;
    virtual bool removerLinha(int numero) = 0;
    virtual void listarLinhas() const = 0;
    virtual bool linhaExiste(int numero) const = 0;

    virtual void adicionarParada(int numLinha, const std::string& cidade,
                                 const std::string& chegada, const std::string& saida,
                                 int posicao) = 0;
    virtual void removerParada(int numLinha, int numParada) = 0;
    virtual void alterarParada(int numLinha, int numParada,
                               const std::string& cidade,
                               const std::string& chegada,
                               const std::string& saida) = 0;
    virtual void listarParadas(int numLinha) const = 0;
    virtual int  quantidadeParadas(int numLinha) const = 0;

    virtual void salvar() = 0;
    virtual void carregar() = 0;
};
