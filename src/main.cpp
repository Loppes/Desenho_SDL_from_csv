#include <SDL.h>
#include "Objetos.h"
#include "Cor.h"
#include "CSVReader.h"
#include "Primitives.h"
#include "CasaSDL.h"
#include "SolSDL.h"
#include "CercaSDL.h"
#include "ArvoreSDL.h"

#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>

int main()
{
    // Configuracoes Globais
    Tela tela;
    // Objetos a serem desenhados
    std::vector<std::shared_ptr<Objeto>> objetos;

    // Ler e popula a Tela e os Objetos
    lerCSV("mapa.csv", tela, objetos);

    // Info da tela
    std::cout << "Tela:\n"
              << "  Largura: " << tela.largura << "\n"
              << "  Altura: " << tela.altura << "\n"
              << "  Mundo Largura: " << tela.mundoLargura << "\n"
              << "  Mundo Altura: " << tela.mundoAltura << "\n"
              << "  Cor: " << tela.cor << "\n";

    // Info dos objetos
    for (size_t i = 0; i < objetos.size(); ++i)
    {
        std::cout << "Objeto " << i << ": tipo = " << typeid(*objetos[i]).name()
                  << ", x = " << objetos[i]->x
                  << ", y = " << objetos[i]->y
                  << ", largura = " << objetos[i]->largura
                  << ", altura = " << objetos[i]->altura;

        if (auto casa = std::dynamic_pointer_cast<Casa>(objetos[i]))
        {
            std::cout << ", corParede = " << casa->corParede
                      << ", corTelhado = " << casa->corTelhado
                      << ", corPorta = " << casa->corPorta;
        }
        else if (auto arvore = std::dynamic_pointer_cast<Arvore>(objetos[i]))
        {
            std::cout << ", corTronco = " << arvore->corTronco
                      << ", corFolhas = " << arvore->corFolhas
                      << ", inclinacao = " << arvore->inclinacao;
        }
        else if (auto cerca = std::dynamic_pointer_cast<Cerca>(objetos[i]))
        {
            std::cout << ", cor = " << cerca->cor
                      << ", inclinacao = " << cerca->inclinacao;
        }
        else if (auto sol = std::dynamic_pointer_cast<Sol>(objetos[i]))
        {
            std::cout << ", cor = " << sol->cor;
        }
        std::cout << std::endl;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "error" << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Mundo SDL",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          tela.largura, tela.altura, SDL_WINDOW_SHOWN);

    Cor corFundo = Cor::fromString(tela.cor);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, tela.largura, tela.altura, // height
                                                    32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

    SDL_FillRect(tempSurface, NULL, SDL_MapRGB(tempSurface->format, 255, 255, 255));
    SDL_FillRect(tempSurface, nullptr, SDL_MapRGB(tempSurface->format, corFundo.r, corFundo.g, corFundo.b));

    std::cout << "Janela criada. Feche para sair..." << std::endl;

    for (size_t i = 0; i < objetos.size(); i++)
    {
        if (auto casa = std::dynamic_pointer_cast<Casa>(objetos[i]))
        {
            CasaSDL casaSDL(*casa, tela);
            casaSDL.draw(tempSurface);
        }
        if (auto sol = std::dynamic_pointer_cast<Sol>(objetos[i]))
        {
            SolSDL solSDL(*sol, tela);
            solSDL.draw(tempSurface);
        }
        if (auto cerca = std::dynamic_pointer_cast<Cerca>(objetos[i]))
        {
            CercaSDL cercaSDL(*cerca, tela);
            cercaSDL.draw(tempSurface);
        }
        if (auto arvore = std::dynamic_pointer_cast<Arvore>(objetos[i]))
        {
            ArvoreSDL arvoreSDL(*arvore, tela);
            arvoreSDL.draw(tempSurface);
        }
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    SDL_FreeSurface(tempSurface);

    SDL_UpdateWindowSurface(window);
    bool rodando = true;
    SDL_Event event;
    while (rodando)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                rodando = false;
        }
        // Limpar renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Copiar textura para o renderer
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Apresentar na tela
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}