#include "SolidRenderer.hpp"

//#include <tbb/tbb.h>  // Include, nur wenn TBB genutzt werden soll

#define EPSILON \
  (1e-12)  // Epsilon um ungenauigkeiten und Rundungsfehler zu kompensieren

/**
 ** Erstellt mittels Raycast das Rendering der mScene in das mImage
 ** Precondition: Sowohl mImage, mScene, mCamera müssen gesetzt sein.
 **/
void SolidRenderer::renderRaycast() {
    if (!mImage) {
        throw std::runtime_error("mImage parameter not set");
    }
    if (!mScene) {
        throw std::runtime_error("mScene parameter not set");
    }
    if (!mCamera) {
        throw std::runtime_error("mCamera parameter not set");
    }
    // This function is part of the base

    std::cout << "Rendern mittels Raycast gestartet." << std::endl;
    // Berechnung der einzelnen Rows in eigener Methode um die
    // Parallelisierbarkeit zu verbessern

    // Ohne parallelisierung:
    for(size_t i = 0; i < mImage->getHeight(); ++i ){
        computeImageRow( i );
    }

    //  Parallelisierung mit OpenMP:

    //#pragma omp parallel for
    //    for(size_t i = 0; i < mImage->getHeight(); ++i )
    //    {
    //        computeImageRow( i );
    //    }

}

void SolidRenderer::computeImageRow(size_t rowNumber) {
    for(size_t column = 0; column < mImage->getWidth(); ++column){
        Ray ray = mCamera->getRay(column, rowNumber);
        Color color = Color(1.0, 1.0, 1.0);
        HitRecord pxel = {.color = color,.parameter = INFINITY, .triangleId = -1, .modelId = -1, .sphereId = -1, };
        const float epsilon = 0.0001;
        if (mScene->intersect(ray, pxel,epsilon)) {
            mImage->setValue(column, rowNumber, pxel.color);
        }else {
            mImage->setValue(column, rowNumber, Color(1.0, 1.0, 1.0));
        }
    }
}
/**
 * Aufgabenblatt 3: Hier wird das Raycasting implementiert. Siehe Aufgabenstellung!
 * Precondition: Sowohl mImage, mScene und mCamera  müssen gesetzt sein.
 */
/**void SolidRenderer::computeImageRow(size_t rowNumber) {
 float alpha = 0.0;
 float beta = 0.5;
 float gamma = 0.8;
 float* alphaptr = &alpha;
 float* betaptr = &beta;
 float* gammaptr = &gamma;
 Color testColor = Color(0.8, 0.5, 0.3);
 for (size_t column = 0; column < mImage->getWidth(); ++column) {
   float tMin = -1.0;
   for (Model model : mScene->getModels()) {
     for (Triangle tri : model.mTriangles) {
       // See http://www.r-5.org/files/books/computers/algo-list/realtime-3d/Christer_Ericson-Real-Time_Collision_Detection-EN.pdf page 190
       // Berechne Schnittpunkt und wenn zSchnittpunkt > zMax färbe Pixel in Farbe des Dreiecks ein
       // Brechne dafür zuerst Schnittpunkt mit der Dreiecksebene und validiere dann mittels Baryzentrischer Koordinaten, dass der Schnittpunkt im Dreieck liegt
       Ray ray = mCamera->getRay(rowNumber, column);
       // t = (p - e) ° n  /  v ° n
       GLVector normal = crossProduct(GLVector(tri.vertex[1](0) - tri.vertex[0](0), tri.vertex[1](1) - tri.vertex[0](1), tri.vertex[1](2) - tri.vertex[0](2)), GLVector(tri.vertex[2](0) - tri.vertex[0](0), tri.vertex[2](1) - tri.vertex[0](1), tri.vertex[2](2) - tri.vertex[0](2)));
       GLVector difference = GLVector(tri.vertex[0](0) - ray.origin(0), tri.vertex[0](1) - ray.origin(1), tri.vertex[0](2) - ray.origin(2));
       float t = dotProduct(difference, normal) / dotProduct(ray.direction, normal);
       if (tMin == -1) {
         tMin = t;
       }
       GLPoint intersectionPoint = ray.origin + (t * ray.direction);

       // alpha = A(P, B, C) / A(A, B, C)
       // A(P, B, C) = ((B - P) x (C - P)) ° (((B - A) x (C - A))/||(B - A) x (C - A)||)
       barycentricCoordinates(intersectionPoint, tri.vertex[0], tri.vertex[1], tri.vertex[2], *alphaptr, *betaptr, *gammaptr);
       if (0 <= alpha && alpha <= 1 && 0 <= beta && beta <= 1 && 0 <= gamma && gamma <= 1) {
         if (t <= tMin) {
           tMin = t;
           mImage->setValue(rowNumber, column, testColor);
           std::cout << *alphaptr << " " << *betaptr << " " << *gammaptr << std::endl;
         }
       }
     }
   }
 }
}
/**
/**
*  Aufgabenblatt 4: Hier wird das raytracing implementiert. Siehe Aufgabenstellung!
*/
void SolidRenderer::shade(HitRecord &r) {
}