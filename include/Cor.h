#pragma once
#include <string>
#include <SDL.h>
#include "Primitives.h"
#include <unordered_map>
#include <iostream>

struct Cor
{
    Uint8 r = 0;
    Uint8 g = 0;
    Uint8 b = 0;

    Cor() = default;
    Cor(Uint8 r, Uint8 g, Uint8 b) : r(r), g(g), b(b) {}

    SDL_Color toSDLColor() const
    {
        return SDL_Color{r, g, b, 255};
    }

    static Cor fromString(const std::string &nome)
    {
        static const std::unordered_map<std::string, Cor> tabela = {
            {"Azul", Cor(0, 0, 255)},
            {"Amarelo", Cor(255, 255, 0)},
            {"Amarela", Cor(255, 255, 0)},
            {"Vermelho", Cor(255, 0, 0)},
            {"Marrom", Cor(139, 69, 19)},
            {"Verde", Cor(0, 255, 0)},
            {"Lima", Cor(50, 205, 50)},
            {"Laranja", Cor(255, 165, 0)}};

        auto it = tabela.find(nome);
        if (it != tabela.end())
            return it->second;
        return Cor(255, 255, 255); // branco padrão
    }
};

// Função auxiliar simples para converter string "r,g,b" em Uint32
Uint32 parseColor(SDL_Surface *surface, const std::string &colorStr)
{
    Cor cor = Cor::fromString(colorStr);
    // Uint32 u = Primitives::rgbToUint32(surface, static_cast<int>(cor.r), static_cast<int>(cor.g), static_cast<int>(cor.b));

    return SDL_MapRGB(surface->format, cor.r, cor.g, cor.b);
}