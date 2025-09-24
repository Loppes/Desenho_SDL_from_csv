#pragma once
#include <string>

struct Tela
{
    int largura;        // pixels
    int altura;         // pixels
    float mundoLargura; // metros
    float mundoAltura;  // metros
    std::string cor;
};

struct Objeto
{
    float x, y;
    float altura, largura;
    virtual ~Objeto() {}
};

struct Casa : Objeto
{
    std::string corParede, corTelhado, corPorta;
};

struct Arvore : Objeto
{
    std::string corTronco, corFolhas;
    float inclinacao = 0;
};

struct Cerca : Objeto
{
    std::string cor;
    float inclinacao = 0;
};

struct Sol : Objeto
{
    std::string cor;
};