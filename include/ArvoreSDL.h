// ArvoreSDL.h
#pragma once
#include <SDL.h>
#include <cmath>
#include "Objetos.h" // defina struct Arvore se ainda não existir
#include "Primitives.h"
#include "Cor.h" // parseColor(surface, Cor)

class ArvoreSDL
{
private:
    Arvore data;
    Tela tela;

public:
    ArvoreSDL(const Arvore &d, const Tela &t) : data(d), tela(t) {}

    void draw(SDL_Surface *surface)
    {
        SDL_Surface *tempSurface = SDL_CreateRGBSurface(
            0, surface->w, surface->h,
            surface->format->BitsPerPixel,
            surface->format->Rmask, surface->format->Gmask,
            surface->format->Bmask, surface->format->Amask);

        int nx = static_cast<int>(tela.largura / tela.mundoLargura);
        int ny = static_cast<int>(tela.altura / tela.mundoAltura);

        // Base (x,y) em pixels
        int x = static_cast<int>(data.x * nx);
        int y = tela.altura - static_cast<int>(data.y * ny);
        int largura = static_cast<int>(data.largura) * nx;
        int altura = -(static_cast<int>(data.altura) * ny);

        // ---------- TRONCO ----------
        int troncoW = std::max(1, static_cast<int>(std::round(largura * 0.40f)));
        int troncoH = static_cast<int>(std::round(altura * 0.60f)); // negativo
        int troncoX = x + (largura - troncoW) / 2;
        int troncoY = y;

        Uint32 corTronco = parseColor(tempSurface, data.corTronco);
        Uint32 corFolhas = parseColor(tempSurface, data.corFolhas);

        int x1 = troncoX + troncoW;
        int y1 = troncoY + troncoH;

        // arestas (topo, direita, base, esquerda)
        Primitives::drawLine(tempSurface, troncoX, troncoY, x1, troncoY, corTronco);
        Primitives::drawLine(tempSurface, x1, troncoY, x1, y1, corTronco);
        Primitives::drawLine(tempSurface, x1, y1, troncoX, y1, corTronco);
        Primitives::drawLine(tempSurface, troncoX, y1, troncoX, troncoY, corTronco);

        // Primitives::drawLine(tempSurface, troncoX, troncoY, troncoX + troncoW, troncoY + troncoH, corTronco);
        Primitives::floodFill(tempSurface, troncoX + 1, troncoY - 1, corTronco);

        // ---------- COPA ----------
        int copaH = static_cast<int>(std::round(altura * 0.40f)); // negativo
        if (copaH != 0)
        {
            // Caixa da copa
            const int copaW = std::max(2, static_cast<int>(std::round(largura * 0.95f)));
            const int copaX = x + (largura - copaW) / 2;
            const int copaY = troncoY + troncoH; // topo da copa (acima do tronco)

            // Pivôs principais (centro e raios)
            const int cx = copaX + copaW / 2;
            const int cy = copaY + copaH / 2; // lembrando: copaH é negativo
            const int rx = copaW / 2;
            const int ry = std::abs(copaH) / 2;

            // Pontos cardeais
            const int xt = cx;
            const int yt = cy - ry; // topo
            const int xr = cx + rx;
            const int yr = cy; // direita
            const int xb = cx;
            const int yb = cy + ry; // base
            const int xl = cx - rx;
            const int yl = cy; // esquerda

            // Curvas (controle nos “cantos” do retângulo da copa)
            // Topo -> Direita (controle no canto superior-direito)
            Primitives::drawBezier(tempSurface, xt, yt, cx + rx, cy - ry, xr, yr, corFolhas);
            // Direita -> Base (controle no canto inferior-direito)
            Primitives::drawBezier(tempSurface, xr, yr, cx + rx, cy + ry, xb, yb, corFolhas);
            // Base -> Esquerda (controle no canto inferior-esquerdo)
            Primitives::drawBezier(tempSurface, xb, yb, cx - rx, cy + ry, xl, yl, corFolhas);
            // Esquerda -> Topo (controle no canto superior-esquerdo)
            Primitives::drawBezier(tempSurface, xl, yl, cx - rx, cy - ry, xt, yt, corFolhas);

            // Opcional: “engrossar” o traço fazendo um pequeno inset (deixa a borda mais sólida)
            // Ajuste 'k' conforme desejar (1 ou 2 px).
            const int k = 1;
            if (rx > k && ry > k)
            {
                Primitives::drawBezier(tempSurface, xt, yt + k, cx + rx - k, cy - ry + k, xr - k, yr, corFolhas);
                Primitives::drawBezier(tempSurface, xr, yr, cx + rx - k, cy + ry - k, xb, yb - k, corFolhas);
                Primitives::drawBezier(tempSurface, xb, yb - k, cx - rx + k, cy + ry - k, xl + k, yl, corFolhas);
                Primitives::drawBezier(tempSurface, xl, yl, cx - rx + k, cy - ry + k, xt, yt + k, corFolhas);
            }
        }

        Primitives::copySurface(surface, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
};
