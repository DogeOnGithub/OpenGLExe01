#include <iostream>

#define GLEW_STATIC

#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "CustomShader.h"
#include "stb_image.h"

void init();

int openGLTest01();

float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};

GLFWwindow * window = NULL;

int main()
{
	return openGLTest01();
}

void init()
{
	glfwInit();

	window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "error create window" << std::endl;
		glfwTerminate();
		system("pause");
		return;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init fail" << std::endl;
		glfwTerminate();
		return;
	}

	glViewport(0, 0, 800, 600);
}

int openGLTest01()
{
	init();

	glEnable(GL_DEPTH);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	CustomShader* lightintShader = new CustomShader("vertex.vert", "lighting.frag");

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightintShader->use();
		//color
		unsigned int objectColor = glGetUniformLocation(lightintShader->ProgramID, "objectColor");
		unsigned int lightColor = glGetUniformLocation(lightintShader->ProgramID, "lightColor");
		glUniform3f(objectColor, 1.0f, 0.5f, 0.3f);
		glUniform3f(lightColor, 1.0f, 1.0f, 1.0f);
		//uniform loaction
		unsigned int lightingModelLocation = glGetUniformLocation(lightintShader->ProgramID, "model");
		unsigned int lightingViewLocation = glGetUniformLocation(lightintShader->ProgramID, "view");
		unsigned int lightingProjectionLocation = glGetUniformLocation(lightintShader->ProgramID, "projection");
		//model mat
		glm::mat4 lightingModel;
		lightingModel = glm::rotate(lightingModel, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(lightingModelLocation, 1, GL_FALSE, glm::value_ptr(lightingModel));
		//view mat
		glm::mat4 lightingView;
		lightingView = glm::translate(lightingView, glm::vec3(0.0f, 0.0f, -3.0f));  //OpenGL��ʹ����������ϵ���ڹ۲�����У������������ƶ�3����λ����Ϊһ��ʼ�����ǽ���λ������ԭ���
		glUniformMatrix4fv(lightingViewLocation, 1, GL_FALSE, glm::value_ptr(lightingView));
		//projection mat
		glm::mat4 lightingProjection;
		//��1��������fov��Ҳ������Ұ��С�������Ӿ�ָ��Ϊ45����2����������ͼ�Ŀ�߱ȣ���3��������near���룬��4��������far����
		lightingProjection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);  //����͸��ͶӰ����
		glUniformMatrix4fv(lightingProjectionLocation, 1, GL_FALSE, glm::value_ptr(lightingProjection));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	system("pause");
	return 0;
}