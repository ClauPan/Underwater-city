#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"
#include "Objects\bubbles.h"
#include <time.h>

void processKeyboardInput();

struct bubbles_struct {
	float y;
	float Ox;
	float Oy;
};

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Window window("Game Engine", 800, 800);
Camera camera;

glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-180.0f, 100.0f, -200.0f);

std::vector<Bubbles> animation;
float angleOx, angleOy, angleOz;
float opacity = 3.5f;
float movment = 3.0f;
float rotation = 4.9f;
bubbles_struct bb1, bb2; //bb sunt balonasele

bool ON = false;
bool startGame = false;
bool settings = false;
//FILE READING
std::vector<std::string> Text;

void OBJ(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, Shader shader, glm::mat4 matrix) {
	GLuint matrixID = glGetUniformLocation(shader.getId(), "MVP");
	GLuint modelMatrixID = glGetUniformLocation(shader.getId(), "model");;
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * matrix;
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &matrix[0][0]);
	glUniform3f(glGetUniformLocation(shader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(shader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
}

Bubbles ANIMATION(Mesh bubbles, Shader shaderBubbles, float positionX, bubbles_struct bb) {
	Bubbles bubblesRight;
	bubblesRight.setMatrix(glm::translate(bubblesRight.getMatrix(), camera.getCameraPosition()));
	bubblesRight.setMatrix(glm::rotate(bubblesRight.getMatrix(), bb.Ox, glm::vec3(1, 0, 0)));
	bubblesRight.setMatrix(glm::rotate(bubblesRight.getMatrix(), bb.Oy, glm::vec3(0, 1, 0)));
	bubblesRight.setMatrix(glm::translate(bubblesRight.getMatrix(), glm::vec3(positionX, bb.y, -5.0f)));
	bubblesRight.setMatrix(glm::scale(bubblesRight.getMatrix(), glm::vec3(0.03f, 0.03f, 0.03f)));
	return bubblesRight;
}

//TEXT STYLER
void TextCenter(const char* text, int indice, bool startMiddle, int variable, bool variableSent)
{
	if (startMiddle == true && variableSent == false) {
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text).x) / 2.f);
		ImGui::SetCursorPosY((ImGui::GetWindowHeight() + 100 * indice) / 4.f);
		ImGui::Text(text);
	}
	if (variableSent == true && startMiddle == false) {
		ImGui::SameLine(0.0f, 50.0f);
		ImGui::Text("%s   %d", text, variable);
	}
	if (variableSent == false && startMiddle == false) {
		ImGui::SetCursorPosX((ImGui::GetWindowWidth()) / 4.f);
		ImGui::Text(text);
	}
	if (variableSent == true && startMiddle == true) {
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(text).x) / 2.f);
		ImGui::SetCursorPosY((ImGui::GetWindowHeight() + 100 * indice) / 4.f);
		ImGui::Text("%s   %d", text, variable);
	}
}
void READFILE(std::string fileName) {
	// Create a text string, which is used to output the text file
	std::string line;

	// Read from the text file
	std::ifstream File(fileName);

	// Use a while loop together with the getline() function to read the file line by line
	while (std::getline(File, line)) {
		Text.push_back(line);
	}

	// Close the file
	File.close();
}

