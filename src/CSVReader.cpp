#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

void lerCSV(const std::string &arquivo, Tela &tela, std::vector<std::shared_ptr<Objeto>> &objetos)
{
    std::ifstream file(arquivo);
    std::string linha;
    std::shared_ptr<Objeto> objetoAtual = nullptr;
    std::string tipoAtual;

    while (std::getline(file, linha))
    {
        if (linha.empty())
            continue;

        std::stringstream ss(linha);
        std::string campo;
        std::getline(ss, campo, ';');

        if (campo == "Tela")
        {
            tipoAtual = "Tela";
        }
        else if (campo == "Casa" || campo == "Arvore" || campo == "Cerca" || campo == "Sol")
        {
            if (objetoAtual)
                objetos.push_back(objetoAtual);

            tipoAtual = campo;
            if (campo == "Casa")
                objetoAtual = std::make_shared<Casa>();
            else if (campo == "Arvore")
                objetoAtual = std::make_shared<Arvore>();
            else if (campo == "Cerca")
                objetoAtual = std::make_shared<Cerca>();
            else if (campo == "Sol")
                objetoAtual = std::make_shared<Sol>();
        }
        else
        {
            std::string valor1, valor2;
            std::getline(ss, valor1, ';');
            std::getline(ss, valor2, ';');

            if (tipoAtual == "Tela")
            {
                if (campo == "Resolucao")
                {
                    tela.largura = std::stoi(valor1);
                    tela.altura = std::stoi(valor2);
                }
                else if (campo == "Metros")
                {
                    tela.mundoLargura = std::stof(valor1);
                    tela.mundoAltura = std::stof(valor2);
                }
                else if (campo == "Cor")
                {
                    tela.cor = valor1;
                }
            }
            else if (objetoAtual)
            {
                if (campo == "Localizacao" || campo == "Localização")
                {
                    objetoAtual->x = std::stof(valor1);
                    objetoAtual->y = std::stof(valor2);
                }
                else if (campo == "Altura")
                {
                    objetoAtual->altura = std::stof(valor1);
                }
                else if (campo == "Largura")
                {
                    objetoAtual->largura = std::stof(valor1);
                }
                else if (campo == "CorParede" && tipoAtual == "Casa")
                {
                    std::static_pointer_cast<Casa>(objetoAtual)->corParede = valor1;
                }
                else if (campo == "CorTelhado" && tipoAtual == "Casa")
                {
                    std::static_pointer_cast<Casa>(objetoAtual)->corTelhado = valor1;
                }
                else if (campo == "CorPorta" && tipoAtual == "Casa")
                {
                    std::static_pointer_cast<Casa>(objetoAtual)->corPorta = valor1;
                }
                else if (campo == "CorTronco" && tipoAtual == "Arvore")
                {
                    std::static_pointer_cast<Arvore>(objetoAtual)->corTronco = valor1;
                }
                else if (campo == "CorFolhas" && tipoAtual == "Arvore")
                {
                    std::static_pointer_cast<Arvore>(objetoAtual)->corFolhas = valor1;
                }
                else if (campo == "Cor" && tipoAtual == "Cerca")
                {
                    std::static_pointer_cast<Cerca>(objetoAtual)->cor = valor1;
                }
                else if (campo == "Inclinacao" && (tipoAtual == "Arvore" || tipoAtual == "Cerca"))
                {
                    if (tipoAtual == "Arvore")
                        std::static_pointer_cast<Arvore>(objetoAtual)->inclinacao = std::stof(valor1);
                    else
                        std::static_pointer_cast<Cerca>(objetoAtual)->inclinacao = std::stof(valor1);
                }
                else if (campo == "Cor" && tipoAtual == "Sol")
                {
                    std::static_pointer_cast<Sol>(objetoAtual)->cor = valor1;
                }
            }
        }
    }

    if (objetoAtual)
        objetos.push_back(objetoAtual);
}