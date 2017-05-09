#include "Application2D.h"
//HACK For tests
#include "Collider.h"
#include "AABox.h"
#include "OBox.h"
#include "CircleCollider.h"
#include "Plane.h"
#include "Ray.h"
#include <iostream>

int main() {

	//HACK TESTING

	// AABox to point
	AABox a1 = AABox({ 0,0 }, { 5,5 });
	std::pair<bool, Vector2> result = a1.doesCollide(Vector2(2, 2));
	if (result.first) {
		std::cout << "AABox collided with point (2,2)\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	}
	else {
		std::cout << "Failed: AABox missed point(2,2)\n";
		system("pause");
	}
	result = a1.doesCollide(Vector2(-2, 2));
	if (result.first) {
		std::cout << "Failed: AABox collided with point (-2,2)\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
		system("pause");
	}
	// AABox to ray
	Ray r1 = Ray(Vector2(-3, 2), Vector2(1, 0.5f));
	if (!r1.doesCollide(&a1)) {
		std::cout << "Failed: Ray(-3,2)(1,0.5) missed AABox\n";
		system("pause");
	}
	r1.setDirection({ -1,0.5f });
	if (r1.doesCollide(&a1)) {
		std::cout << "Failed: Ray(-3,2)(-1,0.5) hit AABox\n";
		system("pause");
	}
	// AABox to each collider

	AABox a2 = AABox({ 5,0 }, { 10,10 });
	result = a1.doesCollideWithAABox(&a2);
	if (result.first) {
		std::cout << "Failed: AABox (0,0)(5,5) and AABox(5,0)(10,10) collided\n\n";
		system("pause");
	}
	a2 = AABox({ 2,2 }, { 7,4 });
	result = a1.doesCollideWithAABox(&a2);
	if (!result.first) {
		std::cout << "Failed: AABox (0,0)(5,5) and AABox(2,2)(7,4) missed\n\n";
		system("pause");
	}
	else {
		std::cout << "AABox (0,0)(5,5) and AABox(2,2)(7,4) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	}
	a2 = AABox({ 2,2 }, { 3,3 });
	result = a1.doesCollideWithAABox(&a2);
	if (!result.first) {
		std::cout << "Failed: AABox (0,0)(5,5) and AABox(2,2)(3,3) missed\n\n";
		system("pause");
	}
	else {
		std::cout << "AABox (0,0)(5,5) and AABox(2,2)(3,3) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	}
	OBox o1 = OBox({ cosf(2),sinf(2) }, { -sinf(2),cosf(2) }, { 7,7 });
	result = a1.doesCollideWithOBox(&o1);
	if (result.first) {
		std::cout << "Failed: AABox (0,0)(5,5) and OBox(7,7) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
		system("pause");
	}
	o1.setCentre({ 1,1 });
	result = a1.doesCollideWithOBox(&o1);
	if (result.first) {
		std::cout << "AABox (0,0)(5,5) and OBox(1,1) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	}
	else {
		std::cout << "Failed: AABox (0,0)(5,5) and OBox(1,1) missed\n";
		system("pause");
	}
	CircleCollider c1 = CircleCollider({ -1,-1 }, 1);
	result = a1.doesCollideWithCircle(&c1);
	if (result.first) {
		std::cout << "Failed: AABox (0,0)(5,5) and CircleCollider({-1,-1},1) collided\n\n";
		system("pause");
	}
	c1.setRadius(2);
	result = a1.doesCollideWithCircle(&c1);
	if (result.first) {
		std::cout << "AABox (0,0)(5,5) and CircleCollider({-1,-1},2) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	}
	else {
		std::cout << "Failed: AABox (0,0)(5,5) and CircleCollider({-1,-1},2) missed\n\n";
		system("pause");
	}

	// OBox to ray
	o1 = OBox({ cosf(2),sinf(2) }, { -sinf(2),cosf(2) }, { 7,7 });
	r1 = Ray({ 5,5 }, { 1,1 });
	if (!r1.doesCollide(&o1)) {
		std::cout << "Failed: Ray(5,5)(1,1) missed OBox\n";
		system("pause");
	}
	r1.setDirection({ -1,-1 });
	if (r1.doesCollide(&o1)) {
		std::cout << "Failed: Ray(5,5)(-1,-1) hit OBox\n";
		system("pause");
	}
	r1.setOrigin({ 6.5, 6.5 });
	if (!r1.doesCollide(&o1)) {
		std::cout << "Failed: Ray(6.5,6.5)(-1,-1) missed OBox\n";
		system("pause");
	}
	// OBox to point
	result = o1.doesCollide({ 7,7 });
	if (result.first) {
		std::cout << "OBox (7,7, 2rad) and point(7,7) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	}
	else {
		std::cout << "Failed: OBox (7,7, 2rad) and point(7,7) missed\n\n";
		system("pause");
	}
	result = o1.doesCollide({ 7.5,7.5 });
	if (result.first) {
		std::cout << "OBox (7,7, 2rad) and point(7.5,7.5) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	}
	else {
		std::cout << "Failed: OBox (7,7, 2rad) and point(7.5,7.5) missed\n\n";
		system("pause");
	}
	result = o1.doesCollide({ 9,9 });
	if (result.first) {
		std::cout << "Failed: OBox (7,7, 2rad) and point(9,9) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
		system("pause");
	}
	else {
		std::cout << "OBox (7,7, 2rad) and point(9,9) missed\n\n";
	}
	// OBox to each collider
	OBox o2 = OBox({ 0,3 }, { 3,0 }, { 3.5,4 });
	result = o1.doesCollide(&o2);
	if (result.first) {
		std::cout << "OBox (7,7, 2rad) and OBox (3.5,4, 3x3) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	}
	else {
		std::cout << "Failed: OBox (7,7, 2rad) and OBox (3.5,4, 3x3) missed\n\n";
		system("pause");
	}
	o2.setHalfExtents({ 0,2 }, { 2,0 });
	result = o1.doesCollide(&o2);
	if (result.first) {
		std::cout << "Failed: OBox (7,7, 2rad) and OBox (3.5,4, 2x2) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
		system("pause");
	}
	c1.setCentre({ 5.5, 5.5 });
	result = o1.doesCollide(&c1);
	if (result.first) {
		std::cout << "OBox (7,7, 2rad) and Circle (5.5,5.5, 2) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	}
	else {
		std::cout << "Failed: OBox (7,7, 2rad) and Circle (5.5,5.5, 2) missed\n\n";
		system("pause");
	}
	c1.setCentre({ 10,10 });
	result = o1.doesCollide(&c1);
	if (result.first) {
		std::cout << "Failed: OBox (7,7, 2rad) and Circle (5.5,5.5, 2) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
		system("pause");
	}
	// TODO Circle to ray
	// TODO Circle to point
	// Circle to each collider
	CircleCollider c2 = CircleCollider({ 0,0 }, 2);
	c1.setCentre({ 5.5, 5.5 });
	result = c1.doesCollide(&c2);
	if (result.first) {
		std::cout << "Failed: Circle (5.5,5.5, 2) and Circle(0,0,2) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
		system("pause");
	}
	c2.setRadius(10);
	result = c1.doesCollide(&c2);
	if (result.first) {
		std::cout << "Circle (5.5,5.5, 2) and Circle(0,0,10) collided\nVector: " << result.second[0] << ", " << result.second[1] << "\n";
	} else{
		std::cout << "Failed: Circle (5.5,5.5, 2) and Circle(0,0,10) missed\n\n";
		system("pause");
	}
	//system("pause");
	//TODO plane tests
	//HACK end tests

	// allocation
	auto app = new Application2D();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}