#include"Gun.h"

using namespace glm;

Gun::Gun(Cube& cube) {
	kyvos = &cube;
	bull = new Sphere(vec3(-1, -1,-1), vec3(0, 0, 0), 0.001, 0.000001);
}

void Gun::shoot() {
	bull = new Sphere(kyvos->x + vec3(0, 0.1, 0), vec3(0, 10, 0), 0.1, 10);
}
void Gun::updateBulets() {
	bull->update(0.1,0.00001);
	bull->draw();

}