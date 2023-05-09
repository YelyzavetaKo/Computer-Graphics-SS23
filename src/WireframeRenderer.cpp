
#include "WireframeRenderer.hpp"

/**
** Zeichnet alle Dreiecke der Scene als Wireframe-Rendering unter Verwendung des
* Bresenham-Algorithmus
** Precondition: Sowohl mImage als auch mScene müssen gesetzt sein.
** (Aufgabenblatt 2 - Aufgabe 1)
**/
void WireframeRenderer::renderScene(Color color) {
}

/**
** Zeichnet unter Verwendung des Bresenham Algorithmus eine Linie zwischen p1
* und p2 (nutzt x & y Komponente - z Komponente wird ignoriert)
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 2)
**/
void WireframeRenderer::drawBresenhamLine(GLPoint p1, GLPoint p2, Color color) {
    if (!mImage) {
        throw std::runtime_error("mImage parameter not set");
    }
    int x0 = p1(0);
    int y0 = p1(1);
    int x1 = p2(0);
    int y1 = p2(1);
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int deltax = x1 - x0;
    int deltay = std::abs(y1 - y0);
    int error = deltax / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        if (steep) {
            mImage->setValue(y, x, color);
        } else {
            mImage->setValue(x, y, color);
        }

        error -= deltay;
        if (error < 0) {
            y += ystep;
            error += deltax;
        }
    }
}

/**
** Füllt einen vorgegebenen Bereich (abgegrenzt durch Randfarbe/borderColor) mit einer vorgegebenen Farbe (fillColor).
** Precondition: Das mImage muss gesetzt sein.
** (Aufgabenblatt 1 - Aufgabe 3) 
**/
void WireframeRenderer::seedFillArea(GLPoint seed, Color borderColor, Color fillColor) {
}
