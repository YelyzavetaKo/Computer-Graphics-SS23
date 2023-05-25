
#include "WireframeRenderer.hpp"
#include <queue>
#include <iostream>

/**
** Zeichnet alle Dreiecke der Scene als Wireframe-Rendering unter Verwendung des
* Bresenham-Algorithmus
** Precondition: Sowohl mImage als auch mScene müssen gesetzt sein.
** (Aufgabenblatt 2 - Aufgabe 1)
**/
void WireframeRenderer::renderScene(Color color) {
    if (!mImage) {
        throw std::runtime_error("mImage parameter not set");
    }
    if (!mScene) {
        throw std::runtime_error("mScene parameter not set");
    }
    for (Model model : mScene->getModels()) {
        std::cout << model.getTransformation();
        for (Triangle tri : model.mTriangles) {
            this->drawBresenhamLine(model.getTransformation()*tri.vertex[0], model.getTransformation()*tri.vertex[1], color);
            this->drawBresenhamLine(model.getTransformation()*tri.vertex[1], model.getTransformation()*tri.vertex[2], color);
            this->drawBresenhamLine(model.getTransformation()*tri.vertex[2], model.getTransformation()*tri.vertex[0], color);
            /*
            std::cout << tri.vertex[0] << " " << tri.vertex[1] << "\t" << model.getTransformation()*tri.vertex[0] << " " << model.getTransformation()*tri.vertex[1] << std::endl;
            */
        }
    }
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
    if (!mImage) {
        throw std::runtime_error("mImage parameter not set");
    }

    // Generate Queue to track visited fields and add seed point
    std::queue<GLPoint> points;
    points.push(seed);

    while (!points.empty()) {
        GLPoint current = points.front();
        int x = current(0);
        int y = current(1);
        points.pop();
        if (x >= 0 && y >= 0 && x <= mImage->getWidth() && y <= mImage->getHeight()) {
            Color currentColor = mImage->getValue(x, y);
            // Check if Pixel is not Border Pixel
            if (!((currentColor.r == borderColor.r) && (currentColor.g == borderColor.g) && (currentColor.b == borderColor.b))) {

                // Check if Pixel was not filled already
                if (!((currentColor.r == fillColor.r) && (currentColor.g == fillColor.g) && (currentColor.b == fillColor.b))) {
                    mImage->setValue(x, y, fillColor);

                    GLPoint right(x + 1.0, y, 0.0);
                    GLPoint up(x, y + 1.0, 0.0);
                    GLPoint left(x - 1.0, y, 0.0);
                    GLPoint down(x, y - 1.0, 0.0);

                    GLPoint neighbours[4] = {right, up, left, down};

                    for (GLPoint point : neighbours) {
                        if (point(0) >= 0 && point(1) >= 0 && point(0) < mImage->getWidth() && point(1) < mImage->getHeight()) {
                            points.push(point);
                        }
                    }
                    /*
                    points.push(right);
                    points.push(up);
                    points.push(left);
                    points.push(down);
                    */
                }
            }
        }
    }
}

