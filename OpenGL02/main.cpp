#include <iostream>

#define GLEW_STATIC

#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "CustomShader.h"
#include "stb_image.h"

int openGLTest01();

int openGLTest02();

int openGLTest03();

int openGLTest04();

void processInput(GLFWwindow* window);

void loadTexture(unsigned int* texture);

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	 0.8f, 0.8f, 0.0f
};

//��������飬�����˶����⣬�����RGB��ɫ
float vertices2[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.8f, 0.8f, 0.0f, 1.0f, 1.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 2,
	1, 3, 2
};

float verticesTexture[] = {
	//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
};

unsigned int indicesTexture[] = {
	0, 1, 3,
	1, 2, 3
};

const char* vertexShaderSource =
"#version 330 core                                    \n"
"layout (location = 0) in vec3 aPos;                  \n"
"out vec4 vertexColor;                                \n"  //����һ��4ά���������vertexShader�ض����һ��4ά����gl_Position����������Զ����������
"void main(){                                         \n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
"    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);         }\n";

const char* fragmentShaderSource =
"#version 330 core                                    \n"
"out vec4 FragColor;                                  \n"
"in vec4 vertexColor;                                 \n"  //����һ��4ά�������룬��������vertexShader�е������������һ�£����Ѹ�������ΪfragShader�������ɫ
/*
Uniform��һ�ִ�CPU�е�Ӧ����GPU�е���ɫ���������ݵķ�ʽ����uniform�Ͷ���������Щ��ͬ
uniform��ȫ�ֵ�(Global)
ȫ����ζ��uniform����������ÿ����ɫ����������ж��Ƕ�һ�޶��ģ����������Ա���ɫ�������������ɫ��������׶η���
�������uniformֵ���ó�ʲô��uniform��һֱ�������ǵ����ݣ�ֱ�����Ǳ����û����
*/
"uniform vec4 customColor;                            \n"
"void main(){                                         \n"
"    FragColor = customColor;}                        \n";

const char* vertexShaderSource2 =
"#version 330 core                                    \n"
"layout (location = 0) in vec3 aPos;                  \n"
"layout (location = 1) in vec3 aColor;                \n"   //��1���ж�ȡ3ά����
"out vec4 vertexColor;                                \n"
"void main(){                                         \n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
"    vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0);         }\n";  //��3ά����תΪ4ά������Ҳ���ǽ�RGBתΪRGBA

const char* fragmentShaderSource2 =
"#version 330 core                                    \n"
"out vec4 FragColor;                                  \n"
"in vec4 vertexColor;                                 \n"
"void main(){                                         \n"
"    FragColor = vertexColor;}                        \n";

int main()
{

	return openGLTest04();

}

//ʹ����������Լ�ʹ��Uniform�ı�Ƭ����ɫ���������ɫ
int openGLTest01()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL02", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "error create" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init fail" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//ʹ���߿�ģʽ,��һ��������ʾ���Ǵ��㽫��Ӧ�õ����е������ε�����ͱ��棬�ڶ�������������������������
	//֮��Ļ��Ƶ��û�һֱ���߿�ģʽ���������Σ�ֱ��������glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)�������û�Ĭ��ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//���������������
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);


	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		int timeVal = glfwGetTime();  //��ȡ��ǰCPUʱ��
		float redValue = sin(timeVal) / 2.0f + 0.5f;   //��redValue��ֵ�̶���0~1
		int colorUniformLocation = glGetUniformLocation(shaderProgram, "customColor");   //��ȡuniform��λ��
		glUniform4f(colorUniformLocation, redValue, 0.0f, 0.0f, 1.0f);   //����ָ��λ�õ�Uniform��ֵ

		//��һ������ָ�����Ƶ�ģʽ���ڶ�������ָ�����ƵĶ�����������������ָ�����������ͣ�����һ��ָ��EBO�е�ƫ����
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

//�ڶ��������д洢RGBֵ������RGBֵ��ΪfragShader������
int openGLTest02()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL02", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "error create" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init fail" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//ʹ���߿�ģʽ,��һ��������ʾ���Ǵ��㽫��Ӧ�õ����е������ε�����ͱ��棬�ڶ�������������������������
	//֮��Ļ��Ƶ��û�һֱ���߿�ģʽ���������Σ�ֱ��������glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)�������û�Ĭ��ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//���������������
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource2, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//��һ������ָ�����Ƶ�ģʽ���ڶ�������ָ�����ƵĶ�����������������ָ�����������ͣ�����һ��ָ��EBO�е�ƫ����
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

//���ⲿ��ȡshaderԴ���ļ���OpenGL����char*���͵�Դ��
int openGLTest03()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL02", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "error create" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init fail" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//ʹ���߿�ģʽ,��һ��������ʾ���Ǵ��㽫��Ӧ�õ����е������ε�����ͱ��棬�ڶ�������������������������
	//֮��Ļ��Ƶ��û�һֱ���߿�ģʽ���������Σ�ֱ��������glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)�������û�Ĭ��ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//���������������
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//����Shader
	CustomShader* shader = new CustomShader("vertx.txt", "frag.txt");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		shader->use();

		//��һ������ָ�����Ƶ�ģʽ���ڶ�������ָ�����ƵĶ�����������������ָ�����������ͣ�����һ��ָ��EBO�е�ƫ����
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

int openGLTest04()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL02", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "error create" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init fail" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTexture), verticesTexture, GL_STATIC_DRAW);

	//���������������
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTexture), indicesTexture, GL_STATIC_DRAW);

	//����Shader
	CustomShader* shader = new CustomShader("vertexTexture.txt", "fragmentTexture.txt");

	//����Texture
	unsigned int texture;
	loadTexture(&texture);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glBindVertexArray(VAO);
		shader->use();
		glBindTexture(GL_TEXTURE_2D, texture);

		//��һ������ָ�����Ƶ�ģʽ���ڶ�������ָ�����ƵĶ�����������������ָ�����������ͣ�����һ��ָ��EBO�е�ƫ����
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void loadTexture(unsigned int* texture)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* textureData = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(textureData);
}