int main() {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 400");
	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);



	//building and compiling shader program
	Shader shaderBubbles("Shaders/bubbles_vs.glsl", "Shaders/bubbles_fs.glsl");
	Shader shaderTerrain("Shaders/terrain_vs.glsl", "Shaders/terrain_fs.glsl");
	Shader shaderLight("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");
	Shader shaderWater("Shaders/water_vs.glsl", "Shaders/water_fs.glsl");
	Shader shaderPedestal("Shaders/pedestal_vs.glsl", "Shaders/pedestal_fs.glsl");
	Shader shaderDragonTempleRuin("Shaders/dragonRuin_vs.glsl", "Shaders/dragonRuin_fs.glsl");
	Shader shaderRock2("Shaders/rock2_vs.glsl", "Shaders/rock2_fs.glsl");
	Shader shaderRock3("Shaders/rock3_vs.glsl", "Shaders/rock3_fs.glsl");
	Shader shaderRock4("Shaders/rock4_vs.glsl", "Shaders/rock4_fs.glsl");
	Shader shaderRock5("Shaders/rock5_vs.glsl", "Shaders/rock5_fs.glsl");
	Shader shaderArchway1("Shaders/archway1_vs.glsl", "Shaders/archway1_fs.glsl");
	Shader shaderSeaweed("Shaders/seaweed1_vs.glsl", "Shaders/seaweed1_fs.glsl");
	Shader shaderSeaweed2("Shaders/seaweed2_vs.glsl", "Shaders/seaweed2_fs.glsl");
	Shader shaderSeaweed3("Shaders/seaweed3_vs.glsl", "Shaders/seaweed3_fs.glsl");
	Shader shaderSeaweed4("Shaders/seaweed4_vs.glsl", "Shaders/seaweed4_fs.glsl");
	Shader shaderRuine1("Shaders/ruine1_vs.glsl", "Shaders/ruine1_fs.glsl");
	Shader shaderRuine2("Shaders/ruine2_vs.glsl", "Shaders/ruine2_fs.glsl");
	Shader shaderSmallRock1("Shaders/smallRock1_vs.glsl", "Shaders/smallRock1_fs.glsl");
	Shader shaderSmallRock2("Shaders/smallRock1_vs.glsl", "Shaders/smallRock1_fs.glsl");
	Shader shaderSmallRock3("Shaders/smallRock1_vs.glsl", "Shaders/smallRock1_fs.glsl");
	Shader shaderSmallRock4("Shaders/smallRock1_vs.glsl", "Shaders/smallRock1_fs.glsl");
	Shader shaderPilares("Shaders/pilares_vs.glsl", "Shaders/pilares_fs.glsl");
	Shader shaderSeaweedT1("Shaders/seaweedT1_vs.glsl", "Shaders/seaweedT1_fs.glsl");
	Shader shaderSeaweedT2("Shaders/seaweedT2_vs.glsl", "Shaders/seaweedT2_fs.glsl");
	Shader shaderSeaweedT3("Shaders/seaweedT3_vs.glsl", "Shaders/seaweedT3_fs.glsl");
	Shader shaderSeaweedT4("Shaders/seaweedT4_vs.glsl", "Shaders/seaweedT4_fs.glsl");
	Shader shaderSeaweedT5("Shaders/seaweedT5_vs.glsl", "Shaders/seaweedT5_fs.glsl");
	Shader shaderSeaweedT6("Shaders/seaweedT6_vs.glsl", "Shaders/seaweedT6_fs.glsl");
	Shader shaderSeaweedT7("Shaders/seaweedT7_vs.glsl", "Shaders/seaweedT7_fs.glsl");
	Shader shaderSeaweedT8("Shaders/seaweedT8_vs.glsl", "Shaders/seaweedT8_fs.glsl");
	Shader shaderSeashell("Shaders/seashell_vs.glsl", "Shaders/seashell_fs.glsl");
	Shader shaderStarfish("Shaders/starfish_vs.glsl", "Shaders/starfish_fs.glsl");

	//Textures
	GLuint textureBubbles = loadBMP("Resources/Textures/blue.bmp");
	GLuint textureWater = loadBMP("Resources/Textures/back2.bmp");
	GLuint textureRock = loadBMP("Resources/Textures/SandG_001.bmp");
	GLuint texturePedestal = loadBMP("Resources/Textures/BaseColorPedestal.bmp");
	GLuint textureDragonTempleRuin = loadBMP("Resources/Textures/BaseColorPedestal.bmp");
	GLuint textureRock2 = loadBMP("Resources/Textures/Rock_2_Base_Color.bmp");
	GLuint textureRock3 = loadBMP("Resources/Textures/Rock_2_Base_Color.bmp");
	GLuint textureRock4 = loadBMP("Resources/Textures/Rock_2_Base_Color.bmp");
	GLuint textureRock5 = loadBMP("Resources/Textures/Rock_2_Base_Color.bmp");
	GLuint textureArchway1 = loadBMP("Resources/Textures/archway_3_Base_Color.bmp");
	GLuint textureSeaweed = loadBMP("Resources/Textures/seaweed_diffuse.bmp");
	GLuint textureSeaweed2 = loadBMP("Resources/Textures/REDALGAE26.bmp");
	GLuint textureSeaweed3 = loadBMP("Resources/Textures/GREENALGAE10.bmp");
	GLuint textureSeaweed4 = loadBMP("Resources/Textures/REDALGAE27.bmp");
	GLuint textureRuine1 = loadBMP("Resources/Textures/T_AncientRuins_Props_D.bmp");
	GLuint textureRuine2 = loadBMP("Resources/Textures/T_AncientRuins_Props_D.bmp");
	GLuint textureSmallRock1 = loadBMP("Resources/Textures/T_AncientRuins_Rocks_D.bmp");
	GLuint textureSmallRock2 = loadBMP("Resources/Textures/T_AncientRuins_Rocks_D.bmp");
	GLuint textureSmallRock3 = loadBMP("Resources/Textures/T_AncientRuins_Rocks_D.bmp");
	GLuint textureSmallRock4 = loadBMP("Resources/Textures/T_AncientRuins_Rocks_D.bmp");
	GLuint texturePilares = loadBMP("Resources/Textures/T_AncientRuins_Props_D.bmp");
	GLuint textureSeaweedT1 = loadBMP("Resources/Textures/GREENALGAE12.bmp");
	GLuint textureSeaweedT2 = loadBMP("Resources/Textures/GREENALGAE16.bmp");
	GLuint textureSeaweedT3 = loadBMP("Resources/Textures/GIANTKELP22.bmp");
	GLuint textureSeaweedT4 = loadBMP("Resources/Textures/GIANTKELP21.bmp");
	GLuint textureSeaweedT5 = loadBMP("Resources/Textures/GIANTKELP10.bmp");
	GLuint textureSeaweedT6 = loadBMP("Resources/Textures/GIANTKELP11.bmp");
	GLuint textureSeaweedT7 = loadBMP("Resources/Textures/GIANTKELP18.bmp");
	GLuint textureSeaweedT8 = loadBMP("Resources/Textures/GIANTKELP1.bmp");
	GLuint textureSeashell = loadBMP("Resources/Textures/Material,Texture.bmp");
	GLuint textureStarfish = loadBMP("Resources/Textures/REDALGAE12.bmp");

	glEnable(GL_DEPTH_TEST);

	//Test custom mesh loading
	std::vector<Vertex> vert;
	vert.push_back(Vertex());
	vert[0].pos = glm::vec3(10.5f, 10.5f, 0.0f);
	vert[0].textureCoords = glm::vec2(1.0f, 1.0f);

	vert.push_back(Vertex());
	vert[1].pos = glm::vec3(10.5f, -10.5f, 0.0f);
	vert[1].textureCoords = glm::vec2(1.0f, 0.0f);

	vert.push_back(Vertex());
	vert[2].pos = glm::vec3(-10.5f, -10.5f, 0.0f);
	vert[2].textureCoords = glm::vec2(0.0f, 0.0f);

	vert.push_back(Vertex());
	vert[3].pos = glm::vec3(-10.5f, 10.5f, 0.0f);
	vert[3].textureCoords = glm::vec2(0.0f, 1.0f);

	vert[0].normals = glm::normalize(glm::cross(vert[1].pos - vert[0].pos, vert[3].pos - vert[0].pos));
	vert[1].normals = glm::normalize(glm::cross(vert[2].pos - vert[1].pos, vert[0].pos - vert[1].pos));
	vert[2].normals = glm::normalize(glm::cross(vert[3].pos - vert[2].pos, vert[1].pos - vert[2].pos));
	vert[3].normals = glm::normalize(glm::cross(vert[0].pos - vert[3].pos, vert[2].pos - vert[3].pos));

	std::vector<int> ind = { 0, 1, 3, 1, 2, 3 };

	bb1.y = -20.0f;
	bb2.y = -15.0f;
	bb1.Ox = angleOx;
	bb2.Ox = angleOx;
	bb1.Oy = angleOy;
	bb2.Oy = angleOy;

	std::vector<Texture> texturesRock;
	texturesRock.push_back(Texture());
	texturesRock[0].id = textureRock;
	texturesRock[0].type = "texture_diffuse";

	std::vector<Texture> texturesBubbles;
	texturesBubbles.push_back(Texture());
	texturesBubbles[0].id = textureBubbles;
	texturesBubbles[0].type = "texture_diffuse";

	std::vector<Texture> texturesWater;
	texturesWater.push_back(Texture());
	texturesWater[0].id = textureWater;
	texturesWater[0].type = "texture_diffuse";

	//seaweed

	std::vector<Texture> texturesSeaweed;
	texturesSeaweed.push_back(Texture());
	texturesSeaweed[0].id = textureSeaweed;
	texturesSeaweed[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweed2;
	texturesSeaweed2.push_back(Texture());
	texturesSeaweed2[0].id = textureSeaweed2;
	texturesSeaweed2[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweed3;
	texturesSeaweed3.push_back(Texture());
	texturesSeaweed3[0].id = textureSeaweed3;
	texturesSeaweed3[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweed4;
	texturesSeaweed4.push_back(Texture());
	texturesSeaweed4[0].id = textureSeaweed4;
	texturesSeaweed4[0].type = "texture_diffuse";

	//seaweedT1

	std::vector<Texture> texturesSeaweedT1;
	texturesSeaweedT1.push_back(Texture());
	texturesSeaweedT1[0].id = textureSeaweedT1;
	texturesSeaweedT1[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweedT2;
	texturesSeaweedT2.push_back(Texture());
	texturesSeaweedT2[0].id = textureSeaweedT2;
	texturesSeaweedT2[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweedT3;
	texturesSeaweedT3.push_back(Texture());
	texturesSeaweedT3[0].id = textureSeaweedT3;
	texturesSeaweedT3[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweedT4;
	texturesSeaweedT4.push_back(Texture());
	texturesSeaweedT4[0].id = textureSeaweedT4;
	texturesSeaweedT4[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweedT5;
	texturesSeaweedT5.push_back(Texture());
	texturesSeaweedT5[0].id = textureSeaweedT5;
	texturesSeaweedT5[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweedT6;
	texturesSeaweedT6.push_back(Texture());
	texturesSeaweedT6[0].id = textureSeaweedT6;
	texturesSeaweedT6[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweedT7;
	texturesSeaweedT7.push_back(Texture());
	texturesSeaweedT7[0].id = textureSeaweedT7;
	texturesSeaweedT7[0].type = "texture_diffuse";

	std::vector<Texture> texturesSeaweedT8;
	texturesSeaweedT8.push_back(Texture());
	texturesSeaweedT8[0].id = textureSeaweedT8;
	texturesSeaweedT8[0].type = "texture_diffuse";

	//pedestal

	std::vector<Texture> texturesPedestal;
	texturesPedestal.push_back(Texture());
	texturesPedestal[0].id = texturePedestal;
	texturesPedestal[0].type = "texture_diffuse";

	//dragon temple

	std::vector<Texture> texturesDragonTempleRuin;
	texturesDragonTempleRuin.push_back(Texture());
	texturesDragonTempleRuin[0].id = textureDragonTempleRuin;
	texturesDragonTempleRuin[0].type = "texture_diffuse";

	//big rocks

	std::vector<Texture> texturesRock2;
	texturesRock2.push_back(Texture());
	texturesRock2[0].id = textureRock2;
	texturesRock2[0].type = "texture_diffuse";

	std::vector<Texture> texturesRock3;
	texturesRock3.push_back(Texture());
	texturesRock3[0].id = textureRock3;
	texturesRock3[0].type = "texture_diffuse";


	std::vector<Texture> texturesRock4;
	texturesRock4.push_back(Texture());
	texturesRock4[0].id = textureRock4;
	texturesRock4[0].type = "texture_diffuse";

	std::vector<Texture> texturesRock5;
	texturesRock5.push_back(Texture());
	texturesRock5[0].id = textureRock5;
	texturesRock5[0].type = "texture_diffuse";


	//small rocks

	std::vector<Texture> texturesSmallRock1;
	texturesSmallRock1.push_back(Texture());
	texturesSmallRock1[0].id = textureSmallRock1;
	texturesSmallRock1[0].type = "texture_diffuse";

	std::vector<Texture> texturesSmallRock2;
	texturesSmallRock2.push_back(Texture());
	texturesSmallRock2[0].id = textureSmallRock2;
	texturesSmallRock2[0].type = "texture_diffuse";

	std::vector<Texture> texturesSmallRock3;
	texturesSmallRock3.push_back(Texture());
	texturesSmallRock3[0].id = textureSmallRock3;
	texturesSmallRock3[0].type = "texture_diffuse";

	std::vector<Texture> texturesSmallRock4;
	texturesSmallRock4.push_back(Texture());
	texturesSmallRock4[0].id = textureSmallRock4;
	texturesSmallRock4[0].type = "texture_diffuse";

	//archways

	std::vector<Texture> texturesArchway1;
	texturesArchway1.push_back(Texture());
	texturesArchway1[0].id = textureArchway1;
	texturesArchway1[0].type = "texture_diffuse";


	//ruine

	std::vector<Texture> texturesRuine1;
	texturesRuine1.push_back(Texture());
	texturesRuine1[0].id = textureRuine1;
	texturesRuine1[0].type = "texture_diffuse";

	std::vector<Texture> texturesRuine2;
	texturesRuine2.push_back(Texture());
	texturesRuine2[0].id = textureRuine2;
	texturesRuine2[0].type = "texture_diffuse";

	//pilares

	std::vector<Texture> texturesPilares;
	texturesPilares.push_back(Texture());
	texturesPilares[0].id = texturePilares;
	texturesPilares[0].type = "texture_diffuse";


	//seashell

	std::vector<Texture> texturesSeashell;
	texturesSeashell.push_back(Texture());
	texturesSeashell[0].id = textureSeashell;
	texturesSeashell[0].type = "texture_diffuse";

	//starfish

	std::vector<Texture> texturesStarfish;
	texturesStarfish.push_back(Texture());
	texturesStarfish[0].id = textureStarfish;
	texturesStarfish[0].type = "texture_diffuse";

	Mesh mesh(vert, ind, texturesBubbles);

	MeshLoaderObj loader;

	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh bubbles = loader.loadObj("Resources/Models/bubbles.obj", texturesBubbles);
	Mesh terrain = loader.loadObj("Resources/Models/plane1.obj", texturesRock);
	Mesh sphere = loader.loadObj("Resources/Models/sphere.obj", texturesWater);
	Mesh pedestal = loader.loadObj("Resources/Models/pedestal.obj", texturesPedestal);
	Mesh DragonTempleRuin = loader.loadObj("Resources/Models/DragonTempleRuins.obj", texturesDragonTempleRuin);
	Mesh Rock2 = loader.loadObj("Resources/Models/Rock_2.obj", texturesRock2);
	Mesh Rock3 = loader.loadObj("Resources/Models/Rock_3.obj", texturesRock3);
	Mesh Rock4 = loader.loadObj("Resources/Models/Rock_4.obj", texturesRock4);
	Mesh Rock5 = loader.loadObj("Resources/Models/Rock_5.obj", texturesRock5);
	Mesh Archway1 = loader.loadObj("Resources/Models/archway_1.obj", texturesArchway1);
	Mesh seaweed = loader.loadObj("Resources/Models/seaweed1.obj", texturesSeaweed);
	Mesh seaweed2 = loader.loadObj("Resources/Models/seaweed2.obj", texturesSeaweed2);
	Mesh seaweed3 = loader.loadObj("Resources/Models/seaweed3.obj", texturesSeaweed3);
	Mesh seaweed4 = loader.loadObj("Resources/Models/seaweed4.obj", texturesSeaweed4);
	Mesh ruine1 = loader.loadObj("Resources/Models/ruine1.obj", texturesRuine1);
	Mesh ruine2 = loader.loadObj("Resources/Models/ruine2.obj", texturesRuine2);
	Mesh SmallRock1 = loader.loadObj("Resources/Models/SmallRock1.obj", texturesSmallRock1);
	Mesh SmallRock2 = loader.loadObj("Resources/Models/SmallRock2.obj", texturesSmallRock2);
	Mesh SmallRock3 = loader.loadObj("Resources/Models/SmallRock3.obj", texturesSmallRock3);
	Mesh SmallRock4 = loader.loadObj("Resources/Models/SmallRock4.obj", texturesSmallRock4);
	Mesh pilares = loader.loadObj("Resources/Models/Pilares.obj", texturesPilares);
	Mesh seaweedT1 = loader.loadObj("Resources/Models/seaweedT1.obj", texturesSeaweedT1);
	Mesh seaweedT2 = loader.loadObj("Resources/Models/seaweedT2.obj", texturesSeaweedT2);
	Mesh seaweedT3 = loader.loadObj("Resources/Models/seaweedT3.obj", texturesSeaweedT3);
	Mesh seaweedT4 = loader.loadObj("Resources/Models/seaweedT4.obj", texturesSeaweedT4);
	Mesh seaweedT5 = loader.loadObj("Resources/Models/seaweedT5.obj", texturesSeaweedT5);
	Mesh seaweedT6 = loader.loadObj("Resources/Models/seaweedT6.obj", texturesSeaweedT6);
	Mesh seaweedT7 = loader.loadObj("Resources/Models/seaweedT7.obj", texturesSeaweedT7);
	Mesh seaweedT8 = loader.loadObj("Resources/Models/seaweedT8.obj", texturesSeaweedT8);
	Mesh seashell = loader.loadObj("Resources/Models/seashell.obj", texturesSeashell);
	Mesh starfish = loader.loadObj("Resources/Models/starfish.obj", texturesStarfish);

	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_ESCAPE) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyboardInput();

		//test mouse input
		if (window.isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			std::cout << "Pressing mouse button" << std::endl;
		}

		//// Code for the light ////
		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight(), 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());

		//Test for one Obj loading = light source
		GLuint lightMatrixID = glGetUniformLocation(shaderLight.getId(), "MVP");
		glm::mat4 matrixLight = glm::mat4(1.0);
		matrixLight = glm::translate(matrixLight, lightPos);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * matrixLight;
		glUniformMatrix4fv(lightMatrixID, 1, GL_FALSE, &MVP[0][0]);

		//////////////////EVERYTHING ELSE///////////////////////

		shaderTerrain.use();

		glm::mat4 matrixTerrain = glm::mat4(1.0);
		matrixTerrain = glm::translate(matrixTerrain, glm::vec3(0.0f, -20.0f, 0.0f));
		matrixTerrain = glm::scale(matrixTerrain, glm::vec3(1.5f, 1.5f, 1.5f));
		OBJ(ViewMatrix, ProjectionMatrix, shaderTerrain, matrixTerrain);

		terrain.draw(shaderTerrain);

		//seaweeds

		glm::vec3 position[] = { glm::vec3(0.0f, -18.0f, -10.0f),glm::vec3(40.0f, -18.0f, 120.0f),glm::vec3(76.0f, -18.0f, 154.0f) ,glm::vec3(45.0f, -18.0f, 12.0f), glm::vec3(-100.0f, -18.0f, 0.0f), glm::vec3(24.0f, -18.0f, 52.0f), glm::vec3(229.0f, -18.0f, -32.0f), glm::vec3(207.0f, -18.0f, -50.0f), glm::vec3(245.0f, -18.0f, -22.0f), glm::vec3(240.0f, -18.0f, -30.0f), glm::vec3(260.0f, -18.0f, 23.0f), glm::vec3(268.0f, -18.0f, 47.0f) };
		for (int i = 0; i < 12; i++) {
			shaderSeaweed.use();
			glm::mat4 matrixSeaweed = glm::mat4(1.0);
			matrixSeaweed = glm::translate(matrixSeaweed, position[i]);
			matrixSeaweed = glm::scale(matrixSeaweed, glm::vec3(7.0f, 7.0f, 7.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweed, matrixSeaweed);
			seaweed.draw(shaderSeaweed);
		}

		glm::vec3 position1[] = { glm::vec3(-20.0f, -18.0f, 40.0f),glm::vec3(-76.0f, -18.0f, 112.0f),glm::vec3(178.0f, -18.0f, 181.0f) ,glm::vec3(128.0f, -18.0f, 68.0f), glm::vec3(-57.0f, -18.0f, 83.0f), glm::vec3(-39.0f, -18.0f, 147.0f), glm::vec3(217.0f, -18.0f, 184.0f), glm::vec3(238.0f, -18.0f, 186.0f), glm::vec3(248.0f, -18.0f, 28.0f), glm::vec3(243.0f, -18.0f, 8.0f), glm::vec3(256.0f, -18.0f, 89.0f), glm::vec3(248.0f, -18.0f, 66.0f) };
		for (int i = 0; i < 12; i++) {
			shaderSeaweed2.use();
			glm::mat4 matrixSeaweed2 = glm::mat4(1.0);
			matrixSeaweed2 = glm::translate(matrixSeaweed2, position1[i]);
			matrixSeaweed2 = glm::scale(matrixSeaweed2, glm::vec3(7.0f, 7.0f, 7.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweed2, matrixSeaweed2);
			seaweed2.draw(shaderSeaweed2);
		}


		glm::vec3 position2[] = { glm::vec3(210.0f, -18.0f, 101.0f),glm::vec3(189.0f, -18.0f, 166.0f),glm::vec3(178.0f, -18.0f, 143.0f) ,glm::vec3(19.0f, -18.0f, 38.0f), glm::vec3(4.0f, -18.0f, 79.0f), glm::vec3(-29.0f, -18.0f, 130.0f), glm::vec3(199.0f, -18.0f, 156.0f), glm::vec3(221.0f, -18.0f, 125.0f), glm::vec3(188.0f, -18.0f, 20.0f), glm::vec3(174.0f, -18.0f, 1.0f), glm::vec3(270.0f, -18.0f, 32.0f), glm::vec3(199.0f, -18.0f, -27.0f) };
		for (int i = 0; i < 12; i++) {
			shaderSeaweed3.use();
			glm::mat4 matrixSeaweed3 = glm::mat4(1.0);
			matrixSeaweed3 = glm::translate(matrixSeaweed3, position2[i]);
			matrixSeaweed3 = glm::scale(matrixSeaweed3, glm::vec3(7.0f, 7.0f, 7.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweed3, matrixSeaweed3);
			seaweed3.draw(shaderSeaweed3);
		}

		glm::vec3 position7[] = { glm::vec3(176.0f, -18.0f, 132.0f),glm::vec3(68.0f, -18.0f, 125.0f),glm::vec3(80.0f, -18.0f, 199.0f) ,glm::vec3(-89.0f, -18.0f, 165.0f), glm::vec3(-45.0f, -18.0f, 53.0f), glm::vec3(-126.0f, -18.0f, 72.0f), glm::vec3(215.0f, -18.0f, 35.0f), glm::vec3(234.0f, -18.0f, 98.0f), glm::vec3(170.0f, -18.0f, 44.0f), glm::vec3(250.0f, -18.0f, -40.0f), glm::vec3(216.0f, -18.0f, 99.0f), glm::vec3(235.0f, -18.0f, 83.0f) };
		for (int i = 0; i < 12; i++) {
			shaderSeaweed4.use();
			glm::mat4 matrixSeaweed4 = glm::mat4(1.0);
			matrixSeaweed4 = glm::translate(matrixSeaweed4, position7[i]);
			matrixSeaweed4 = glm::scale(matrixSeaweed4, glm::vec3(7.0f, 7.0f, 7.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweed4, matrixSeaweed4);
			seaweed4.draw(shaderSeaweed4);
		}


		//seaweedT1

		glm::vec3 position13[] = { glm::vec3(0.0f, -18.0f, 383.0f),glm::vec3(40.0f, -18.0f, 400.0f),glm::vec3(20.0f, -18.0f, 321.0f),glm::vec3(150.0f, -18.0f, 384.0f), glm::vec3(65.0f, -18.0f, 374.0f), glm::vec3(289.0f, -18.0f, 364.0f), glm::vec3(-120.0f, -18.0f, 296.0f), glm::vec3(-98.0f, -18.0f, 268.0f), glm::vec3(234.0f, -18.0f, 213.0f), glm::vec3(246.0f, -18.0f, 194.0f), glm::vec3(120.0f, -18.0f, 242.0f), glm::vec3(91.0f, -18.0f, 238.0f), glm::vec3(29.0f, -18.0f, 264.0f),glm::vec3(25.0f, -18.0f, 238.0f) };
		for (int i = 0; i < 14; i++) {
			shaderSeaweedT1.use();
			glm::mat4 matrixSeaweedT1 = glm::mat4(1.0);
			matrixSeaweedT1 = glm::translate(matrixSeaweedT1, position13[i]);
			matrixSeaweedT1 = glm::scale(matrixSeaweedT1, glm::vec3(40.0f, 40.0f, 40.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweedT1, matrixSeaweedT1);
			seaweedT1.draw(shaderSeaweedT1);
		}

		glm::vec3 position14[] = { glm::vec3(-20.0f, -18.0f, 345.0f),glm::vec3(-145.0f, -18.0f, 420.0f),glm::vec3(-76.0f, -18.0f, 420.0f) ,glm::vec3(-100.0f, -18.0f, 394.0f), glm::vec3(290.0f, -18.0f, 339.0f), glm::vec3(300.0f, -18.0f, 381.0f), glm::vec3(-45.0f, -18.0f, 279.0f), glm::vec3(-88.0f, -18.0f, 284.0f), glm::vec3(55.0f, -18.0f, 238.0f), glm::vec3(267.0f, -18.0f, 233.0f), glm::vec3(63.0f, -18.0f, 178.0f), glm::vec3(82.0f, -18.0f, 277.0f), glm::vec3(24.0f, -18.0f, 219.0f), glm::vec3(32.0f, -18.0f, 276.0f) };
		for (int i = 0; i < 14; i++) {
			shaderSeaweedT2.use();
			glm::mat4 matrixSeaweedT2 = glm::mat4(1.0);
			matrixSeaweedT2 = glm::translate(matrixSeaweedT2, position14[i]);
			matrixSeaweedT2 = glm::scale(matrixSeaweedT2, glm::vec3(40.0f, 40.0f, 40.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweedT2, matrixSeaweedT2);
			seaweedT2.draw(shaderSeaweedT2);
		}

		glm::vec3 position15[] = { glm::vec3(198.0f, -18.0f, 370.0f),glm::vec3(123.0f, -18.0f, 389.0f),glm::vec3(144.0f, -18.0f, 394.0f) ,glm::vec3(130.0f, -18.0f, 378.0f), glm::vec3(210.0f, -18.0f, 335.0f), glm::vec3(87.0f, -18.0f, 307.0f), glm::vec3(-129.0f, -18.0f, 321.0f), glm::vec3(-61.0f, -18.0f, 223.0f), glm::vec3(264.0f, -18.0f, 188.0f), glm::vec3(258.0f, -18.0f, 179.0f), glm::vec3(109.0f, -18.0f, 298.0f), glm::vec3(126.0f, -18.0f, 288.0f), glm::vec3(36.0f, -18.0f, 257.0f), glm::vec3(14.0f, -18.0f, 281.0f) };
		for (int i = 0; i < 14; i++) {
			shaderSeaweedT3.use();
			glm::mat4 matrixSeaweedT3 = glm::mat4(1.0);
			matrixSeaweedT3 = glm::translate(matrixSeaweedT3, position15[i]);
			matrixSeaweedT3 = glm::scale(matrixSeaweedT3, glm::vec3(40.0f, 40.0f, 40.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweedT3, matrixSeaweedT3);
			seaweedT3.draw(shaderSeaweedT3);
		}

		glm::vec3 position16[] = { glm::vec3(-68.0f, -18.0f, 376.0f),glm::vec3(-59.0f, -18.0f, 368.0f),glm::vec3(167.0f, -18.0f, 362.0f) ,glm::vec3(5.0f, -18.0f, 356.0f), glm::vec3(222.0f, -18.0f, 377.0f), glm::vec3(39.0f, -18.0f, 367.0f), glm::vec3(-76.0f, -18.0f, 369.0f), glm::vec3(-38.0f, -18.0f, 254.0f),glm::vec3(48.0f, -18.0f, 277.0f), glm::vec3(244.0f, -18.0f, 259.0f), glm::vec3(62.0f, -18.0f, 267.0f), glm::vec3(119.0f, -18.0f, 274.0f), glm::vec3(17.0f, -18.0f, 261.0f), glm::vec3(19.0f, -18.0f, 285.0f) };
		for (int i = 0; i < 14; i++) {
			shaderSeaweedT4.use();
			glm::mat4 matrixSeaweedT4 = glm::mat4(1.0);
			matrixSeaweedT4 = glm::translate(matrixSeaweedT4, position16[i]);
			matrixSeaweedT4 = glm::scale(matrixSeaweedT4, glm::vec3(40.0f, 40.0f, 40.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweedT4, matrixSeaweedT4);
			seaweedT4.draw(shaderSeaweedT4);
		}

		glm::vec3 position17[] = { glm::vec3(-138.0f, -18.0f, 326.0f),glm::vec3(-149.0f, -18.0f, 388.0f),glm::vec3(-131.0f, -18.0f, 241.0f) ,glm::vec3(-111.0f, -18.0f, 272.0f), glm::vec3(-100.0f, -18.0f, 283.0f), glm::vec3(-104.0f, -18.0f, 236.0f), glm::vec3(-53.0f, -18.0f, 249.0f), glm::vec3(-65.0f, -18.0f, 219.0f), glm::vec3(59.0f, -18.0f, 301.0f), glm::vec3(320.0f, -18.0f, 328.0f), glm::vec3(91.0f, -18.0f, 249.0f),glm::vec3(84.0f, -18.0f, 233.0f) };
		for (int i = 0; i < 12; i++) {
			shaderSeaweedT5.use();
			glm::mat4 matrixSeaweedT5 = glm::mat4(1.0);
			matrixSeaweedT5 = glm::translate(matrixSeaweedT5, position17[i]);
			matrixSeaweedT5 = glm::scale(matrixSeaweedT5, glm::vec3(40.0f, 40.0f, 40.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweedT5, matrixSeaweedT5);
			seaweedT5.draw(shaderSeaweedT5);
		}

		glm::vec3 position18[] = { glm::vec3(-64.0f, -18.0f, 172.0f),glm::vec3(-47.0f, -18.0f, 199.0f),glm::vec3(-29.0f, -18.0f, 184.0f) ,glm::vec3(-138.0f, -18.0f, 176.0f), glm::vec3(-31.0f, -18.0f, 184.0f), glm::vec3(-37.0f, -18.0f, 192.0f), glm::vec3(-4.0f, -18.0f, 178.0f), glm::vec3(-45.0f, -18.0f, 198.0f), glm::vec3(53.0f, -18.0f, 337.0f), glm::vec3(332.0f, -18.0f, 340.0f), glm::vec3(66.0f, -18.0f, 236.0f), glm::vec3(58.0f, -18.0f, 219.0f) };
		for (int i = 0; i < 12; i++) {
			shaderSeaweedT6.use();
			glm::mat4 matrixSeaweedT6 = glm::mat4(1.0);
			matrixSeaweedT6 = glm::translate(matrixSeaweedT6, position18[i]);
			matrixSeaweedT6 = glm::scale(matrixSeaweedT6, glm::vec3(40.0f, 40.0f, 40.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweedT6, matrixSeaweedT6);
			seaweedT6.draw(shaderSeaweedT6);
		}

		glm::vec3 position19[] = { glm::vec3(160.0f, -18.0f, 389.0f),glm::vec3(167.0f, -18.0f, 378.0f),glm::vec3(159.0f, -18.0f, 369.0f) ,glm::vec3(168.0f, -18.0f, 389.0f), glm::vec3(145.0f, -18.0f, 358.0f), glm::vec3(174.0f, -18.0f, 366.0f), glm::vec3(179.0f, -18.0f, 298.0f), glm::vec3(163.0f, -18.0f, 373.0f), glm::vec3(39.0f, -18.0f, 382.0f), glm::vec3(158.0f, -18.0f, 258.0f), glm::vec3(101.0f, -18.0f, 266.0f), glm::vec3(106.0f, -18.0f, 275.0f) };
		for (int i = 0; i < 12; i++) {
			shaderSeaweedT7.use();
			glm::mat4 matrixSeaweedT7 = glm::mat4(1.0);
			matrixSeaweedT7 = glm::translate(matrixSeaweedT7, position19[i]);
			matrixSeaweedT7 = glm::scale(matrixSeaweedT7, glm::vec3(40.0f, 40.0f, 40.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweedT7, matrixSeaweedT7);
			seaweedT7.draw(shaderSeaweedT7);
		}

		glm::vec3 position20[] = { glm::vec3(149.0f, -18.0f, 288.0f),glm::vec3(177.0f, -18.0f, 222.0f),glm::vec3(188.0f, -18.0f, 228.0f) ,glm::vec3(194.0f, -18.0f, 237.0f), glm::vec3(200.0f, -18.0f, 205.0f), glm::vec3(191.0f, -18.0f, 216.0f), glm::vec3(184.0f, -18.0f, 279.0f), glm::vec3(45.0f, -18.0f, 314.0f), glm::vec3(177.0f, -18.0f, 323.0f), glm::vec3(79.0f, -18.0f, 332.0f), glm::vec3(110.0f, -18.0f, 211.0f),glm::vec3(90.0f, -18.0f, 143.0f) };
		for (int i = 0; i < 12; i++) {
			shaderSeaweedT8.use();
			glm::mat4 matrixSeaweedT8 = glm::mat4(1.0);
			matrixSeaweedT8 = glm::translate(matrixSeaweedT8, position20[i]);
			matrixSeaweedT8 = glm::scale(matrixSeaweedT8, glm::vec3(40.0f, 40.0f, 40.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeaweedT8, matrixSeaweedT8);
			seaweedT8.draw(shaderSeaweedT8);
		}


		//seashell

		glm::vec3 position21[] = { glm::vec3(35.0f, -18.0f, 145.0f),glm::vec3(67.0f, -18.0f, 187.0f),glm::vec3(99.0f, -18.0f, 201.0f), glm::vec3(100.0f, -18.0f, 100.0f),glm::vec3(128.0f, -18.0f, 288.0f),glm::vec3(-40.0f, -18.0f, 123.0f) };
		for (int i = 0; i < 6; i++) {
			shaderSeashell.use();
			glm::mat4 matrixSeashell = glm::mat4(1.0);
			matrixSeashell = glm::translate(matrixSeashell, position21[i]);
			matrixSeashell = glm::scale(matrixSeashell, glm::vec3(10.0f, 10.0f, 10.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSeashell, matrixSeashell);
			seashell.draw(shaderSeashell);
		}

		//starfish

		glm::vec3 position22[] = { glm::vec3(40.0f, -18.0f, 130.0f), glm::vec3(123.0f, -18.0f, 56.0f), glm::vec3(-10.0f, -18.0f, 23.0f), glm::vec3(145.0f, -18.0f, 45.0f), glm::vec3(6.0f, -18.0f, 136.0f), glm::vec3(129.0f, -18.0f, 78.0f) };
		for (int i = 0; i < 6; i++) {
			shaderStarfish.use();
			glm::mat4 matrixStarfish = glm::mat4(1.0);
			matrixStarfish = glm::translate(matrixStarfish, position22[i]);
			matrixStarfish = glm::scale(matrixStarfish, glm::vec3(20.0f, 20.0f, 20.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderStarfish, matrixStarfish);
			starfish.draw(shaderStarfish);
		}


		//pedestals

		glm::vec3 position3[] = { glm::vec3(0.0f, -18.0f, 75.0f),glm::vec3(150.0f, -18.0f, 246.0f) };
		for (int i = 0; i < 2; i++) {
			shaderPedestal.use();
			glm::mat4 matrixPedestal = glm::mat4(1.0);
			matrixPedestal = glm::translate(matrixPedestal, position3[i]);
			matrixPedestal = glm::scale(matrixPedestal, glm::vec3(7.0f, 7.0f, 7.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderPedestal, matrixPedestal);
			pedestal.draw(shaderPedestal);
		}

		//pilares

		glm::vec3 position12[] = { glm::vec3(-120.0f, -18.0f, -15.0f),glm::vec3(195.0f, -18.0f, -25.0f), glm::vec3(180.0f, -18.0f, 180.0f), glm::vec3(199.0f, -18.0f, 138.0f) };
		for (int i = 0; i < 4; i++) {
			shaderPilares.use();
			glm::mat4 matrixPilares = glm::mat4(1.0);
			matrixPilares = glm::translate(matrixPilares, position12[i]);
			matrixPilares = glm::scale(matrixPilares, glm::vec3(3.0f, 3.0f, 3.0f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderPilares, matrixPilares);
			pilares.draw(shaderPilares);
		}

		//dragon temple

		shaderDragonTempleRuin.use();
		glm::mat4 matrixDragonTempleRuin = glm::mat4(1.0);
		matrixDragonTempleRuin = glm::scale(matrixDragonTempleRuin, glm::vec3(0.2f, 0.2f, 0.2f));
		matrixDragonTempleRuin = glm::translate(matrixDragonTempleRuin, glm::vec3(250.0f, 100.0f, -500.0f));
		OBJ(ViewMatrix, ProjectionMatrix, shaderDragonTempleRuin, matrixDragonTempleRuin);
		DragonTempleRuin.draw(shaderDragonTempleRuin);

		//big rocks

		shaderRock2.use();
		glm::mat4 matrixRock2 = glm::mat4(1.0);
		matrixRock2 = glm::scale(matrixRock2, glm::vec3(0.4f, 0.4f, 0.4f));
		matrixRock2 = glm::translate(matrixRock2, glm::vec3(-60.0f, -150.0f, -500.0f));
		OBJ(ViewMatrix, ProjectionMatrix, shaderRock2, matrixRock2);
		Rock2.draw(shaderRock2);

		shaderRock3.use();
		glm::mat4 matrixRock3 = glm::mat4(1.0);
		matrixRock3 = glm::scale(matrixRock3, glm::vec3(0.4f, 0.4f, 0.4f));
		matrixRock3 = glm::translate(matrixRock3, glm::vec3(450.0f, -150.0f, -500.0f));
		OBJ(ViewMatrix, ProjectionMatrix, shaderRock3, matrixRock3);
		Rock3.draw(shaderRock3);


		shaderRock4.use();
		glm::mat4 matrixRock4 = glm::mat4(1.0);
		matrixRock4 = glm::scale(matrixRock4, glm::vec3(0.4f, 0.4f, 0.4f));
		matrixRock4 = glm::translate(matrixRock4, glm::vec3(800.0f, -150.0f, -500.0f));
		OBJ(ViewMatrix, ProjectionMatrix, shaderRock4, matrixRock4);
		Rock4.draw(shaderRock4);


		shaderRock5.use();
		glm::mat4 matrixRock5 = glm::mat4(1.0);
		matrixRock5 = glm::scale(matrixRock5, glm::vec3(0.4f, 0.4f, 0.4f));
		matrixRock5 = glm::translate(matrixRock5, glm::vec3(-400.0f, -150.0f, -500.0f));
		OBJ(ViewMatrix, ProjectionMatrix, shaderRock5, matrixRock5);
		Rock5.draw(shaderRock5);

		//small rocks

		glm::vec3 position8[] = { glm::vec3(178.0f, -18.0f, 121.0f),glm::vec3(35.0f, -18.0f, 211.0f),glm::vec3(79.0f, -18.0f, 139.0f), glm::vec3(166.0f, -18.0f, 112.0f), glm::vec3(126.0f, -18.0f, 122.0f), glm::vec3(199.0f, -18.0f, 20.0f) };
		for (int i = 0; i < 6; i++) {
			shaderSmallRock1.use();
			glm::mat4 matrixSmallRock1 = glm::mat4(1.0);
			matrixSmallRock1 = glm::translate(matrixSmallRock1, position8[i]);
			matrixSmallRock1 = glm::scale(matrixSmallRock1, glm::vec3(0.2f, 0.2f, 0.2f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSmallRock1, matrixSmallRock1);
			SmallRock1.draw(shaderSmallRock1);
		}


		glm::vec3 position9[] = { glm::vec3(62.0f, -18.0f, 45.0f),glm::vec3(19.0f, -18.0f, 56.0f),glm::vec3(201.0f, -18.0f, 79.0f) ,glm::vec3(97.0f, -18.0f, 99.0f), glm::vec3(173.0f, -18.0f, 101.0f), glm::vec3(134.0f, -18.0f, -10.0f) };
		for (int i = 0; i < 6; i++) {
			shaderSmallRock2.use();
			glm::mat4 matrixSmallRock2 = glm::mat4(1.0);
			matrixSmallRock2 = glm::translate(matrixSmallRock2, position9[i]);
			matrixSmallRock2 = glm::scale(matrixSmallRock2, glm::vec3(0.125f, 0.125f, 0.125f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSmallRock2, matrixSmallRock2);
			SmallRock2.draw(shaderSmallRock2);
		}


		glm::vec3 position10[] = { glm::vec3(200.0f, -18.0f, 118.0f),glm::vec3(28.0f, -18.0f, 137.0f),glm::vec3(10.0f, -18.0f, 146.0f) ,glm::vec3(97.0f, -18.0f, 36.0f), glm::vec3(-92.0f, -18.0f, 12.0f), glm::vec3(5.0f, -18.0f, 7.0f) };
		for (int i = 0; i < 6; i++) {
			shaderSmallRock3.use();
			glm::mat4 matrixSmallRock3 = glm::mat4(1.0);
			matrixSmallRock3 = glm::translate(matrixSmallRock3, position10[i]);
			matrixSmallRock3 = glm::scale(matrixSmallRock3, glm::vec3(0.2f, 0.2f, 0.2f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSmallRock3, matrixSmallRock3);
			SmallRock3.draw(shaderSmallRock3);
		}


		glm::vec3 position11[] = { glm::vec3(-60.0f, -18.0f, 88.0f),glm::vec3(345.0f, -18.0f, 64.0f),glm::vec3(221.0f, -18.0f, 42.0f) ,glm::vec3(34.0f, -18.0f, 57.0f), glm::vec3(-10.0f, -18.0f, 91.0f), glm::vec3(129.0f, -18.0f, 31.0f) };
		for (int i = 0; i < 6; i++) {
			shaderSmallRock4.use();
			glm::mat4 matrixSmallRock4 = glm::mat4(1.0);
			matrixSmallRock4 = glm::translate(matrixSmallRock4, position11[i]);
			matrixSmallRock4 = glm::scale(matrixSmallRock4, glm::vec3(0.2f, 0.2f, 0.2f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderSmallRock4, matrixSmallRock4);
			SmallRock4.draw(shaderSmallRock4);
		}

		//arches

		glm::vec3 position4[] = { glm::vec3(500.0f, -150.0f, 500.0f),glm::vec3(500.0f, -150.0f, 600.0f),glm::vec3(500.0f, -150.0f, 700.0f) ,glm::vec3(500.0f, -150.0f, 800.0f) };
		for (int i = 0; i < 4; i++) {
			shaderArchway1.use();
			glm::mat4 matrixArchway1 = glm::mat4(1.0);
			matrixArchway1 = glm::scale(matrixArchway1, glm::vec3(0.125f, 0.125f, 0.125f));
			matrixArchway1 = glm::translate(matrixArchway1, position4[i]);
			OBJ(ViewMatrix, ProjectionMatrix, shaderArchway1, matrixArchway1);
			Archway1.draw(shaderArchway1);
		}

		//ruine

		glm::vec3 position5[] = { glm::vec3(180.0f, -20.0f, 57.0f), glm::vec3(-20.0f, -20.0f, 201.0f), glm::vec3(185.0f, -20.0f, 143.0f) };
		for (int i = 0; i < 3; i++) {
			shaderRuine1.use();
			glm::mat4 matrixRuine1 = glm::mat4(1.0);
			matrixRuine1 = glm::translate(matrixRuine1, position5[i]);
			matrixRuine1 = glm::scale(matrixRuine1, glm::vec3(0.2f, 0.2f, 0.2f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderRuine1, matrixRuine1);
			ruine1.draw(shaderRuine1);
		}

		glm::vec3 position6[] = { glm::vec3(160.0f, -20.0f, 43.0f), glm::vec3(190.0f, -20.0f, 127.0f), glm::vec3(-30.0f, -20.0f, 162.0f) };
		for (int i = 0; i < 3; i++) {
			shaderRuine2.use();
			glm::mat4 matrixRuine2 = glm::mat4(1.0);
			matrixRuine2 = glm::translate(matrixRuine2, position6[i]);
			matrixRuine2 = glm::scale(matrixRuine2, glm::vec3(0.2f, 0.2f, 0.2f));
			OBJ(ViewMatrix, ProjectionMatrix, shaderRuine2, matrixRuine2);
			ruine2.draw(shaderRuine2);
		}

		if (ON) {
			shaderBubbles.use();
			Bubbles bubblesR = ANIMATION(bubbles, shaderBubbles, 4.0f, bb1);
			OBJ(ViewMatrix, ProjectionMatrix, shaderBubbles, bubblesR.getMatrix());
			bubbles.draw(shaderBubbles);
			shaderBubbles.use();
			Bubbles bubblesL = ANIMATION(bubbles, shaderBubbles, -3.0f, bb2);
			OBJ(ViewMatrix, ProjectionMatrix, shaderBubbles, bubblesL.getMatrix());
			bubbles.draw(shaderBubbles);
			bb1.y += 0.05f;
			bb2.y += 0.05f;
			if (bb1.y > 10.0f) {  //on reset, it spawn in fornt of camera
				bb1.y = -10.0f;
				bb1.Oy = angleOy;
			}
			if (bb2.y > 15.0f) {
				bb2.y = -15.0f;
				bb2.Oy = angleOy;
			}
			
		}

		shaderWater.use();

		glm::mat4 matrixWater = glm::mat4(1.0);
		matrixWater = glm::translate(matrixWater, camera.getCameraPosition());
		glUniform1f(glGetUniformLocation(shaderWater.getId(), "opacity"), opacity / 10);
		OBJ(ViewMatrix, ProjectionMatrix, shaderWater, matrixWater);

		sphere.draw(shaderWater);

		struct ImGuiParameterState
		{
			int selected_radio;
		};

		static ImGuiParameterState state;

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoResize;
		bool open_ptr = true;
		ImGuiCond cond = 0;

		if (startGame == false) {
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(window.getWidth(), window.getHeight()), cond);
			READFILE("Resources/Text/Story.txt");
			ImGui::Begin("Story Arc", &open_ptr, window_flags);
			for (int i = 0; i < Text.size(); i++) {
				TextCenter(Text[i].c_str(), i, true, 0, false);
			}
			Text.clear();
			if (ImGui::Button("Start")) { startGame = true; }
			ON = true;
			ImGui::End();
		}
		if (startGame == true && settings == false) {
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(window.getWidth() / 3.5, window.getHeight() / 6), cond);
			if (ImGui::Begin("HUD", &open_ptr, window_flags)) {
				if (ImGui::Button("Settings")) { settings = true; }
				ImGui::Text("\n\n\nHi there adventurer!\n I'm your very reliable game developer!");
			}ImGui::End();
		}
		else if (startGame == true && settings == true) {
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(window.getWidth() / 2, window.getHeight() / 4), cond);
			if (ImGui::Begin("Settings", &open_ptr, window_flags)) {
				ImGui::Text("SETTINGS\n\n\n");
				ImGui::Checkbox("Animation", &ON);
				ImGui::SliderFloat("Opacity", &opacity, 0, 10);
				ImGui::SliderFloat("Movment Sensitivity", &movment, 0, 10);
				ImGui::SliderFloat("Rotation Sensitivity", &rotation, 0, 10);
				if (ImGui::Button("Back")) { settings = false; }
			}ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		///// FPS camera base on mouse movements //////

		if (window.getMouseMoved())
		{
			float cameraSpeed = rotation/100 * 30 / 0.049 * deltaTime;
			double yaw = window.getYaw();
			angleOy -= window.getYaw() * (rotation/100.0);
			camera.rotateOy(cameraSpeed * (-yaw));
			double pitch = window.getPitch();;
			camera.rotateOx(cameraSpeed * pitch);
			window.setMouseMoved(false);
		}

		window.update();

	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void processKeyboardInput()
{
	float cameraSpeed = movment * 10 * deltaTime;

	//translation
	if (window.isPressed(GLFW_KEY_W))
		camera.keyboardMoveFront(cameraSpeed);
	if (window.isPressed(GLFW_KEY_S))
		camera.keyboardMoveBack(cameraSpeed);
	if (window.isPressed(GLFW_KEY_A))
		camera.keyboardMoveLeft(cameraSpeed);
	if (window.isPressed(GLFW_KEY_D))
		camera.keyboardMoveRight(cameraSpeed);
	if (window.isPressed(GLFW_KEY_Q))
		camera.keyboardMoveUp(cameraSpeed);
	if (window.isPressed(GLFW_KEY_E))
		camera.keyboardMoveDown(cameraSpeed);
	if (window.isPressed(GLFW_KEY_Z)) {
		angleOz -= deltaTime / 1.9 * 57.2957795;
		camera.rotateOz(-cameraSpeed);
	}
	if (window.isPressed(GLFW_KEY_C)) {
		angleOz += deltaTime / 1.9 * 57.2957795;
		camera.rotateOz(cameraSpeed);
	}

	//rotation
	/*if (window.isPressed(GLFW_KEY_LEFT)) {
		angleOy += cameraSpeed1;
		camera.rotateOy(cameraSpeed1);
	}
	if (window.isPressed(GLFW_KEY_RIGHT)) {
		angleOy -= cameraSpeed1;
		camera.rotateOy(-cameraSpeed1);
	};
	if (window.isPressed(GLFW_KEY_UP)) {
		angleOx += cameraSpeed1;
		camera.rotateOx(cameraSpeed1);
	}
	if (window.isPressed(GLFW_KEY_DOWN)) {
		angleOx -= cameraSpeed1;
		camera.rotateOx(-cameraSpeed1);
	}*/
}
