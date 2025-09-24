#pragma once
#include <SDL.h>

struct Mat3 {
    float a, c, tx; // primeira linha
    float b, d, ty; // segunda linha

    // construtor identidade
    Mat3(float a=1, float c=0, float tx=0,
         float b=0, float d=1, float ty=0)
        : a(a), c(c), tx(tx), b(b), d(d), ty(ty) {}

    // aplica transformação em um ponto (x,y)
    void transformPoint(float x, float y, float &outX, float &outY) const {
        outX = a * x + c * y + tx;
        outY = b * x + d * y + ty;
    }

    // cria matriz de translação
    static Mat3 translation(float tx, float ty) {
        return Mat3(1, 0, tx,
                    0, 1, ty);
    }

    // cria matriz de escala
    static Mat3 scale(float sx, float sy) {
        return Mat3(sx, 0, 0,
                    0, sy, 0);
    }

    // cria matriz de rotação em torno da origem
    static Mat3 rotation(float angleRad) {
        float cosA = cos(angleRad);
        float sinA = sin(angleRad);
        return Mat3(cosA, -sinA, 0,
                    sinA,  cosA, 0);
    }

    static Mat3 rotationAround(float angleRad, float px, float py) {
        return Mat3::translation(px, py) * Mat3::rotation(angleRad) * Mat3::translation(-px, -py);
    }

    // multiplicação de matrizes (this * o)
    Mat3 operator*(const Mat3 &o) const {
        return Mat3(
            a*o.a + c*o.b,   a*o.c + c*o.d,   a*o.tx + c*o.ty + tx,
            b*o.a + d*o.b,   b*o.c + d*o.d,   b*o.tx + d*o.ty + ty
        );
    }
};

class Primitives
{
public:
    // Pixel "base" — usado por todos os outros algoritmos
    static void setPixel(SDL_Surface *surface, int x, int y, Uint32 color);

    // Função para pegar a cor de um pixel na superfície
    static Uint32 getPixel(SDL_Surface *surface, int x, int y);

    // Função para converter valores RGB em Uint32
    static Uint32 rgbToUint32(SDL_Surface *surface, int r, int g, int b);

    // Retas
    static void drawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color);
    static void drawLineAA(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 color);

    // Círculo
    static void drawCircle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 color);

    // Curvas de Bézier (exemplo: quadrática)
    static void drawBezier(SDL_Surface *surface, int x0, int y0, int x1, int y1, int x2, int y2, Uint32 color);

    // Flood fill (4-direções)
    static void floodFill(SDL_Surface *surface, int x, int y, Uint32 fillColor, Uint32 borderColor);

    // Flood fill
    static void floodFill(SDL_Surface *surface, int x, int y, Uint32 fillColor);

    // Converte uma cor RGBA para padrão SDL
    static SDL_Color uint32ToSDL_COLOR(SDL_Surface *surface, Uint32 color);

    static SDL_Surface *transformSurface(SDL_Surface *src, const Mat3 &m, int newW, int newH);

    static void blitTransformed(SDL_Surface* dest, SDL_Surface* src, const Mat3& m);

    static void copySurface(SDL_Surface* dest, SDL_Surface* src);
};