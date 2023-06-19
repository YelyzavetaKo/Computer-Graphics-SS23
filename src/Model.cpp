
#include "Model.hpp"

// Konstruktor
Model::Model() {
  /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die default Werte */
  mScale = GLVector(1.0, 1.0, 1.0);
  mRotation = GLVector(0.0, 0.0, 0.0);
  mTranslation = GLVector(0.0, 0.0, 0.0);
}

// Setter für das Material
void Model::setMaterial(Material material) {
  mMaterial = Material();
  mMaterial.smooth = material.smooth;
  mMaterial.reflection = material.reflection;
  mMaterial.refraction = material.refraction;
  mMaterial.transparency = material.transparency;
  mMaterial.color = Color(material.color.r, material.color.g, material.color.b);
}

/* Aufgabenblatt 2, Aufgabe 3: Implementieren Sie die vier Methoden für die Transformationen hier */
void Model::setRotation(GLVector rotation) {
  mRotation = rotation;
  this->updateMatrix();
}

void Model::setTranslation(GLVector translation) {
  mTranslation = translation;
  this->updateMatrix();
}

void Model::setScale(GLVector scale) {
  mScale = scale;
  this->updateMatrix();
}

void Model::updateMatrix() {
  GLMatrix rotationMatrix = GLMatrix();
  
  GLMatrix rotationX = GLMatrix();
  rotationX.setValue(1, 1, std::cos(mRotation(0)));
  rotationX.setValue(1, 2, -std::sin(mRotation(0)));
  rotationX.setValue(2, 1, std::sin(mRotation(0)));
  rotationX.setValue(2, 2, std::cos(mRotation(0)));

  GLMatrix rotationY = GLMatrix();
  rotationY.setValue(0, 0, std::cos(mRotation(1)));
  rotationY.setValue(0, 2, std::sin(mRotation(1)));
  rotationY.setValue(2, 0, -std::sin(mRotation(1)));
  rotationY.setValue(2, 2, std::cos(mRotation(1)));

  GLMatrix rotationZ = GLMatrix();
  rotationZ.setValue(0, 0, std::cos(mRotation(2)));
  rotationZ.setValue(0, 1, -std::sin(mRotation(2)));
  rotationZ.setValue(1, 0, std::sin(mRotation(2)));
  rotationZ.setValue(1, 1, std::cos(mRotation(2)));

  rotationMatrix = rotationZ * rotationY * rotationX;

  GLMatrix translationMatrix = GLMatrix();
  translationMatrix.setValue(0, 3, mTranslation(0));
  translationMatrix.setValue(1, 3, mTranslation(1));
  translationMatrix.setValue(2, 3, mTranslation(2));

  GLMatrix scalingMatrix = GLMatrix();
  scalingMatrix.setValue(0, 0, mScale(0));
  scalingMatrix.setValue(1, 1, mScale(1));
  scalingMatrix.setValue(2, 2, mScale(2));

  mMatrix = translationMatrix * rotationMatrix * scalingMatrix;
}

GLMatrix Model::getTransformation() const { return mMatrix; }

Material Model::getMaterial() const { return mMaterial; }

std::vector<Triangle> Model::getTriangles() const { return mTriangles; }
