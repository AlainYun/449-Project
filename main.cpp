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
#include "Camera.h"

/**
 * @brief Defines a collection of objects that should be rendered with a specific shader program.
 */

struct Scene {
	ShaderProgram defaultShader;
	std::vector<Object3D> objects;
	std::vector<Animator> animators;
	//std::vector<ParallelAnimator> panimators;
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

Scene Intro() {
	auto skybox = assimpLoad("models/Sky/Skybox.obj", true);
	auto car = assimpLoad("models/Intro/Car.obj", true);
	auto carNW = assimpLoad("models/Intro/CarNoWindows.obj", true);
	auto carDoor = assimpLoad("models/Intro/CarDoor.obj", true);
	auto road = assimpLoad("models/Intro/Road.obj", true);
	auto road1 = assimpLoad("models/Intro/Road.obj", true);
	auto road2 = assimpLoad("models/Intro/Road.obj", true);
	auto trees = assimpLoad("models/intro/trees/Trees.obj", true);
	auto trees1 = assimpLoad("models/intro/trees/Trees.obj", true);
	auto trees2 = assimpLoad("models/intro/trees/Trees.obj", true);
	auto badCar = assimpLoad("models/intro/CarNoWheels.obj", true);

	auto swingT = assimpLoad("models/Body/SwingTop.obj", true);
	auto swingB = assimpLoad("models/Body/SwingBot.obj", true);
	auto armBotL = assimpLoad("models/Body/ArmBotL.obj", true);
	auto armTopL = assimpLoad("models/Body/ArmTopL.obj", true);
	auto armBotR = assimpLoad("models/Body/ArmBotR.obj", true);
	auto armTopR = assimpLoad("models/Body/ArmTopR.obj", true);
	auto legBotL = assimpLoad("models/Body/LegBotL.obj", true);
	auto legTopL = assimpLoad("models/Body/LegTopL.obj", true);
	auto legBotR = assimpLoad("models/Body/LegBotR.obj", true);
	auto legTopR = assimpLoad("models/Body/LegTopR.obj", true);
	auto body = assimpLoad("models/Body/Body.obj", true);
	auto head = assimpLoad("models/Body/Head.obj", true);

	swingT.setPosition(glm::vec3(0,5,0));
	
	swingB.addChild(std::move(swingT));
	swingB.setScale(glm::vec3(.16));
	swingB.rotate(glm::vec3(0, glm::radians(270.0f),0));
	swingB.setPosition(glm::vec3(11, 10.2, .2));

	armBotL.setPosition(glm::vec3(0, -1.5, 0));
	armTopL.setPosition(glm::vec3(-1.5, 1.5, 0));
	armBotR.setPosition(glm::vec3(0, -1.5, 0));
	armTopR.setPosition(glm::vec3(1.5, 1.5, 0));
	legBotL.setPosition(glm::vec3(0, -1.5, 0));
	legTopL.setPosition(glm::vec3(-.5, -2.5, 0));
	legBotR.setPosition(glm::vec3(0, -1.5, 0));
	legTopR.setPosition(glm::vec3(.5,-2.5,0));
	head.setPosition(glm::vec3(0,2,0));

	legTopL.rotate(glm::vec3(0, 0, glm::radians(-10.0f)));
	legTopR.rotate(glm::vec3(0, 0, glm::radians(10.0f)));
	legTopR.addChild(std::move(legBotR));//2.1
	legTopL.addChild(std::move(legBotL));//3.1
	armTopR.addChild(std::move(armBotR));//4.1
	armTopL.addChild(std::move(armBotL));//5.1
	body.addChild(std::move(head));//1
	body.addChild(std::move(legTopR));//2
	body.addChild(std::move(legTopL));//3
	body.addChild(std::move(armTopR));//4
	body.addChild(std::move(armTopL));//5

	body.setScale(glm::vec3(.16));
	body.rotate(glm::vec3(0, glm::radians(90.0f), 0));
	body.setPosition(glm::vec3(54, .9, 0));

	badCar.setPosition(glm::vec3(58, 0, 3));
	badCar.rotate(glm::vec3(0, glm::radians(110.0f), 0));

	carDoor.setPosition(glm::vec3(.74779,0, .4537));
	carDoor.rotate(glm::vec3(0, glm::radians(90.0f), 0));
	//carNW.addChild(std::move(carDoor));
	carNW.rotate(glm::vec3(0, glm::radians(90.0f), 0));
	carNW.setPosition(glm::vec3(0, 0, 1.2));

	car.rotate(glm::vec3(0, glm::radians(90.0f), 0));
	car.setPosition(glm::vec3(0,0,1.2));

	road.addChild(std::move(trees));
	road1.addChild(std::move(trees1));
	road2.addChild(std::move(trees2));

	road.setScale(glm::vec3(.06));
	road.setPosition(glm::vec3(0, 0, 0));
	road1.setScale(glm::vec3(.06));
	road1.setPosition(glm::vec3(18, 0, 0));
	road2.setScale(glm::vec3(.06));
	road2.setPosition(glm::vec3(36, 0, 0));


	std::vector<Object3D> objects;
	objects.push_back(std::move(skybox));//0
	objects.push_back(std::move(car));//1
	objects.push_back(std::move(road));//2
	objects.push_back(std::move(road1));//3
	objects.push_back(std::move(road2));//4
	objects.push_back(std::move(body));//5
	objects.push_back(std::move(carDoor));
	objects.push_back(std::move(carNW));
	objects.push_back(std::move(badCar));
	objects.push_back(std::move(swingB));


	Animator ArmMoveR;
	ArmMoveR.addAnimation(std::make_unique<PauseAnimation>(objects[5].getChild(4), 4));
	ArmMoveR.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(4), .5, glm::vec3(0, 0, 1.5)),true);
	ArmMoveR.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(4).getChild(1), .5, glm::vec3(0, 0, .5)),true);
	for (int i = 0; i < 5; i++) {
		ArmMoveR.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(4), .5, glm::vec3(0, 0, -1)));
		ArmMoveR.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(4), .5, glm::vec3(0, 0, 1)));
		i++;
	}
	ArmMoveR.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(4), .5, glm::vec3(0, 0, -2)));
	ArmMoveR.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(4).getChild(1), .5, glm::vec3(0, 0, -1.5)));
	
	Animator ArmMoveL;
	ArmMoveL.addAnimation(std::make_unique<PauseAnimation>(objects[5].getChild(5), 4));
	ArmMoveL.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(5), .5, glm::vec3(0, 0, -1.5)), true);
	ArmMoveL.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(5).getChild(1), .5, glm::vec3(0, 0, -.5)), true);
	for (int i = 0; i < 5; i++) {
		ArmMoveL.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(5), .5, glm::vec3(0, 0, 1)));
		ArmMoveL.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(5), .5, glm::vec3(0, 0, -1)));
		i++;
	}
	ArmMoveL.addAnimation(std::make_unique<PauseAnimation>(objects[5].getChild(5), 1));
	ArmMoveL.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(5), .5, glm::vec3(0, 0, .7)));
	ArmMoveL.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(5).getChild(1), .5, glm::vec3(0, 0, 1.2)));

	Animator LegMoveL;
	LegMoveL.addAnimation(std::make_unique<PauseAnimation>(objects[5].getChild(3), 8));
	LegMoveL.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(3), 1, glm::vec3(.5, .25, 0)), true);
	LegMoveL.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(3).getChild(1), 1, glm::vec3(-.25, 0, 0)), true);
	
	Animator Body;
	Body.addAnimation(std::make_unique<TranslationAnimation>(objects[5], 4, glm::vec3(-36, 0, 0)));
	Body.addAnimation(std::make_unique<TranslationAnimation>(objects[5], 4, glm::vec3(-10, 0, 0)));
	Body.addAnimation(std::make_unique<RotationAnimation>(objects[5], 2, glm::vec3(.1, 1, .1)));

	Animator bCar;
	bCar.addAnimation(std::make_unique<TranslationAnimation>(objects[8], 4, glm::vec3(-36, 0, 0)));
	bCar.addAnimation(std::make_unique<TranslationAnimation>(objects[8], 4, glm::vec3(-10, 0, 0)));
	

	Animator Head;
	Head.addAnimation(std::make_unique<PauseAnimation>(objects[5].getChild(1), 8));
	Head.addAnimation(std::make_unique<RotationAnimation>(objects[5].getChild(1), 2, glm::vec3(0, .5, 0)));

	Animator roadmove;
	roadmove.addAnimation(std::make_unique<TranslationAnimation>(objects[2], 4, glm::vec3(-36, 0, 0)));
	roadmove.addAnimation(std::make_unique<TranslationAnimation>(objects[2], .01, glm::vec3(0, 3, 0)));
	roadmove.addAnimation(std::make_unique<TranslationAnimation>(objects[2], .05, glm::vec3(36, 0, 0)));
	roadmove.addAnimation(std::make_unique<TranslationAnimation>(objects[2], .01, glm::vec3(0, -3, 0)));
	roadmove.addAnimation(std::make_unique<TranslationAnimation>(objects[2], 4, glm::vec3(-10, 0, 0)));
	Animator roadmove1;
	roadmove1.addAnimation(std::make_unique<TranslationAnimation>(objects[3], 4, glm::vec3(-36, 0, 0)));
	roadmove1.addAnimation(std::make_unique<TranslationAnimation>(objects[3], .01, glm::vec3(0, 3, 0)));
	roadmove1.addAnimation(std::make_unique<TranslationAnimation>(objects[3], .05, glm::vec3(36, 0, 0)));
	roadmove1.addAnimation(std::make_unique<TranslationAnimation>(objects[3], .01, glm::vec3(0, -3, 0)));
	roadmove1.addAnimation(std::make_unique<TranslationAnimation>(objects[3], 4, glm::vec3(-10, 0, 0)));
	Animator roadmove2;
	roadmove2.addAnimation(std::make_unique<TranslationAnimation>(objects[4], 4, glm::vec3(-36, 0, 0)));
	roadmove2.addAnimation(std::make_unique<TranslationAnimation>(objects[4], .01, glm::vec3(0, 3, 0)));
	roadmove2.addAnimation(std::make_unique<TranslationAnimation>(objects[4], .05, glm::vec3(36, 0, 0)));
	roadmove2.addAnimation(std::make_unique<TranslationAnimation>(objects[4], .01, glm::vec3(0, -3, 0)));
	roadmove2.addAnimation(std::make_unique<TranslationAnimation>(objects[4], 4, glm::vec3(-10, 0, 0)));

	Animator carSwap;
	carSwap.addAnimation(std::make_unique<PauseAnimation>(objects[1], 3));
	carSwap.addAnimation(std::make_unique<TranslationAnimation>(objects[1], .1,glm::vec3(0,-2,0)));

	Animator carDoorMove;
	carDoorMove.addAnimation(std::make_unique<PauseAnimation>(objects[6], 12));
	carDoorMove.addAnimation(std::make_unique<RotationAnimation>(objects[6], 1, glm::vec3(0, -.5, 0)));

	Animator batSwing;
	batSwing.addAnimation(std::make_unique<PauseAnimation>(objects[9], 24.2));
	batSwing.addAnimation(std::make_unique<TranslationAnimation>(objects[9], .1 ,glm::vec3(0,-10,0)));
	batSwing.addAnimation(std::make_unique<PauseAnimation>(objects[9], .1));
	batSwing.addAnimation(std::make_unique<RotationAnimation>(objects[9], .5,glm::vec3(0,glm::radians(90.0f), 0)));
	

	std::vector<Animator> animators;
	animators.push_back(std::move(Head));
	animators.push_back(std::move(Body));
	animators.push_back(std::move(carSwap));
	animators.push_back(std::move(LegMoveL));
	animators.push_back(std::move(ArmMoveR));
	animators.push_back(std::move(ArmMoveL));
	animators.push_back(std::move(roadmove));
	animators.push_back(std::move(roadmove1));
	animators.push_back(std::move(roadmove2));
	animators.push_back(std::move(carDoorMove));
	animators.push_back(std::move(bCar));
	animators.push_back(std::move(batSwing));

	return Scene{
		phongLighting(),
		std::move(objects),
		std::move(animators),
	};
}

