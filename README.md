# 🚌 BusTotem

Um sistema desenvolvido em C++ para consulta e otimização de rotas de ônibus urbanos. O projeto nasceu na disciplina de Estruturas de Dados / Engenharia de Software do curso de Engenharia de Computação e foi inteiramente **refatorado seguindo os princípios SOLID**.

O objetivo principal é fornecer o horário de partida mais próximo com base na origem, destino e horário atual do usuário, garantindo um código limpo, testável e escalável.

## 🚀 Funcionalidades
- Cadastro, consulta e gerenciamento de rotas e paradas (via `MenuAdmin`).
- **Busca Inteligente:** O `BuscaService` calcula e retorna o ônibus disponível mais próximo após um horário específico.
- Tratamento de tempo e horários de forma isolada e segura.

## 🏗️ Arquitetura e Refatoração (SOLID)

O projeto original possuía funções infladas (o `main` acumulava ~500 linhas) e estruturas de dados super-acopladas (uma `DoubleList` que acumulava 5 responsabilidades). O sistema foi refatorado adotando Arquitetura Limpa e **SOLID**:

*   **S - Single Responsibility Principle (SRP):** Cada classe tem apenas um motivo para mudar. O `main` agora é um orquestrador fino que apenas injeta dependências. Responsabilidades foram divididas em: `Validator` (validações), `TimeUtils` (cálculos de horário), `Parada` (dados), `LinhaOnibus` (gerenciamento de paradas), `RepositorioLinhas` (persistência), `BuscaService` (lógica de negócio) e `MenuAdmin` (UI).
*   **O - Open/Closed Principle (OCP):** A classe `LinhaOnibus` gerencia as paradas via `std::vector<Parada>`. Adicionar novas regras de ordenação ou filtros não exige modificar a classe existente. A persistência implementa a interface `IRepositorioLinhas`, permitindo a criação de novos repositórios (ex: Banco de Dados ou JSON) sem alterar o código base.
*   **L - Liskov Substitution Principle (LSP):** O sistema garante que implementações concretas como `RepositorioLinhas` podem ser perfeitamente substituídas por qualquer outra implementação de `IRepositorioLinhas` sem quebrar clientes como `BuscaService` ou `MenuAdmin`.
*   **I - Interface Segregation Principle (ISP):** A interface `IRepositorioLinhas` expõe estritamente os métodos necessários para seus clientes. Diferente da versão anterior, onde o sistema inteiro dependia de uma estrutura `ForwardList` exposta globalmente, forçando dependências desnecessárias.
*   **D - Dependency Inversion Principle (DIP):** Módulos de alto nível (`BuscaService`, `MenuAdmin`) não dependem de módulos de baixo nível (leitura de arquivos), mas sim da abstração `IRepositorioLinhas` injetada via construtor. O `main` centraliza a resolução dessas dependências.

## 🛠️ Tecnologias Utilizadas
- **C++ (Standard Library)**
- Orientação a Objetos (Interfaces, Polimorfismo, Injeção de Dependência)
- Padrões de Projeto e Clean Code

## ⚙️ Como executar o projeto

Pré-requisitos: Compilador GCC/G++ instalado.

1. Clone o repositório:
```bash
git clone [https://github.com/SEU_USUARIO/BusTotem.git](https://github.com/SEU_USUARIO/BusTotem.git)

```
2. Acesse a pasta do projeto:
```Bash
cd BusTotem
```

2. Compile o código apontando para a pasta de dependências:
```Bash
g++ src/main.cpp -I include -o bustotem
```

3. Execute o sistema:
```Bash
./bustotem
```
👩‍💻 Autora
Ana Beatriz (Bia) | Estudante de Engenharia de Computação (UFC)
