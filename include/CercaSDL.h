#pragma once
#include <SDL.h>
#include <cmath>
#include "Objetos.h"
#include "Primitives.h"
#include "Cor.h"

class CercaSDL
{
private:
    Cerca data;
    Tela  tela;

    static inline void rotacionaPonto(float cx, float cy, float& x, float& y, float angRad)
    {
        float s = std::sin(angRad);
        float c = std::cos(angRad);
        float tx = x - cx;
        float ty = y - cy;
        float rx =  tx * c - ty * s;
        float ry =  tx * s + ty * c;
        x = cx + rx;
        y = cy + ry;
    }

    static inline void linhaRot(SDL_Surface* surf, float x0, float y0, float x1, float y1, float cx, float cy, float angRad, Uint32 cor)
    {
        rotacionaPonto(cx, cy, x0, y0, angRad);
        rotacionaPonto(cx, cy, x1, y1, angRad);
        Primitives::drawLine(surf, (int)std::round(x0), (int)std::round(y0),
                                   (int)std::round(x1), (int)std::round(y1), cor);
    }

public:
    CercaSDL(const Cerca& d, const Tela& t) : data(d), tela(t) {}

    void draw(SDL_Surface* surface)
    {
        SDL_Surface *tempSurface = SDL_CreateRGBSurface(
            0, surface->w, surface->h,
            surface->format->BitsPerPixel,
            surface->format->Rmask, surface->format->Gmask,
            surface->format->Bmask, surface->format->Amask);

        
        int nx = static_cast<int>(tela.largura / tela.mundoLargura);
        int ny = static_cast<int>(tela.altura / tela.mundoAltura);

        // Converter as cores
        Uint32 cor = parseColor(tempSurface, data.cor);

        // Dimensões
        int x = static_cast<int>(data.x) * nx;
        int y = tela.altura - (static_cast<int>(data.y) * ny);
        int largura = static_cast<int>(data.largura) * nx;
        int altura = -(static_cast<int>(data.altura) * ny);

        int unidade = static_cast<int>(largura/10);
        
        int nro_fios = 8;
        int altura_fios = static_cast<int>(altura/nro_fios);

        // ===========================
        // 1) Desenha a parede (retângulo cheio via flood fill)
        // ===========================
        // contorno
        Primitives::drawLine(tempSurface, x, y, x + unidade, y, cor);
        Primitives::drawLine(tempSurface, x, y, x, y + altura, cor);
        Primitives::drawLine(tempSurface, x, y + altura, x + unidade, y + altura, cor);
        Primitives::drawLine(tempSurface, x + unidade, y, x + unidade, y + altura, cor);

        for (int i = 0; i <= nro_fios; ++i)
        {
            Primitives::drawLine(tempSurface, x + unidade, y + (altura_fios * i), largura - unidade, y + (altura_fios * i), cor);
        }

        Primitives::drawLine(tempSurface, largura, y, largura-unidade, y, cor);
        Primitives::drawLine(tempSurface, largura, y, largura, y+altura, cor);
        Primitives::drawLine(tempSurface, largura, y+altura, largura-unidade, y+altura, cor);
        Primitives::drawLine(tempSurface, largura-unidade, y+altura, largura-unidade, y, cor);

        Primitives::floodFill(tempSurface, x+(unidade/2), y+(altura / 2), cor);
        Primitives::floodFill(tempSurface, largura-(unidade/2), y+(altura / 2), cor);


        Mat3 rot = Mat3::rotationAround(data.inclinacao * (3.14/180), x +(largura/2), y+(altura/2));

        Primitives::blitTransformed(tempSurface,tempSurface,rot);

        Primitives::copySurface(surface, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
};
