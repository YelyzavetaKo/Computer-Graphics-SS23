#include "Scene.hpp"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

Scene::Scene() {}


/**
 * Gibt zurück ob ein gegebener Strahl ein Objekt (Modell oder Kugel) der Szene trifft
 *  (Aufgabenblatt 3)
 */
bool Scene::intersect(const Ray &ray, HitRecord &hitRecord,
                      const float epsilon) {
    bool hit = false;
    for (int s = 0; s < this->getSpheres().size(); s++) {
      if (sphereIntersect(ray, this->getSpheres()[s], hitRecord, epsilon)) {
        hit = true;
        hitRecord.sphereId = s;
        // hitRecord.color = this->getSpheres()[s].getMaterial().color;
      }
    }
    for (int m = 0; m < this->getModels().size(); m++) {
        for(int t = 0; t < this->getModels()[m].mTriangles.size(); t++) {
            Triangle transformedTriangle;
            for (size_t i = 0; i < this->getModels()[m].mTriangles[t].vertex.size(); i++) {
                transformedTriangle.vertex[i] = this->getModels()[m].getTransformation() * this->getModels()[m].mTriangles[t].vertex[i];
            }
            if (triangleIntersect(ray, transformedTriangle, hitRecord, epsilon)) {
                // std::cout << m;
                hit = true;
                hitRecord.modelId = m;
                hitRecord.triangleId = t;
                // hitRecord.color = this->getModels()[m].getMaterial().color;
            }
        }
    }
    return hit;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl ein Dreieck  eines Modells der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jedes Objektdreieck aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
 */
bool Scene::triangleIntersect(const Ray &ray, const Triangle &triangle,
                              HitRecord &hitRecord, const float epsilon) {
    //vertex[1] = b, vertex[2] = c, vertex[0] = a

    GLVector ab = triangle.vertex[1]- triangle.vertex[0];
    GLVector ac = triangle.vertex[2]- triangle.vertex[0];
    GLVector qp = GLVector(-ray.direction(0), -ray.direction(1), -ray.direction(2));
    GLVector normal = crossProduct(ab, ac);
    double denominator = dotProduct(qp, normal);
    if (denominator <= 0.0) return false;
    GLVector ap = ray.origin - triangle.vertex[0];
    double HRparameter = dotProduct(ap, normal);
    if (HRparameter < 0.0) return false;
    GLVector e = crossProduct(qp, ap);
    double v = dotProduct(ac, e);
    if (v < 0.0 || v > denominator) return false;
    double w = -dotProduct(ab, e);
    if (w < 0.0 || (v + w) > denominator) return false;
    double ood = 1.0 / denominator;
    HRparameter *= ood;
    if (hitRecord.parameter > HRparameter) {
    hitRecord.rayDirection = ray.direction;
    hitRecord.normal = normal;
    hitRecord.intersectionPoint = ray.origin + ray.direction * HRparameter;
    hitRecord.parameter = HRparameter;
    hitRecord.rayDirection = ray.direction;
    hitRecord.beta = v * ood;
    hitRecord.gamma = w * ood;
    hitRecord.alpha = 1.0 - hitRecord.beta - hitRecord.gamma;
    // std::cout << "alpha " << hitRecord.alpha << "\tbeta " << hitRecord.beta << "\tgamma " << hitRecord.gamma << std::endl;
    return true;
    }
    return false;
}

/** Aufgabenblatt 3: Gibt zurück ob ein gegebener Strahl eine Kugel der Szene trifft
 *  Diese Methode sollte in Scene::intersect für jede Kugel aufgerufen werden
 *  Aufgabenblatt 4: Diese Methode befüllt den den HitRecord im Fall eines Treffers mit allen für das shading notwendigen informationen
*/
bool Scene::sphereIntersect(const Ray &ray, const Sphere &sphere,
                            HitRecord &hitRecord, const float epsilon) {
    double a = dotProduct(ray.direction, ray.direction);
    double b = 2 * dotProduct(ray.direction, ray.origin - sphere.getPosition());
    double c = dotProduct(ray.origin - sphere.getPosition(), ray.origin - sphere.getPosition()) - (sphere.getRadius() * sphere.getRadius());

    // Calculate the discriminant
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
      return false;
    }
    double parameter;
    if (discriminant > 0) {
        // Two real and distinct roots
        // double root1 = (-b + std::sqrt(discriminant)) / (2 * a);
        parameter = (-b - std::sqrt(discriminant)) / (2 * a);
    } else if (discriminant == 0) {
        // One real root (repeated)
        parameter = -b / (2 * a);
    }
    if (parameter <= hitRecord.parameter) {
      hitRecord.parameter = parameter;
      hitRecord.intersectionPoint = ray.origin + ray.direction * parameter;
      hitRecord.rayDirection = ray.direction;
      hitRecord.normal = hitRecord.intersectionPoint - sphere.getPosition();
      hitRecord.normal.normalize();
      return true;
    }
    return false;
}