Scene Game() {
	auto level = assimpLoad("models/Game/Level.obj", true);
	auto cap = assimpLoad("models/Game/cap.obj", true);
	auto glowstick = assimpLoad("models/game/GlowStick.obj", true);
	auto carrot0 = assimpLoad("models/game/Carrot0.obj", true);
	auto carrot1 = assimpLoad("models/game/Carrot1.obj", true);
	auto carrot2 = assimpLoad("models/game/Carrot2.obj", true);
	auto carrot3 = assimpLoad("models/game/Carrot3.obj", true);
	auto carrotc = assimpLoad("models/game/CarrotC.obj", true);
	glowstick.setMass(10.0f);
	glowstick.setScale(glm::vec3(.1));
	glowstick.setPosition(glm::vec3(0, 5, 0));

	cap.setPosition(glm::vec3(0, .91351, -5.6626));
	//cap.rotate(glm::vec3(glm::radians(-90.0f), 0, 0));
	std::vector<Object3D> objects;
	objects.push_back(std::move(level));
	objects.push_back(std::move(glowstick));
	objects.push_back(std::move(cap));
	objects.push_back(std::move(carrot0));
	objects.push_back(std::move(carrot1));
	objects.push_back(std::move(carrot2));
	objects.push_back(std::move(carrot3));
	objects.push_back(std::move(carrotc));

	return Scene{
		phongLighting(),
		std::move(objects),
	};
}

