#pragma once
#include <SDL.h>
#include <cmath>
#include "Objetos.h"
#include "Primitives.h"
#include "Cor.h"

class SolSDL
{
private:
    Sol  data;
    Tela tela;

public:
    SolSDL(const Sol& d, const Tela& t) : data(d), tela(t) {}

    void draw(SDL_Surface* surface)
    {
        SDL_Surface* tempSurface = SDL_CreateRGBSurface(
            0, surface->w, surface->h,
            surface->format->BitsPerPixel,
            surface->format->Rmask, surface->format->Gmask,
            surface->format->Bmask, surface->format->Amask);

        // escala mundo -> tela
        const int nx = static_cast<int>(tela.largura / tela.mundoLargura);
        const int ny = static_cast<int>(tela.altura  / tela.mundoAltura);

        // retângulo do sol em pixels (mesma convenção da CasaSDL: Y invertido)
        const int x = static_cast<int>(data.x) * nx;
        const int y = tela.altura - (static_cast<int>(data.y) * ny);
        const int largura = static_cast<int>(data.largura) * nx;
        const int altura = -(static_cast<int>(data.altura) * ny); // negativo para "subir"

        // centro e raio do disco
        const int centroX = x + largura / 2;
        const int centroY = y + altura / 2;
        const int r  = static_cast<int>(0.5f * std::min(std::abs(largura), std::abs(altura)));

        // cor
        Uint32 cor = parseColor(tempSurface, data.cor);

        Primitives::drawCircle(tempSurface, centroX, centroY, r, cor);

        // preenchimento do disco
        Primitives::floodFill(tempSurface, centroX, centroY, cor);

        // copia pra surface final
        Primitives::copySurface(surface, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
};
