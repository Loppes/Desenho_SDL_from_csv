#pragma once
#include <SDL.h>
#include "Objetos.h"
#include <string>
#include "Primitives.h"
#include "Cor.h"
#include <iostream>
#include <cmath>

class CasaSDL
{
private:
    Casa data;
    Tela tela;

public:
    CasaSDL(const Casa &d, const Tela &t) : data(d), tela(t) {}

    void draw(SDL_Surface *surface)
    {
        SDL_Surface *tempSurface = SDL_CreateRGBSurface(
            0, surface->w, surface->h,
            surface->format->BitsPerPixel,
            surface->format->Rmask, surface->format->Gmask,
            surface->format->Bmask, surface->format->Amask);

        
        int nx = static_cast<int>(tela.largura / tela.mundoLargura);
        int ny = static_cast<int>(tela.altura / tela.mundoAltura);

        // Converter as cores
        Uint32 corParede = parseColor(tempSurface, data.corParede);
        Uint32 corTelhado = parseColor(tempSurface, data.corTelhado);
        Uint32 corPorta = parseColor(tempSurface, data.corPorta);

        // Dimensões
        int x = static_cast<int>(data.x) * nx;
        int y = tela.altura - (static_cast<int>(data.y) * ny);
        int largura = static_cast<int>(data.largura) * nx;
        int altura = -(static_cast<int>(data.altura) * ny);

        // ===========================
        // 1) Desenha a parede (retângulo cheio via flood fill)
        // ===========================
        // contorno
        Primitives::drawLine(tempSurface, x, y, x + largura, y, corParede);
        Primitives::drawLine(tempSurface, x, y, x, y + altura, corParede);
        Primitives::drawLine(tempSurface, x + largura, y, x + largura, y + altura, corParede);
        Primitives::drawLine(tempSurface, x, y + altura, x + largura, y + altura, corParede);

        // flood fill no centro
        Primitives::floodFill(tempSurface, x + largura / 2, y + altura / 2, corParede);

        // ===========================
        // 2) Desenha o telhado (triângulo)
        // ===========================
        int telhadoA_x = x;
        int telhadoA_y = y + altura;
        int telhadoB_x = x + largura;
        int telhadoB_y = y + altura;
        int telhadoC_x = x + largura / 2;
        int telhadoC_y = y + altura + (altura / 2);

        Primitives::drawLine(tempSurface, telhadoA_x, telhadoA_y, telhadoB_x, telhadoB_y, corTelhado);
        Primitives::drawLine(tempSurface, telhadoB_x, telhadoB_y, telhadoC_x, telhadoC_y, corTelhado);
        Primitives::drawLine(tempSurface, telhadoC_x, telhadoC_y, telhadoA_x, telhadoA_y, corTelhado);

        // // flood fill dentro
        Primitives::floodFill(tempSurface, x + (largura / 2), y +(altura/4)+ altura, corTelhado);

        // // ===========================
        // // 3) Desenha a porta (retângulo menor no meio da base)
        // // ===========================
        int portaLarg = largura / 4;
        int portaAlt = altura / 2;
        int portaX = x + (largura / 2 - portaLarg / 2);
        int portaY = y;

        Primitives::drawLine(tempSurface, portaX, portaY, portaX + portaLarg, portaY, corPorta);
        Primitives::drawLine(tempSurface, portaX, portaY, portaX, portaY + portaAlt, corPorta);
        Primitives::drawLine(tempSurface, portaX + portaLarg, portaY, portaX + portaLarg, portaY + portaAlt, corPorta);
        Primitives::drawLine(tempSurface, portaX, portaY + portaAlt, portaX + portaLarg, portaY + portaAlt, corPorta);

        Primitives::floodFill(tempSurface, portaX + portaLarg / 2, portaY + portaAlt / 2, corPorta);

        Primitives::copySurface(surface, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
};