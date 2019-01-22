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
		lightingView = glm::translate(lightingView, glm::vec3(0.0f, 0.0f, -3.0f));  //OpenGL中使用右手坐标系，在观察矩阵中，将场景往后移动3个单位，因为一开始，我们将是位于世界原点的
		glUniformMatrix4fv(lightingViewLocation, 1, GL_FALSE, glm::value_ptr(lightingView));
		//projection mat
		glm::mat4 lightingProjection;
		//第1个参数是fov，也就是视野大小，正常视觉指定为45，第2个参数是视图的宽高比，第3个参数是near距离，第4个参数是far距离
		lightingProjection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);  //创建透视投影矩阵
		glUniformMatrix4fv(lightingProjectionLocation, 1, GL_FALSE, glm::value_ptr(lightingProjection));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	system("pause");
	return 0;
}