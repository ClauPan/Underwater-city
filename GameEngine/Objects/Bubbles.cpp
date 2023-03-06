#include "Bubbles.h"

Bubbles::Bubbles() {
	this->matrix = glm::mat4(1.0f);
	this->active = true;
}
Bubbles::Bubbles(glm::vec3 position, glm::vec3 offset, glm::vec3 scale) {
	this->matrix = glm::mat4(1.0f);
	this->scale = scale;
	this->startPosition = glm::vec3(0.03f, 0.03f, 0.03f);
	this->endPosition = glm::vec3(0.03f, 0.03f, 0.03f);
	this->offSetPosition = offset;
	this->angleOx = 0.0f;
	this->angleOy = 0.0f;
	this->active = true;
	matrix=glm::translate(matrix, position);
	matrix=glm::rotate(matrix, angleOx, glm::vec3(1, 0, 0));
	matrix=glm::rotate(matrix, angleOy, glm::vec3(0, 1, 0));
	matrix=glm::translate(matrix, offset);
	matrix=glm::scale(matrix, scale);
}

Bubbles::~Bubbles() { }

glm::mat4 Bubbles:: getMatrix() {
	return matrix;
}
glm::vec3 Bubbles::getScale() {
	return scale;
}
glm::vec3 Bubbles::getStartPos() {
	return startPosition;
}
glm::vec3 Bubbles::getEndPos() {
	return endPosition;
}
glm::vec3 Bubbles::getOffSet() {
	return offSetPosition;
}
float Bubbles::getAngleOx() {
	return angleOx;
}
float Bubbles::getAngleOy() {
	return angleOy;
}
bool Bubbles::getActive() {
	return active;
}

void Bubbles::setMatrix(glm::mat4 obj) {
	this->matrix = obj;
}
void Bubbles::setScale(glm::vec3 obj) {
	this->scale = obj;
}
void Bubbles::setStart(glm::vec3 obj) {
	this->startPosition = obj;
}
void Bubbles::setEnd(glm::vec3 obj) {
	this->endPosition = obj;
}
void Bubbles::setOffSet(glm::vec3 obj) {
	this->offSetPosition = obj;
}
void Bubbles::setAngleOx(float obj) {
	this->angleOx = obj;
}
void Bubbles::setAngleOy(float obj) {
	this->angleOy = obj;
}