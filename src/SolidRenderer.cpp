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
  Color color = Color(1.0, 1.0, 1.0);
  int antiAliasing = 1;
    for(size_t column = 0; column < mImage->getWidth(); ++column){
        Color finalColor = Color(0.0, 0.0, 0.0);

        for (int i = 0; i < antiAliasing; i++) {
          Ray ray = mCamera->getRay(column, rowNumber);
          HitRecord pxel = {.color = color,.parameter = INFINITY, .triangleId = -1, .modelId = -1, .sphereId = -1, };
          if (mScene->intersect(ray, pxel, EPSILON)) {
              this->shade(pxel);
              finalColor += pxel.color;
          }
          else {
            finalColor += color;
          }
        }
        finalColor /= antiAliasing;
        mImage->setValue(column, rowNumber, finalColor);
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
  double k_a = 0.4; // Coefficient for ambient light intensity
  double k_d = 0.4; // Coefficient for diffuse light intensity
  double k_s = 0.2; // Coefficient for specular light intensity
  int n = 20; // Roughness parameter

  // I = k_s * I_s + k_d * I_d + k_a * I_a
  // I_s = I_i * cos^n(omega) = I_i * (R ° V)^n   mit I_i Intesität der Lichtquelle i, V Betrachtungsrichtung, R Reflexionsrichtung, n Rauheit
  // I_d = I_i * cos(theta) = I_i * (L ° N)   mit I_i Intesität der Lichtquelle i, L Lichtvektor, N Punktnormale

  GLPoint lightPos = GLPoint(-100.0, 100.0, 100.0);
  double I_i = 1.0;
  double I_a = 1.0;

  GLVector N = GLVector(0.0, 0.0, 0.0);  
  if (r.modelId >= 0) {
    r.color = mScene->getModels()[r.modelId].getMaterial().color;
    N = (r.alpha * mScene->getModels()[r.modelId].mTriangles[r.triangleId].vertex[0].getNormal()) + (r.beta * mScene->getModels()[r.modelId].mTriangles[r.triangleId].vertex[1].getNormal()) + (r.gamma * mScene->getModels()[r.modelId].mTriangles[r.triangleId].vertex[2].getNormal());
    // std::cout << "pNormal " << N << "\tnorm " << N.norm() << std::endl; 
  }
  else if (r.sphereId >= 0) {
    r.color = mScene->getSpheres()[r.sphereId].getMaterial().color;
    N = r.intersectionPoint - mScene->getSpheres()[r.sphereId].getPosition();
  }
  N.normalize();
  GLVector L = lightPos - r.intersectionPoint;
  L.normalize();
  GLVector R = 2 * dotProduct(L, N) * N - L;
  R.normalize();
  GLVector V = mCamera->getEyePoint() - r.intersectionPoint;
  V.normalize();

  double I = k_s * (I_i * std::pow(dotProduct(R, V), n)) + k_d * (I_i * dotProduct(L, N)) + k_a * I_a;
  r.color *= I;

  // Schattierung
  Ray shadowRay = Ray();
  shadowRay.origin = r.intersectionPoint + EPSILON * r.rayDirection;
  shadowRay.direction = lightPos - shadowRay.origin;
  shadowRay.direction.normalize();

  HitRecord shadowHR = {.color = r.color,.parameter = INFINITY, .triangleId = -1, .modelId = -1, .sphereId = -1, };
  if (mScene->intersect(shadowRay, shadowHR, EPSILON)) {
    if (shadowHR.parameter >= 0 && shadowHR.parameter < (lightPos - shadowRay.origin).norm()) {
      r.color *= 0.5;
    }
  }

  if ((r.modelId >= 0 && mScene->getModels()[r.modelId].getMaterial().reflection > 0.0) || (r.sphereId >= 0 && mScene->getSpheres()[r.sphereId].getMaterial().reflection > 0.0)) {
    Ray reflectionRay = Ray();
    reflectionRay.origin = r.intersectionPoint + EPSILON * r.rayDirection;
    GLVector viewDirection = GLVector(-r.rayDirection(0), -r.rayDirection(1), -r.rayDirection(2)); // r.rayDirection;
    viewDirection.normalize();
    reflectionRay.direction = 2 * dotProduct(viewDirection, N) * N - viewDirection;
    reflectionRay.direction.normalize();

    // globalen HitRecord aktualisieren
    r.triangleId = -1;
    r.modelId = -1;
    r.sphereId = -1;
    r.parameter = INFINITY;
    r.recursions++;

    int maxRecursionDepth = 2;
    if (r.recursions <= maxRecursionDepth) {
      if (mScene->intersect(reflectionRay, r, EPSILON)) {
        this->shade(r);
      }
    } 
  }
}