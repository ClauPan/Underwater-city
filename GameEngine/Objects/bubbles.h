#pragma once
#include <glm.hpp>
#include <gtx\transform.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "..\Graphics\window.h"

class Bubbles {
private:
	glm::mat4 matrix;
	glm::vec3 scale;
	glm::vec3 startPosition;
	glm::vec3 endPosition;
	glm::vec3 offSetPosition;
	bool active;
	float angleOx;
	float angleOy;

public:
	Bubbles();
	Bubbles(glm::vec3 position, glm::vec3 offset, glm::vec3 scale);
	~Bubbles();
	glm::mat4 getMatrix();
	glm::vec3 getScale();
	glm::vec3 getStartPos();
	glm::vec3 getEndPos();
	glm::vec3 getOffSet();
	float getAngleOx();
	float getAngleOy();
	bool getActive();

	void setMatrix(glm::mat4 obj);
	void setScale(glm::vec3 obj);
	void setStart(glm::vec3 obj);
	void setEnd(glm::vec3 obj);
	void setOffSet(glm::vec3 obj);
	void setAngleOx(float obj);
	void setAngleOy(float obj);
};
