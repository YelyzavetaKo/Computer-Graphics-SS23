#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <unistd.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <random>
#include "Color.hpp"
#include "Scene.hpp"
#include "SolidRenderer.hpp"
#include "WireframeRenderer.hpp"
#include "math.hpp"


int main(int argc, char **argv) {


    // Dimensionen des Ergebnisbildes im Konstruktor setzen
    std::shared_ptr <Image> img = std::make_shared<Image>(400, 300);

    // Verwendete Modelle festlegen
    std::vector <std::string> path_vector;
    path_vector.push_back(std::string("../data/bunny/bunny_scaled.ply"));
    path_vector.push_back(std::string("../data/basicObjects/cube_scaled.ply"));
    path_vector.push_back(std::string("../data/basicObjects/cube_scaled.ply"));
    path_vector.push_back(std::string("../data/basicObjects/cube_scaled.ply"));
    path_vector.push_back(std::string("../data/basicObjects/cube_scaled.ply"));
    path_vector.push_back(std::string("../data/basicObjects/cube_scaled.ply"));
    // Erzeuge die Szene mit dem default Konstruktor und lade die Modelle
    auto scene = std::make_shared<Scene>();
    scene->load(path_vector);


    //Aufgabenblatt 1: Instanziieren Sie einen WireframeRenderer */
    //auto renderer = std::make_shared<WireframeRenderer>(scene, img);
    /* Aufgabenblatt 1, Aufgabe 2: Testen Sie Ihre drawBresenhamLine-Methode hier */
    // Draw a line using the Bresenham algorithm
    // Save the image to a file
    /*
    GLPoint p1(100.0, 50.0, 0.0);
    GLPoint p2(200.0, 150.0, 0.0);
    Color color(1.0, 0.0, 1.0);
    renderer->drawBresenhamLine(p1, p2, color);
    img->writeAsPPM("wireframe.ppm");

    GLPoint bl(20.0, 10.0, 0.0);
    GLPoint br(10.0, 10.0, 0.0);
    GLPoint tl(20.0, 20.0, 0.0);
    GLPoint tr(10.0, 20.0, 0.0);

    renderer->drawBresenhamLine(bl, br, color);
    renderer->drawBresenhamLine(bl, tl, color);
    renderer->drawBresenhamLine(br, tr, color);

    GLPoint obl(25.0, 5.0, 0.0);
    GLPoint obr(5.0, 5.0, 0.0);
    GLPoint otl(25.0, 25.0, 0.0);
    GLPoint otr(5.0, 25.0, 0.0);

    renderer->drawBresenhamLine(obl, obr, color);
    renderer->drawBresenhamLine(obl, otl, color);
    renderer->drawBresenhamLine(obr, otr, color);
    renderer->drawBresenhamLine(otl, otr, color);

    GLPoint yellow_seed(15.0, 15.0, 0.0);
    Color yellow(1.0, 1.0, 0.0);
    renderer->seedFillArea(yellow_seed, color, yellow);
    */

  /* Aufgabenblatt 1, Aufgabe 3: Testen Sie Ihre seedFillArea-Methode hier */  
  /*
    GLPoint p3(250.0, 150.0, 0.0);
    GLPoint p4(250.0, 300.0, 0.0);
    GLPoint p5(400.0, 300.0, 0.0);
    GLPoint p6(400.0, 150.0, 0.0);
    Color borderColor(0.0, 0.0, 0.0);
    renderer->drawBresenhamLine(p3, p4, borderColor);
    renderer->drawBresenhamLine(p4, p5, borderColor);
    renderer->drawBresenhamLine(p5, p6, borderColor);
    renderer->drawBresenhamLine(p6, p3, borderColor);

    GLPoint seed(300.0, 250.0, 0.0);
    Color fillColor(0.1, 0.1, 0.1);
    renderer->seedFillArea(seed, borderColor, fillColor);
    img->writeAsPPM("wireframe.ppm");

    GLPoint f1(325.0, 190.0, 0.0);
    GLPoint f2(325.0, 225.0, 0.0);
    GLPoint f3(375.0, 220.0, 0.0);
    GLPoint f4(275.0, 220.0, 0.0);
    GLPoint f5(330.0, 193.0, 0.0);
    GLPoint f6(325.0, 200.0, 0.0);
    GLPoint f7(320.0, 193.0, 0.0);
    GLPoint f8(310.0, 223.0, 0.0);
    GLPoint f9(340.0, 223.0, 0.0);
    GLPoint f10(297.0, 221.0, 0.0);
    GLPoint f11(353.0, 221.0, 0.0);
    GLPoint f12(314.0, 213.0, 0.0);
    GLPoint f13(336.0, 213.0, 0.0);
    GLPoint f14(337.0, 265.0, 0.0);
    GLPoint f15(313.0, 265.0, 0.0);
    GLPoint f16(361.0, 252.0, 0.0);
    GLPoint f17(289.0, 252.0, 0.0);
    GLPoint f18(366.0, 285.0, 0.0);
    GLPoint f19(283.0, 285.0, 0.0);

    renderer->drawBresenhamLine(f1, f2, borderColor);
    renderer->drawBresenhamLine(f2, f3, borderColor);
    renderer->drawBresenhamLine(f3, f1, borderColor);
    renderer->drawBresenhamLine(f1, f4, borderColor);
    renderer->drawBresenhamLine(f2, f4, borderColor);
    renderer->drawBresenhamLine(f5, f6, borderColor);
    renderer->drawBresenhamLine(f6, f7, borderColor);
    renderer->drawBresenhamLine(f5, f9, borderColor);
    renderer->drawBresenhamLine(f7, f8, borderColor);
    renderer->drawBresenhamLine(f10, f12, borderColor);
    renderer->drawBresenhamLine(f11, f13, borderColor);
    renderer->drawBresenhamLine(f2, f14, borderColor);
    renderer->drawBresenhamLine(f2, f15, borderColor);
    renderer->drawBresenhamLine(f14, f15, borderColor);
    renderer->drawBresenhamLine(f14, f16, borderColor);
    renderer->drawBresenhamLine(f15, f17, borderColor);
    renderer->drawBresenhamLine(f8, f17, borderColor);
    renderer->drawBresenhamLine(f9, f16, borderColor);
    renderer->drawBresenhamLine(f4, f17, borderColor);
    renderer->drawBresenhamLine(f3, f16, borderColor);
    renderer->drawBresenhamLine(f15, f19, borderColor);
    renderer->drawBresenhamLine(f14, f18, borderColor);
    renderer->drawBresenhamLine(f17, f19, borderColor);
    renderer->drawBresenhamLine(f16, f18, borderColor);

    Color black(0.05, 0.05, 0.05);
    Color white(0.95, 0.95, 0.95);
    Color lightOrange(0.9, 0.45, 0.0);

    GLPoint s1(327.0, 193.0, 0.0);
    GLPoint s2(323.0, 193.0, 0.0);
    GLPoint s3(306.0, 220.0, 0.0);
    GLPoint s4(344.0, 220.0, 0.0);
    GLPoint s5(327.0, 210.0, 0.0);
    GLPoint s6(323.0, 210.0, 0.0);
    GLPoint s7(300.0, 210.0, 0.0);
    GLPoint s8(350.0, 210.0, 0.0);
    GLPoint s9(325.0, 230.0, 0.0);
    GLPoint s10(337.0, 230.0, 0.0);
    GLPoint s11(312.0, 230.0, 0.0);
    GLPoint s12(350.0, 230.0, 0.0);
    GLPoint s13(300.0, 230.0, 0.0);
    GLPoint s14(340.0, 265.0, 0.0);
    GLPoint s15(310.0, 265.0, 0.0);

    renderer->seedFillArea(s1, borderColor, black);
    renderer->seedFillArea(s2, borderColor, black);
    renderer->seedFillArea(s3, borderColor, black);
    renderer->seedFillArea(s4, borderColor, black);
    renderer->seedFillArea(s5, borderColor, lightOrange);
    renderer->seedFillArea(s6, borderColor, lightOrange);
    renderer->seedFillArea(s7, borderColor, white);
    renderer->seedFillArea(s8, borderColor, white);
    renderer->seedFillArea(s9, borderColor, lightOrange);
    renderer->seedFillArea(s10, borderColor, lightOrange);
    renderer->seedFillArea(s11, borderColor, lightOrange);
    renderer->seedFillArea(s12, borderColor, lightOrange);
    renderer->seedFillArea(s13, borderColor, lightOrange);
    renderer->seedFillArea(s14, borderColor, lightOrange);
    renderer->seedFillArea(s15, borderColor, lightOrange);
*/

 /* Aufgabenblatt 2, Aufgabe 3: Setzen Sie die Transformationen der Modelle */
    /**
    Color wireframeColor(1.0, 0.5, 0.0);

    Model &bunny = scene->getModels()[0];
    bunny.setTranslation(GLVector(250.0, 100.0, 0.0));
    bunny.setRotation(GLVector(0.0, M_PI/180*5.0, 0.0));
    bunny.setScale(GLVector(0.8, 0.8, 0.8));

    Model &cube = scene->getModels()[1];
    cube.setTranslation(GLVector(100.0, 100.0, 0.0));
    cube.setRotation(GLVector(M_PI/180*20.0, M_PI/180*45.0, 0.0));
    cube.setScale(GLVector(0.9, 0.9, 0.9));
    /*

    /* Aufgabenblatt 2, Aufgabe 1: Rufen Sie Ihre renderScene-Methode hier auf */

 // renderer->renderScene(wireframeColor);
 // img->writeAsPPM("wireframe.ppm");


  /* Setup der Camera - Erst ab Aufgabenblatt 3 relevant. */
  // Diese Einstellungen beziehen sich auf den world space
  // Beachten Sie, dass Sie in diesem Praktikum keine explizite Umwandlung in
  // den ViewSpace benötigen, da die Strahlen für Raycasting und Raytracing im
  // World space definiert sind. Modelle müssen also lediglich in den World
  // space überführt werden
  
  /* Aufgabenblatt 3:  kommentieren Sie die Zeilen wieder ein, die eine Kamera erzeugen und zur Scene hinzufügen*/
  
  auto cam = std::make_shared<Camera>();
  GLPoint eye = GLPoint(0.0, 0.0, 200.0);
  cam->setEyePoint(eye);
  cam->setUp(GLVector(0.0, 1.0, 0.0));
  GLVector viewDirection = GLVector(0.0, 0, -1.0);
  viewDirection.normalize();
  cam->setViewDirection(viewDirection);
  cam->setSize(img->getWidth(), img->getHeight());
  scene->setCamera(cam);


  /* Aufgabenblatt 3: Erzeugen Sie mindestens eine Kugel und fügen Sie diese zur Szene hinzu*/

  scene->addSphere(Sphere(GLPoint(-150.0, 0.0, -30.0), 50.0));
  scene->addSphere(Sphere(GLPoint(150.0, 0.0, -30.0), 50.0));
    
  /* Aufgabenblatt 4: Setzen Sie materialeigenschaften für die Kugelen und die Modelle. Die Materialeigenschaften für eine Darstellung entsprechend der Beispiellösung ist in der Aufgabenstellung gegeben. */

  /* Aufgabenblatt 3: (Wenn nötig) Transformationen der Modelle im World space, sodass sie von der Kamera gesehen werden könnnen. Die nötigen Transformationen für eine Darstellung entsprechend der Beispiellösung ist in der Aufgabenstellung gegeben. */
    Model &bunny = scene->getModels()[0];
    bunny.setTranslation(GLVector(0.0, 10.0, -30.0));
    bunny.setRotation(GLVector(0.0, M_PI/180*170.0, 0.0));
    Model &cube1 = scene->getModels()[1];
    cube1.setTranslation(GLVector(-60.0, -50.0, -0.0));
    Model &cube2 = scene->getModels()[2];
    cube2.setTranslation(GLVector(60.0, 50.0, -50.0));
    Model &cube3 = scene->getModels()[3];
    cube3.setTranslation(GLVector(-80.0, 10.0, -100.0));
    Model &cube4 = scene->getModels()[4];
    cube4.setTranslation(GLVector(0.0, -100.0, 0.0));
    cube4.setScale(GLVector(500, 0.01, 500));
    Model &cube5 = scene->getModels()[5];
    cube5.setScale(GLVector(300, 300, 300));

    // Flip cube5 normals
    for (Triangle tri : cube5.mTriangles) {
      tri.normal = GLVector(-tri.normal(0), -tri.normal(1), -tri.normal(2));
    }

  /* Stelle materialeigenschaften zur verfügung (Relevant für Aufgabenblatt 4)*/
    Color bunnyColor = Color(0.0f,0.0f,0.0f);
    Color cube1Color = Color(0.9,0.9,0.3);
    Material bunnyMaterial;
    bunnyMaterial.color = bunnyColor;
    bunnyMaterial.reflection = 1.0;
    Material cube1Material;
    cube1Material.color = cube1Color;
    Material cube2Material;
    cube2Material.color = Color(0.9,0.4,0.3);
    Material cube3Material;
    cube3Material.color = Color(1.0,0.0,0.0);
    Material cube4Material;
    cube4Material.color =  Color(0.9,0.9,0.9);
    Material cube5Material;
    cube5Material.color =  Color(0.9,0.9,0.9);
    Material sphere1Material;
    sphere1Material.color = Color(0.0,0.0,1.0);
    Material sphere2Material;
    sphere2Material.color = Color(0.0,1.0,1.0);
    bunny.setMaterial(bunnyMaterial);
    cube1.setMaterial(cube1Material);
    cube2.setMaterial(cube2Material);
    cube3.setMaterial(cube3Material);
    cube4.setMaterial(cube4Material);
    cube5.setMaterial(cube5Material);
    scene->getSpheres()[0].setMaterial(sphere1Material);
    scene->getSpheres()[1].setMaterial(sphere2Material);
  /* Aufgabenblatt 4  Fügen Sie ein Licht zur Szene hinzu */
  
    
  /* Aufgabenblatt 3: erzeugen Sie einen SolidRenderer (vorzugsweise mit einem shared_ptr) und rufen sie die Funktion renderRaycast auf */
  
  auto solidRenderer = std::make_shared<SolidRenderer>(scene, img, cam);
  solidRenderer->renderRaycast();

  // Schreiben des Bildes in Datei

if (argc > 1) {
img->writeAsPPM(argv[1]);
std::cout << "Bild mit Dimensionen " << img->getWidth() << "x"
<< img->getHeight() << " in Datei " << argv[1] << " geschrieben."
<< std::endl;
} else {
std::cerr
<< "Fehler: Kein Dateiname angegeben. Es wurde kein Output generiert."
<< std::endl;
}

return 0;
}
