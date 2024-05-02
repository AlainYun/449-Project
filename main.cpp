/**
This application renders a textured mesh that was loaded with Assimp.
*/

#include <iostream>
#include <memory>
#include<glad/glad.h>

#include "Mesh3D.h"
#include "Object3D.h"
#include "AssimpImport.h"
#include "Animator.h"
#include "ShaderProgram.h"

/**
 * @brief Defines a collection of objects that should be rendered with a specific shader program.
 */

struct Scene {
	ShaderProgram defaultShader;
	std::vector<Object3D> objects;
	std::vector<Animator> animators;
};

ShaderProgram phongLighting() {
	ShaderProgram program;
	try {
		program.load("shaders/light_perspective.vert", "shaders/multilights.frag");
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		exit(1);
	}
	return program;
}

/**
 * @brief Constructs a shader program that renders textured meshes without lighting.
 */
ShaderProgram textureMapping() {
	ShaderProgram program;
	try {
		program.load("shaders/texture_perspective.vert", "shaders/texturing.frag");
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		exit(1);
	}
	return program;
}

/**
 * @brief Loads an image from the given path into an OpenGL texture.
 */
Texture loadTexture(const std::filesystem::path& path, const std::string& samplerName = "baseTexture") {
	sf::Image i;
	i.loadFromFile(path.string());
	return Texture::loadImage(i, samplerName);
}

Scene lightTest() {
	std::vector<Texture> light = {
	loadTexture("models/light.png", "baseTexture")
	};
	std::vector<Texture> wall = {
	loadTexture("models/wall.jpg", "baseTexture")
	};

	auto bunny = assimpLoad("models/bunny_textured.obj", true);
	bunny.setScale(glm::vec3(2, 2, 2));
	bunny.rotate(glm::vec3(0, 0, 0));
	bunny.setPosition(glm::vec3(0, 0, 0));
	bunny.setMass(1.f);
	
	auto cmesh = Mesh3D::cube(wall);
	auto mesh2 = Mesh3D::square(light);
	auto square = Object3D(std::vector<Mesh3D>{mesh2});
	auto square1 = Object3D(std::vector<Mesh3D>{mesh2});
	auto square2 = Object3D(std::vector<Mesh3D>{mesh2});
	auto square3 = Object3D(std::vector<Mesh3D>{mesh2});
	auto cube = Object3D(std::vector<Mesh3D>{cmesh});
	auto cube1 = Object3D(std::vector<Mesh3D>{cmesh});
	auto cube2 = Object3D(std::vector<Mesh3D>{cmesh});
	auto cube3 = Object3D(std::vector<Mesh3D>{cmesh});

	cube.setPosition(glm::vec3(0,0,0));
	cube1.setPosition(glm::vec3(2.1,1.1,-1));
	cube2.setPosition(glm::vec3(2.3,0,2));
	cube3.setPosition(glm::vec3(2.2,.5,3));
	cube.setScale(glm::vec3(.5, .5, .5));
	cube1.setScale(glm::vec3(.5, .5, .5));
	cube2.setScale(glm::vec3(.5, .5, .5));
	cube3.setScale(glm::vec3(.5, .5, .5));

	square.rotate(glm::vec3(glm::radians(-90.f), 0, 0));
	square.setPosition(glm::vec3(0, -1, 0));
	square.setScale(glm::vec3(3, 3, 3));

	square1.rotate(glm::vec3(0, 0, 0));
	square1.setPosition(glm::vec3(0, 0, -1.5));
	square1.setScale(glm::vec3(3, 3, 3));

	square2.rotate(glm::vec3(0, glm::radians(-90.f), 0));
	square2.setPosition(glm::vec3(1.5, 0, 0));
	square2.setScale(glm::vec3(3, 3, 3));

	square3.rotate(glm::vec3(0, glm::radians(90.f), 0));
	square3.setPosition(glm::vec3(-1.5, 0, 0));
	square3.setScale(glm::vec3(3, 3, 3));

	std::vector<Object3D> objects;
	objects.push_back(std::move(bunny));
	objects.push_back(std::move(square));
	objects.push_back(std::move(square1));
	objects.push_back(std::move(square2));
	objects.push_back(std::move(square3));
	objects.push_back(std::move(cube));
	objects.push_back(std::move(cube1));
	objects.push_back(std::move(cube2));
	objects.push_back(std::move(cube3));
	return Scene{
		phongLighting(),
		std::move(objects),
	};
}


glm::vec3 Random() {
	srand(time(NULL));
	int randomNum0 = rand() % 11 + (-5);
	int randomNum1 = rand() % 5 + (1);
	int randomNum2 = rand() % 11 + (-5);
	return glm::vec3(randomNum0, randomNum1, randomNum2);
}