int main() {
	bool car0 = false;
	bool car1 = false;
	bool car2 = false;
	bool car3 = false;
	bool CameraEnabled = false;
	bool FPS = false;
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
	auto scene = Intro();
	bool boolscene = true;
	auto scene1 = Game();
	bool boolscene1 = false;

	auto& glow0 = scene1.objects[1];

	//camera stuff
	Camera camera;
	double fov = 45.0;
	auto perspective = glm::perspective(glm::radians(fov), static_cast<double>(window.getSize().x) / window.getSize().y, 0.1, 100.0);
	camera.Pos = glm::vec3(4, 5, 0);
	camera.Front = glm::vec3(-.6, -.6, .3);
	//
	//camera.Pos = (glm::vec3(95, 1, 45));
	//
	ShaderProgram& mainShader = scene.defaultShader;

	glEnable(GL_LIGHT1 + 1);
	mainShader.activate();

	mainShader.setUniform("pointLight.position", glm::vec3(0, 0, 0));
	mainShader.setUniform("pointLight.ambient", glm::vec3(0));
	mainShader.setUniform("pointLight.diffuse", glm::vec3(0));
	mainShader.setUniform("pointLight.specular", glm::vec3(0));
	mainShader.setUniform("pointLight.constant", 1.0f);
	mainShader.setUniform("pointLight.linear", 0.09f);
	mainShader.setUniform("pointLight.quadratic", 0.032f);

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
		window.clear();
		glm::mat4 view = camera.GetViewMatrix();
		perspective = glm::perspective(glm::radians(fov), static_cast<double>(window.getSize().x) / window.getSize().y, 0.1, 100.0);

		mainShader.setUniform("viewPos", camera.Pos);
		mainShader.setUniform("view", view);
		mainShader.setUniform("projection", perspective);
		mainShader.setUniform("material", glm::vec4(.1, .5, 1, 32));

		//std::cout << "X: " << camera.Front.x << "Y: " << camera.Front.y << "Z: " << camera.Front.z << std::endl;
		std::cout << "PX: " << camera.Pos.x << "PY: " << camera.Pos.y << "PZ: " << camera.Pos.z << std::endl;
		//std::cout << "Y: " << camera.Yaw << "P:" << camera.Pitch << std::endl;
		auto now = c.getElapsedTime();
		auto diff = now - last;
		auto diffSeconds = diff.asSeconds();
		last = now;
		camera.Movespeed = 10.0f * diffSeconds;

		// camera controls:
		if (CameraEnabled) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Equal)) {
				if (!FPS)
					FPS = true;
				else
					FPS = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) {
				camera.ProcessMovement(SPACE, FPS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LControl)) {
				camera.ProcessMovement(CTRL, FPS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) {
				camera.ProcessMovement(FORWARD, FPS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) {
				camera.ProcessMovement(BACKWARD, FPS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {
				camera.ProcessMovement(RIGHT, FPS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {
				camera.ProcessMovement(LEFT, FPS);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
				camera.ProcessAngle(CUP);

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Down)) {
				camera.ProcessAngle(CDOWN);

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
				camera.ProcessAngle(CLEFT);

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
				camera.ProcessAngle(CRIGHT);

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q)) {
				fov = 20.0;
			}
			else {
				fov = 45.0;
			}
		}
		if (boolscene) {
			mainShader.setUniform("dirLight.direction", glm::vec3(0.0f, -6.0f, 0.0f));
			mainShader.setUniform("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			mainShader.setUniform("dirLight.diffuse", glm::vec3(0.04f, 0.04f, 0.04f));
			mainShader.setUniform("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			mainShader.setUniform("spotLight[0].position", glm::vec3(1.5, .45, .7));
			mainShader.setUniform("spotLight[0].direction", glm::vec3(1, 0, 0));
			mainShader.setUniform("spotLight[0].ambient", glm::vec3(0, 0, 0));
			mainShader.setUniform("spotLight[0].diffuse", glm::vec3(1, 1, 1));
			mainShader.setUniform("spotLight[0].specular", glm::vec3(1, 1, 1));
			mainShader.setUniform("spotLight[0].constant", 1.0f);
			mainShader.setUniform("spotLight[0].linear", 0.09f);
			mainShader.setUniform("spotLight[0].quadratic", 0.032f);
			mainShader.setUniform("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
			mainShader.setUniform("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));
			mainShader.setUniform("spotLight[1].position", glm::vec3(1.5, .45, 1.7));
			mainShader.setUniform("spotLight[1].direction", camera.Front);
			mainShader.setUniform("spotLight[1].ambient", glm::vec3(0, 0, 0));
			mainShader.setUniform("spotLight[1].diffuse", glm::vec3(1, 1, 1));
			mainShader.setUniform("spotLight[1].specular", glm::vec3(1, 1, 1));
			mainShader.setUniform("spotLight[1].constant", 1.0f);
			mainShader.setUniform("spotLight[1].linear", 0.09f);
			mainShader.setUniform("spotLight[1].quadratic", 0.032f);
			mainShader.setUniform("spotLight[1].cutOff", glm::cos(glm::radians(12.5f)));
			mainShader.setUniform("spotLight[1].outerCutOff", glm::cos(glm::radians(15.0f)));

			if (c.getElapsedTime().asSeconds() > 1.5 && c.getElapsedTime().asSeconds() < 9) {
				if (camera.Pos.x > 0)
					camera.Pos.x -= 1.5 * diffSeconds;
				if (camera.Pos.y > 1)
					camera.Pos.y -= 1 * diffSeconds;
				if (camera.Pos.z < 1)
					camera.Pos.z += 1 * diffSeconds;
				if (camera.Front.x < 2)
					camera.Front.x += .5 * diffSeconds;
			}

			if (c.getElapsedTime().asSeconds() > 5 && c.getElapsedTime().asSeconds() < 11) {
				if (camera.Front.y < 0 && camera.Front.z > 0) {
					camera.Front.y = 0;
					camera.Front.z = 0;
				}
			}

			if (c.getElapsedTime().asSeconds() > 11 && c.getElapsedTime().asSeconds() < 13) {
				if (camera.Front.z > 0)
					camera.Front.x += 2 * diffSeconds;
				camera.Front.z -= 2 * diffSeconds;
			}
			if (c.getElapsedTime().asSeconds() > 13 && c.getElapsedTime().asSeconds() < 16) {
				if (camera.Pos.z > -.5)
					camera.Pos.z -= 1 * diffSeconds;
				if (camera.Pos.y < 1.5)
					camera.Pos.y += .3 * diffSeconds;
				if (camera.Front.z < 0)
					camera.Front.z += 2 * diffSeconds;
				if (camera.Front.x > 1)
					camera.Front.x -= 2 * diffSeconds;
			}
			if (c.getElapsedTime().asSeconds() > 16 && c.getElapsedTime().asSeconds() < 18) {
				if (camera.Front.x > .9) {
					camera.Front.x -= 2 * diffSeconds;
				}
				if (camera.Front.z < .3)
					camera.Front.z += 2 * diffSeconds;
			}
			if (c.getElapsedTime().asSeconds() > 18 && c.getElapsedTime().asSeconds() < 20) {
				camera.Pos.x += 5.5 * diffSeconds;
				camera.Pos.z += 1 * diffSeconds;
			}
			if (c.getElapsedTime().asSeconds() > 20 && c.getElapsedTime().asSeconds() < 21) {
				camera.Pos.y -= .5 * diffSeconds;
				camera.Front.y -= .5 * diffSeconds;
			}
			if (c.getElapsedTime().asSeconds() > 23 && c.getElapsedTime().asSeconds() < 25) {
				camera.Front.x -= .5 * diffSeconds;
				camera.Front.z -= .5 * diffSeconds;
				camera.Front.y += .4 * diffSeconds;
			}
			if (c.getElapsedTime().asSeconds() > 25 && c.getElapsedTime().asSeconds() < 26) {
				camera.Up.z -= 5 * diffSeconds;
				camera.Pos.y -= .5 * diffSeconds;

			}
			if (c.getElapsedTime().asSeconds() > 26) {
				boolscene = false;
				boolscene1 = true;
				CameraEnabled = true;
				mainShader.setUniform("spotLight[0].ambient", glm::vec3(0, 0, 0));
				mainShader.setUniform("spotLight[0].diffuse", glm::vec3(0, 0, 0));
				mainShader.setUniform("spotLight[0].specular", glm::vec3(0, 0, 0));
				mainShader.setUniform("spotLight[1].ambient", glm::vec3(0, 0, 0));
				mainShader.setUniform("spotLight[1].diffuse", glm::vec3(0, 0, 0));
				mainShader.setUniform("spotLight[1].specular", glm::vec3(0, 0, 0));
				FPS = true;

			}

			for (auto& animator : scene.animators) {
				animator.tick(diffSeconds);
			}
		}

		if (boolscene1) {
			glow0.tick(diffSeconds);
			auto& glowpos = glow0.getPosition();
			glow0.addForce(glm::vec3(0, -9.8f * glow0.getMass(), 0));
			mainShader.setUniform("pointLight.position", glowpos);
			mainShader.setUniform("pointLight.ambient", glm::vec3(.05f));
			mainShader.setUniform("pointLight.diffuse", glm::vec3(.8f));
			mainShader.setUniform("pointLight.specular", glm::vec3(.1f, .5f, .1f));
			mainShader.setUniform("pointLight.constant", 1.0f);
			mainShader.setUniform("pointLight.linear", 0.09f);
			mainShader.setUniform("pointLight.quadratic", 0.032f);
			//std::cout << "x: " << glowpos.x << "Y: " << glowpos.z << "Z: " << glowpos.z << "vel : " << glow0.getVelocity().y << "M: " << glow0.getMass() << std::endl;
			if (c.getElapsedTime().asSeconds() > 26 && c.getElapsedTime().asSeconds() < 26.1) {
				camera.Pos = glm::vec3(0, 8.5, 18);
				camera.Up = glm::vec3(0, 1, 0);
				camera.Front = glm::vec3(0, 0, 0);
				camera.Yaw = 270;
				camera.Pitch = 0;

			}
			if (glowpos.y < 1) {
				glow0.setPosition(glm::vec3(glowpos.x, 1,glowpos.z));
				glow0.setVelocity(glm::vec3(1, -1, 1)* glow0.getVelocity() * .7f);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::E)) {
				glow0.setOrientation(camera.Front);
				glow0.setPosition(camera.Pos);
				glow0.setVelocity(glm::vec3(camera.Front.x * 15, 10, camera.Front.z * 15));
			}
			

			//Big Room
			if (glowpos.x < -19.9 && glowpos.x > -21 && glowpos.z < 11.9 && glowpos.z > -52.1) {
				glow0.setPosition(glm::vec3(-19.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z > -52.1 && glowpos.z < -51 && glowpos.x < -19.9 && glowpos.x > -60.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, -52.1));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.x > -60.1 && glowpos.x < -59 && glowpos.z < 12.1 && glowpos.z > -52.1) {
				glow0.setPosition(glm::vec3(-60.1, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z > 11.9 && glowpos.z < 13 && glowpos.x < -59.9 && glowpos.x > -124.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 11.9));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.x < -123.9 && glowpos.x > -125 && glowpos.z < 11.9 && glowpos.z > -52.1) {
				glow0.setPosition(glm::vec3(-123.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z < -51.9 && glowpos.z > -53 && glowpos.x < -67.9 && glowpos.x > -124.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, -51.9));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.x < -67.9 && glowpos.x > -69 && glowpos.z < -51.9 && glowpos.z > -60.1) {
				glow0.setPosition(glm::vec3(-67.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z < -59.9 && glowpos.z > -61 && glowpos.x < -11.9 && glowpos.x > -68.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, -59.9));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.x > -12.1 && glowpos.x < -11 && glowpos.z < -19.9 && glowpos.z > -60.1) {
				glow0.setPosition(glm::vec3(-12.1, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			//Portal
			if (glowpos.z < -19.9 && glowpos.z > -21 && glowpos.x < 60.1 && glowpos.x > -12.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, -19.9));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.x < 60.1 && glowpos.x > 59 && glowpos.z < -19.9 && glowpos.z > -44.1) {
				glow0.setPosition(glm::vec3(60.1, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z > -44.1 && glowpos.z < -43 && glowpos.x < 60.1 && glowpos.x > 43.9) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, -44.1));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.x < 44.1 && glowpos.x > 43 && glowpos.z < -43.9 && glowpos.z > -84.1) {
				glow0.setPosition(glm::vec3(44.1, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z < -83.9 && glowpos.z > -85 && glowpos.x < 84.1 && glowpos.x > 43.9) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, -83.9));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.x > 83.9 && glowpos.x < 85 && glowpos.z < -43.9 && glowpos.z > -84.1) {
				glow0.setPosition(glm::vec3(83.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z > -44.1 && glowpos.z < -43 && glowpos.x < 84.1 && glowpos.x > 68.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, -44.1));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.x > 67.9 && glowpos.x < 69 && glowpos.z < -11.9 && glowpos.z > -44.1) {
				glow0.setPosition(glm::vec3(67.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z > -12.1 && glowpos.z < -11 && glowpos.x < 68.1 && glowpos.x > 19.9) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, -12.1));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			//L Hall
			if (glowpos.x > 19.9 && glowpos.x < 21 && glowpos.z < 52.1 && glowpos.z > -12.1) {
				glow0.setPosition(glm::vec3(19.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z < 52.1 && glowpos.z > 51 && glowpos.x < 52.1 && glowpos.x > 20.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 52.1));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.x > 51.9 && glowpos.x < 53 && glowpos.z < 60.1 && glowpos.z > 51.9) {
				glow0.setPosition(glm::vec3(51.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.z > 59.9 && glowpos.z < 61 && glowpos.x < 52.1 && glowpos.x > 11.9) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 59.9));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.x < 12.1 && glowpos.x > 11 && glowpos.z < 60.1 && glowpos.z > 19.9) {
				glow0.setPosition(glm::vec3(12.1, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1) * glow0.getVelocity() * .7f);
			}
			//Corner
			if (glowpos.z > 19.9 && glowpos.z < 21 && glowpos.x < 12.1 && glowpos.x >-33.9) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 19.9));
				glow0.setVelocity(glm::vec3(1, 1, -1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.x > -34.1 && glowpos.x < -33 && glowpos.z < 52.1 && glowpos.z > 20.1) {
				glow0.setPosition(glm::vec3(-34.1, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.z < 52.1 && glowpos.z > 51 && glowpos.x < -17.9 && glowpos.x >-34.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 52.1));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.x < -17.9 && glowpos.x > -19 && glowpos.z < 52.1 && glowpos.z >35.9) {
				glow0.setPosition(glm::vec3(-17.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z > 35.9 && glowpos.z < 37 && glowpos.x < -17.9 && glowpos.x >-32.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 35.9));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.x < -31.9 && glowpos.x > -33 && glowpos.z < 36.1 && glowpos.z >29.9) {
				glow0.setPosition(glm::vec3(-31.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z < 29.9 && glowpos.z > 29 && glowpos.x < -9.9 && glowpos.x > -32.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 29.9));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.x > -10.1 && glowpos.x < -9 && glowpos.z < 60.1 && glowpos.z >28.9) {
				glow0.setPosition(glm::vec3(-10.1, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z > 59.9 && glowpos.z < 61 && glowpos.x < -9.9 && glowpos.x >-42.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 59.9));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.x < -41.9 && glowpos.x > -43 && glowpos.z < 60.1 && glowpos.z >-28.1) {
				glow0.setPosition(glm::vec3(-41.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z < -27.9 && glowpos.z > -29 && glowpos.x < -33.9 && glowpos.x >-42.1) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, -27.9));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.x > -34.1 && glowpos.x < -33 && glowpos.z < 12.1 && glowpos.z >-27.9) {
				glow0.setPosition(glm::vec3(-34.1, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z < 12.1 && glowpos.z > 11 && glowpos.x < -19.9 && glowpos.x >-34.9) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 12.1));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			//Hole
			if (glowpos.x > 109.9 && glowpos.x < 111 && glowpos.z < 60.1 && glowpos.z >27.9) {
				glow0.setPosition(glm::vec3(109.9, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.z > 59.9 && glowpos.z < 61 && glowpos.x < 110.1 && glowpos.x >79.9) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 59.9));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			if (glowpos.x < 80.1 && glowpos.x > 79 && glowpos.z < 60.1 && glowpos.z >27.9) {
				glow0.setPosition(glm::vec3(80.1, glowpos.y, glowpos.z));
				glow0.setVelocity(glm::vec3(-1, 1, 1)* glow0.getVelocity() * .7f);
			}
			if (glowpos.z < 28.1 && glowpos.z > 27 && glowpos.x < 110.1 && camera.Pos.x >79.9) {
				glow0.setPosition(glm::vec3(glowpos.x, glowpos.y, 28.1));
				glow0.setVelocity(glm::vec3(1, 1, -1) * glow0.getVelocity() * .7f);
			}
			if (camera.Pos.z > 32 && camera.Pos.z < 40 && camera.Pos.x >90 && camera.Pos.x < 98) {
				car0 = true;
				scene1.objects[3].setPosition(glm::vec3(0, -6, 0));
			}
			//tele holes
			if (camera.Pos.z > -78 && camera.Pos.z < -68 && camera.Pos.x > 46 && camera.Pos.x < 56) {
				camera.Pos = glm::vec3(0, 8.5, 18);
			}
			if (camera.Pos.z > -78 && camera.Pos.z < -68 && camera.Pos.x > 59 && camera.Pos.x < 69) {
				camera.Pos = glm::vec3(0, 8.5, 18);
			}
			if (camera.Pos.z > -78 && camera.Pos.z < -68 && camera.Pos.x > 71 && camera.Pos.x < 80) {

			}


			//Big Room
			if (camera.Pos.x < -19.9 && camera.Pos.x > -21 && camera.Pos.z < 11.9 && camera.Pos.z > -52.1) {
				camera.Pos.x = -19.9;
			}
			if (camera.Pos.z > -52.1 && camera.Pos.z < -51 && camera.Pos.x < -19.9 && camera.Pos.x > -60.1) {
				camera.Pos.z = -52.1;
			}
			if (camera.Pos.x > -60.1 && camera.Pos.x < -59 && camera.Pos.z < 12.1 && camera.Pos.z > -52.1) {
				camera.Pos.x = -60.1;//1
			}
			if (camera.Pos.z > 11.9 && camera.Pos.z < 13 && camera.Pos.x < -59.9 && camera.Pos.x > -124.1) {
				camera.Pos.z = 11.9;//2
			}
			if (camera.Pos.x < -123.9 && camera.Pos.x > -125 && camera.Pos.z < 11.9 && camera.Pos.z > -52.1) {
				camera.Pos.x = -123.9;//3
			}
			if (camera.Pos.z < -51.9 && camera.Pos.z > -53 && camera.Pos.x < -67.9 && camera.Pos.x > -124.1) {
				camera.Pos.z = -51.9;//4
			}
			if (camera.Pos.x < -67.9 && camera.Pos.x > -69 && camera.Pos.z < -51.9 && camera.Pos.z > -60.1) {
				camera.Pos.x = -67.9;
			}
			if (camera.Pos.z < -59.9 && camera.Pos.z > -61 && camera.Pos.x < -11.9 && camera.Pos.x > -68.1) {
				camera.Pos.z = -59.9;
			}
			if (camera.Pos.x > -12.1 && camera.Pos.x < -11 && camera.Pos.z < -19.9 && camera.Pos.z > -60.1) {
				camera.Pos.x = -12.1;
			}
			//Portal
			if (camera.Pos.z < -19.9 && camera.Pos.z > -21 && camera.Pos.x < 60.1 && camera.Pos.x > -12.1) {
				camera.Pos.z = -19.9;
			}
			if (camera.Pos.x < 60.1 && camera.Pos.x > 59 && camera.Pos.z < -19.9 && camera.Pos.z > -44.1) {
				camera.Pos.x = 60.1;
			}
			if (camera.Pos.z > -44.1 && camera.Pos.z < -43 && camera.Pos.x < 60.1 && camera.Pos.x > 43.9) {
				camera.Pos.z = -44.1;
			}
			if (camera.Pos.x < 44.1 && camera.Pos.x > 43 && camera.Pos.z < -43.9 && camera.Pos.z > -84.1) {
				camera.Pos.x = 44.1;
			}
			if (camera.Pos.z < -83.9 && camera.Pos.z > -85 && camera.Pos.x < 84.1 && camera.Pos.x > 43.9) {
				camera.Pos.z = -83.9;
			}
			if (camera.Pos.x > 83.9 && camera.Pos.x < 85 && camera.Pos.z < -43.9 && camera.Pos.z > -84.1) {
				camera.Pos.x = 83.9;
			}
			if (camera.Pos.z > -44.1 && camera.Pos.z < -43 && camera.Pos.x < 84.1 && camera.Pos.x > 68.1) {
				camera.Pos.z = -44.1;
			}
			if (camera.Pos.x > 67.9 && camera.Pos.x < 69 && camera.Pos.z < -11.9 && camera.Pos.z > -44.1) {
				camera.Pos.x = 67.9;
			}
			if (camera.Pos.z > -12.1 && camera.Pos.z < -11 && camera.Pos.x < 68.1 && camera.Pos.x > 19.9) {
				camera.Pos.z = -12.1;
			}
			//L Hall
			if (camera.Pos.x > 19.9 && camera.Pos.x < 21 && camera.Pos.z < 52.1 && camera.Pos.z > -12.1) {
				camera.Pos.x = 19.9;
			}
			if (camera.Pos.z < 52.1 && camera.Pos.z > 51 && camera.Pos.x < 52.1 && camera.Pos.x > 20.1) {
				camera.Pos.z = 52.1;
			}
			if (camera.Pos.x > 51.9 && camera.Pos.x < 53 && camera.Pos.z < 60.1 && camera.Pos.z > 51.9) {
				camera.Pos.x = 51.9;
			}
			if (camera.Pos.z > 59.9 && camera.Pos.z < 61 && camera.Pos.x < 52.1 && camera.Pos.x > 11.9) {
				camera.Pos.z = 59.9;
			}
			if (camera.Pos.x < 12.1 && camera.Pos.x > 11 && camera.Pos.z < 60.1 && camera.Pos.z > 19.9) {
				camera.Pos.x = 12.1;
			}
			//Corner
			if (camera.Pos.z > 19.9 && camera.Pos.z < 21 && camera.Pos.x < 12.1 && camera.Pos.x >-33.9) {
				camera.Pos.z = 19.9;
			}
			if (camera.Pos.x > -34.1 && camera.Pos.x < -33 && camera.Pos.z < 52.1 && camera.Pos.z > 20.1) {
				camera.Pos.x = -34.1;
			}
			if (camera.Pos.z < 52.1 && camera.Pos.z > 51 && camera.Pos.x < -17.9 && camera.Pos.x >-34.1) {
				camera.Pos.z = 52.1;
			}
			if (camera.Pos.x < -17.9 && camera.Pos.x > -19 && camera.Pos.z < 52.1 && camera.Pos.z >35.9) {
				camera.Pos.x = -17.9;
			}
			if (camera.Pos.z > 35.9 && camera.Pos.z < 37 && camera.Pos.x < -17.9 && camera.Pos.x >-32.1) {
				camera.Pos.z = 35.9;
			}
			if (camera.Pos.x < -31.9 && camera.Pos.x > -33 && camera.Pos.z < 36.1 && camera.Pos.z >29.9) {
				camera.Pos.x = -31.9;
			}
			if (camera.Pos.z < 29.9 && camera.Pos.z > 29 && camera.Pos.x < -9.9 && camera.Pos.x > -32.1) {
				camera.Pos.z = 29.9;
			}
			if (camera.Pos.x > -10.1 && camera.Pos.x < -9 && camera.Pos.z < 60.1 && camera.Pos.z >28.9) {
				camera.Pos.x = -10.1;
			}
			if (camera.Pos.z > 59.9 && camera.Pos.z < 61 && camera.Pos.x < -9.9 && camera.Pos.x >-42.1) {
				camera.Pos.z = 59.9;
			}
			if (camera.Pos.x < -41.9 && camera.Pos.x > -43 && camera.Pos.z < 60.1 && camera.Pos.z >-28.1) {
				camera.Pos.x = -41.9;
			}
			if (camera.Pos.z < -27.9 && camera.Pos.z > -29 && camera.Pos.x < -33.9 && camera.Pos.x >-42.1) {
				camera.Pos.z = -27.9;
			}
			if (camera.Pos.x > -34.1 && camera.Pos.x < -33 && camera.Pos.z < 12.1 && camera.Pos.z >-27.9) {
				camera.Pos.x = -34.1;
			}
			if (camera.Pos.z < 12.1 && camera.Pos.z > 11 && camera.Pos.x < -19.9 && camera.Pos.x >-34.9) {
				camera.Pos.z = 12.1;
			}
			//Hole
			if (camera.Pos.x > 109.9 && camera.Pos.x < 111 && camera.Pos.z < 60.1 && camera.Pos.z >27.9) {
				camera.Pos.x = 109.9;
			}
			if (camera.Pos.z > 59.9 && camera.Pos.z < 61 && camera.Pos.x < 110.1 && camera.Pos.x >79.9) {
				camera.Pos.z = 59.9;
			}
			if (camera.Pos.x < 80.1 && camera.Pos.x > 79 && camera.Pos.z < 60.1 && camera.Pos.z >27.9) {	
				camera.Pos.x = 80.1;
			}
			if (camera.Pos.z < 28.1 && camera.Pos.z > 27 && camera.Pos.x < 110.1 && camera.Pos.x >79.9) {
				camera.Pos.z = 28.1;
			}

		}

			// Clear the OpenGL "context".
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Render each object in the scene.
			if (boolscene) {
				for (auto& obj : scene.objects) {
					obj.render(window, mainShader);
				}
			}
			if (boolscene1) {
				for (auto& obj : scene1.objects) {
					obj.render(window, mainShader);
				}
			}
			//std::cout << 1 / diff.asSeconds() << " FPS " << std::endl;
			window.display();
		}

		return 0;
	}