/**
** Liest die Modelle (deren Dateinamen in pFiles stehen) ein und speichert sie
*in mModels
**/
void Scene::load(const std::vector<std::string> &pFiles) {
  std::cout << "Laden der PLY Dateien:" << std::endl;
  // Für alle Objekte (Objekt meint hier das selbe wie Model)
  for (int obj_nr = 0; obj_nr < pFiles.size(); obj_nr++) {
    std::cout << "\tModel-Index: " << obj_nr << std::endl;
    // Assimp übernimmt das Einlesen der ply-Dateien
    Assimp::Importer importer;
    const aiScene *assimpScene = importer.ReadFile(
        pFiles[obj_nr], aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                            aiProcess_JoinIdenticalVertices |
                            aiProcess_SortByPType);
    
      if( !assimpScene)
        {
          std::cout << importer.GetErrorString() << std::endl;
        }
      assert(assimpScene);
    auto meshes = assimpScene->mMeshes;
    // Neues Model erstellen
    Model model = Model();

    // Für alle Meshes des Models
    for (int i = 0; i < assimpScene->mNumMeshes; i++) {
      std::cout << "\t\tMesh-Index: " << i << " (" << meshes[i]->mNumFaces
                << " Faces)" << std::endl;
      auto faces = meshes[i]->mFaces;
      auto vertices = meshes[i]->mVertices;
      auto normals = meshes[i]->mNormals;

      // Für alle Faces einzelner Meshes
      for (int j = 0; j < meshes[i]->mNumFaces; j++) {
        // Dreieck konstruieren und nötige Werte berechnen
        Triangle tri;
        assert(faces[j].mNumIndices == 3);
        tri.vertex[0] = GLPoint(vertices[faces[j].mIndices[0]].x,
                                vertices[faces[j].mIndices[0]].y,
                                vertices[faces[j].mIndices[0]].z);
        tri.vertex[0].setNormal(GLVector(normals[faces[j].mIndices[0]][0], normals[faces[j].mIndices[0]][1], normals[faces[j].mIndices[0]][2]));
        tri.vertex[1] = GLPoint(vertices[faces[j].mIndices[1]].x,
                                vertices[faces[j].mIndices[1]].y,
                                vertices[faces[j].mIndices[1]].z);
        tri.vertex[1].setNormal(GLVector(normals[faces[j].mIndices[1]][0], normals[faces[j].mIndices[1]][1], normals[faces[j].mIndices[1]][2]));
        tri.vertex[2] = GLPoint(vertices[faces[j].mIndices[2]].x,
                                vertices[faces[j].mIndices[2]].y,
                                vertices[faces[j].mIndices[2]].z);
        tri.vertex[2].setNormal(GLVector(normals[faces[j].mIndices[2]][0], normals[faces[j].mIndices[2]][1], normals[faces[j].mIndices[2]][2]));
        // std::cout << "normal " << tri.vertex[2].getNormal() << std::endl;
        GLVector normal = crossProduct(tri.vertex[1] - tri.vertex[0],
                                       tri.vertex[2] - tri.vertex[0]);
        normal.normalize();
        tri.normal = normal;
        // Jedes Dreieck zum Vector der Dreiecke des aktuellen Models hinzufügen
        model.mTriangles.push_back(tri);
      }
    }
    // Immer das gleiche Material für das Model setzen
    Material material;
    material.color = Color(0.00, 1.00, 0.00);
    model.setMaterial(material);
    // Jedes Model zum Vector der Models der Scene hinzufügen
    mModels.push_back(model);
  }

  std::cout << "Laden der PLY Dateien abgeschlossen." << std::endl;
}

void Scene::setCamera(std::shared_ptr<Camera> cam) { mCamera = cam; }

std::shared_ptr<Camera> Scene::getCamera() const { return mCamera; }

GLPoint Scene::getViewPoint() const {
  if (mCamera)
    return mCamera->getEyePoint();
  else {
    std::cerr << "No Camera set to get view point from." << std::endl;
    return GLPoint(0, 0, 0);
  }
}



void Scene::addPointLight(GLPoint pointLight) {
  mPointLights.push_back(pointLight);
}

void Scene::addModel(Model model) { mModels.push_back(model); }

void Scene::addSphere(Sphere sphere) { mSpheres.push_back(sphere); }

std::vector<Model> &Scene::getModels() { return mModels; }

std::vector<Sphere> &Scene::getSpheres() { return mSpheres; }

std::vector<GLPoint> &Scene::getPointLights() { return mPointLights; }
