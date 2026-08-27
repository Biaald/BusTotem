#pragma once
#include <string>
#include <sstream>

// SRP - Responsabilidade Única: TimeUtils tem uma unica responsabilidade — operacoes sobre horarios.
// Antes, convertHourToNum(), converterHorarioParaMinutos() e formatar()
// estavam duplicadas em DUAS classes (DoubleList e ForwardList), violando
// tambem o principio DRY (Don't Repeat Yourself).
class TimeUtils {
public:
    // Converte "HH:MM" para numero decimal (ex: "13:30" -> 13.5)
    static double paraDecimal(const std::string& horarioRaw) {
        std::string h = horarioRaw;
        while (!h.empty() && h[0] == ' ') h.erase(0, 1);
        h = h.substr(0, 5);
        if (h.size() < 5) return -1;
        return std::stoi(h.substr(0, 2)) + std::stoi(h.substr(3, 2)) / 60.0;
    }

    // Converte "HH:MM" para minutos totais
    static int paraMinutos(const std::string& horario) {
        int horas = 0, minutos = 0;
        std::istringstream ss(horario);
        char sep;
        ss >> horas >> sep >> minutos;
        return horas * 60 + minutos;
    }

    // Calcula duracao entre saida e chegada em minutos
    static int duracaoMinutos(const std::string& saida, const std::string& chegada) {
        return paraMinutos(chegada) - paraMinutos(saida);
    }

    // Formata minutos para "Xh Ymin"
    static std::string formatar(int minutos) {
        std::stringstream ss;
        ss << minutos / 60 << "h" << minutos % 60 << "min";
        return ss.str();
    }
};