int main() {
	// Initialize the window and OpenGL.
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	unsigned int width = 1200;
	unsigned int height = 800;
	sf::RenderWindow window(sf::VideoMode{ width, height }, "SFML Demo", sf::Style::Resize | sf::Style::Close, Settings);
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);

	// Initialize scene objects.
	auto scene = lightTest();

	//camera stuff
	double fov = 45.0;
	auto cameraPos = glm::vec3(0, 0, 2);
	float cameraSpeed;
	auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	auto camera = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	auto perspective = glm::perspective(glm::radians(fov), static_cast<double>(window.getSize().x) / window.getSize().y, 0.1, 100.0);
	float yaw = -90.0f;
	float pitch = 0.0f;

	ShaderProgram& mainShader = scene.defaultShader;

	mainShader.activate();
	auto& o = scene.objects[0];
	auto& pos = o.getPosition();
	mainShader.setUniform("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	mainShader.setUniform("dirLight.ambient", glm::vec3(.01, .01, .01));
	mainShader.setUniform("dirLight.diffuse", glm::vec3(.05, .05, .05));
	mainShader.setUniform("dirLight.specular", glm::vec3(1, 1, 1));

	mainShader.setUniform("spotLight.position", cameraPos);
	mainShader.setUniform("spotLight.direction", cameraFront);
	mainShader.setUniform("spotLight.ambient", glm::vec3(0, 0, 0));
	mainShader.setUniform("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	mainShader.setUniform("spotLight.specular", glm::vec3( 1, 1, 1));
	mainShader.setUniform("spotLight.constant", 1.0f);
	mainShader.setUniform("spotLight.linear", 0.09f);
	mainShader.setUniform("spotLight.quadratic", 0.032f);
	mainShader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	mainShader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	mainShader.setUniform("material", glm::vec4(0, .1, 1, 32.f));

	// Ready, set, go!
	for (auto& animator : scene.animators) {
		animator.start();
	}
	bool running = true;
	sf::Clock c;
	auto last = c.getElapsedTime();
	while (running) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				running = false;
			}
		}
		o.addForce(glm::vec3(0, -9.8 * o.getMass(), 0));

		camera = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		mainShader.setUniform("view", camera);
		mainShader.setUniform("projection", perspective);
		mainShader.setUniform("spotLight.direction", cameraFront);
		mainShader.setUniform("viewPos", cameraPos);
		mainShader.setUniform("spotLight.position", cameraPos);

		auto now = c.getElapsedTime();
		auto diff = now - last;
		auto diffSeconds = diff.asSeconds();
		cameraSpeed = 2.0f * diffSeconds;
		last = now;
		o.tick(diff.asSeconds());
		for (auto& animator : scene.animators) {
			animator.tick(diffSeconds);
		}
		
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::B)) {
			o.setRotationalVelocity(Random());
			o.setVelocity(Random());
			//o.addForce(glm::vec3(0,100,0));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) {
			cameraPos += cameraSpeed * cameraUp;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LControl)) {
			cameraPos -= cameraSpeed * cameraUp;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
			cameraPos += cameraSpeed * cameraFront;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
			cameraPos -= cameraSpeed * cameraFront;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
			pitch += cameraSpeed * 20;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
			pitch -= cameraSpeed * 20;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
			yaw -= cameraSpeed * 20;	
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
			yaw += cameraSpeed * 20;
			
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q)) {
			fov = 20.0;
		}
		else {
			fov = 45.0;
		}

		// Limit pitch to straight up or straight down with a little headroom
		pitch = std::max(-89.0f, std::min(89.0f, pitch));

		// Wrap yaw values: 0 <= yaw < 360
		if (yaw < 0.0f) {
			yaw += 360.0f;
		}
		else if (yaw >= 360.0f) {
			yaw -= 360.0f;
		}

		// Recalculate the front vector
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
		

		
		// Clear the OpenGL "context".
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Render each object in the scene.

		if (pos.y < -1) {
			o.setPosition(glm::vec3(pos.x, -1, pos.z));
			o.setVelocity(glm::vec3(1, -1, 1) * o.getVelocity() * 0.7f);
		}
		if (pos.y > 1.5) {
			o.setPosition(glm::vec3(pos.x, 1.5, pos.z));
			o.setVelocity(glm::vec3(1, -1, 1) * o.getVelocity() * 0.7f);
		}
		if (pos.x < -1.5) {
			o.setPosition(glm::vec3(-1.5, pos.y, pos.z));
			o.setVelocity(glm::vec3(-1, 1, 1) * o.getVelocity() * 0.7f);
		}
		if (pos.x > 1.5) {
			o.setPosition(glm::vec3(1.5, pos.y, pos.z));
			o.setVelocity(glm::vec3(-1, 1, 1) * o.getVelocity() * 0.7f);
		}
		if (pos.z < -1.5) {
			o.setPosition(glm::vec3(pos.x, pos.y, -1.5));
			o.setVelocity(glm::vec3(1, 1, -1) * o.getVelocity() * 0.7f);
		}
		if (pos.z > 1.5) {
			o.setPosition(glm::vec3(pos.x, pos.y, 1.5));
			o.setVelocity(glm::vec3(1, 1, -1) * o.getVelocity() * 0.7f);
		}
		for (int i = 0; i < 3; ++i) {
			if (o.getRotationalVelocity()[i] < 0) {
				glm::vec3 newAcceleration = o.getRotationalAcceleration();
				newAcceleration[i] = 1.5;
				o.setRotationalAcceleration(newAcceleration);
			}
			else if (o.getRotationalVelocity()[i] > 0) {
				glm::vec3 newAcceleration = o.getRotationalAcceleration();
				newAcceleration[i] = -1.5;
				o.setRotationalAcceleration(newAcceleration);
			}

			if (o.getRotationalVelocity()[i] == 0) {
				glm::vec3 newVelocity (0,0,0);
				o.setRotationalVelocity(newVelocity);
				o.setRotationalAcceleration(glm::vec3(0,0,0));
			}
		}
		for (auto& obj : scene.objects) {
			obj.render(window, mainShader);
		}
		window.display();
	}

	return 0;
}


