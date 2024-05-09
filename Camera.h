#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPACE,
	CTRL
};

enum Camera_Angle {
	CUP,
	CDOWN,
	CLEFT,
	CRIGHT
};


const float YAW = 90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENS = .04f;
const float ZOOM = 45.0f;

class Camera {
public:
	glm::vec3 Pos;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;
	float Movespeed;
	float Sens;
	float Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Movespeed(SPEED),
	Sens(SENS), Zoom(ZOOM){
		Pos = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Movespeed(SPEED), Sens(SENS), Zoom(ZOOM)
	{
		Pos = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Pos, Pos + Front, Up);
	}
	void ProcessMovement(Camera_Movement direction, bool FPS) {
		float velocity = Movespeed;
		//FPS
		if (FPS) {
			glm::vec3 moveDirection = Front;
			moveDirection.y = 0;
			moveDirection = glm::normalize(moveDirection);
			if (direction == FORWARD)
				Pos += moveDirection * velocity;
			if (direction == BACKWARD)
				Pos -= moveDirection * velocity;
		}
		if (!FPS) {
			if (direction == FORWARD)
				Pos += Front * velocity;
			if (direction == BACKWARD)
				Pos -= Front * velocity;
			if (direction == SPACE)
				Pos += Up * velocity;
			if (direction == CTRL)
				Pos -= Up * velocity;
		}
		
		if (direction == LEFT)
			Pos -= Right * velocity;
		if (direction == RIGHT)
			Pos += Right * velocity;
	}

	void ProcessAngle(Camera_Angle angle) {
		Pitch = std::max(-89.0f, std::min(89.0f, Pitch));
		if (angle == CUP)
			Pitch += Sens;
		if (angle == CDOWN)
			Pitch -= Sens;
		if (angle == CLEFT)
			Yaw -= Sens;
		if (angle == CRIGHT)
			Yaw += Sens;

		if (Yaw < 0.0f) {
			Yaw += 360.0f;
		}
		else if (Yaw >= 360.0f) {
			Yaw -= 360.0f;
		}
		updateCameraVectors();
	}

private:
	void updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};