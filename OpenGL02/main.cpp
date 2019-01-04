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

int openGLTest01();

int openGLTest02();

int openGLTest03();

int openGLTest04();

int openGLTest05();

int openGLTest06();

int openGLTest07();

void processInput(GLFWwindow* window);

void loadTexture(unsigned int* texture, const char* path, GLint interFormat, GLenum format);

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
	 0.8f, 0.8f, 0.0f
};

//这里的数组，新增了顶点外，顶点的RGB颜色
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
	//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标，也就是uv值，也可以叫st值，实际上就是从纹理的左下角作为原点x和y轴的0到1之间的float值 -
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indicesTexture[] = {
	0, 1, 3,
	1, 2, 3
};

const char* vertexShaderSource =
"#version 330 core                                    \n"
"layout (location = 0) in vec3 aPos;                  \n"
"out vec4 vertexColor;                                \n"  //声明一个4维向量输出，vertexShader必定输出一个4维向量gl_Position，此外可以自定义其他输出
"void main(){                                         \n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
"    vertexColor = vec4(1.0, 0.0, 0.0, 1.0);         }\n";

const char* fragmentShaderSource =
"#version 330 core                                    \n"
"out vec4 FragColor;                                  \n"
"in vec4 vertexColor;                                 \n"  //声明一个4维向量输入，这个输入和vertexShader中的输出类型名称一致，并把该输入作为fragShader的输出颜色
/*
Uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式，但uniform和顶点属性有些不同
uniform是全局的(Global)
全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的，而且它可以被着色器程序的任意着色器在任意阶段访问
无论你把uniform值设置成什么，uniform会一直保存它们的数据，直到它们被重置或更新
*/
"uniform vec4 customColor;                            \n"
"void main(){                                         \n"
"    FragColor = customColor;}                        \n";

const char* vertexShaderSource2 =
"#version 330 core                                    \n"
"layout (location = 0) in vec3 aPos;                  \n"
"layout (location = 1) in vec3 aColor;                \n"   //从1槽中读取3维向量
"out vec4 vertexColor;                                \n"
"void main(){                                         \n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
"    vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0);         }\n";  //将3维向量转为4维向量，也就是将RGB转为RGBA

const char* fragmentShaderSource2 =
"#version 330 core                                    \n"
"out vec4 FragColor;                                  \n"
"in vec4 vertexColor;                                 \n"
"void main(){                                         \n"
"    FragColor = vertexColor;}                        \n";

int main()
{

	return openGLTest07();

}

//使用输出输入以及使用Uniform改变片段着色器输出的颜色
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

	//使用线框模式,第一个参数表示我们打算将其应用到所有的三角形的正面和背面，第二个参数告诉我们用线来绘制
	//之后的绘制调用会一直以线框模式绘制三角形，直到我们用glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)将其设置回默认模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//创建索引缓冲对象
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

		int timeVal = glfwGetTime();  //获取当前CPU时间
		float redValue = sin(timeVal) / 2.0f + 0.5f;   //将redValue的值固定在0~1
		int colorUniformLocation = glGetUniformLocation(shaderProgram, "customColor");   //获取uniform的位置
		glUniform4f(colorUniformLocation, redValue, 0.0f, 0.0f, 1.0f);   //设置指定位置的Uniform的值

		//第一个参数指定绘制的模式，第二个参数指定绘制的顶点数，第三个参数指定索引的类型，最有一个指定EBO中的偏移量
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

//在顶点数组中存储RGB值，并将RGB值作为fragShader的输入
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

	//使用线框模式,第一个参数表示我们打算将其应用到所有的三角形的正面和背面，第二个参数告诉我们用线来绘制
	//之后的绘制调用会一直以线框模式绘制三角形，直到我们用glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)将其设置回默认模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//创建索引缓冲对象
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

		//第一个参数指定绘制的模式，第二个参数指定绘制的顶点数，第三个参数指定索引的类型，最有一个指定EBO中的偏移量
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

//从外部读取shader源码文件，OpenGL接受char*类型的源码
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

	//使用线框模式,第一个参数表示我们打算将其应用到所有的三角形的正面和背面，第二个参数告诉我们用线来绘制
	//之后的绘制调用会一直以线框模式绘制三角形，直到我们用glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)将其设置回默认模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//创建索引缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//创建Shader
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

		//第一个参数指定绘制的模式，第二个参数指定绘制的顶点数，第三个参数指定索引的类型，最有一个指定EBO中的偏移量
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

	//创建索引缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTexture), indicesTexture, GL_STATIC_DRAW);

	//创建Shader
	CustomShader* shader = new CustomShader("vertexTexture.txt", "fragmentTexture.txt");

	//创建Texture
	unsigned int texture;
	loadTexture(&texture, "container.jpg", GL_RGB, GL_RGB);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//应用纹理，这里使用顶点属性定义uv值的取样
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glBindVertexArray(VAO);
		shader->use();

		//绑定纹理，第一个参数指定绑定的纹理的类型，第二个参数是纹理的id，会自动把纹理赋值给片段着色器的采样器
		glBindTexture(GL_TEXTURE_2D, texture);

		//第一个参数指定绘制的模式，第二个参数指定绘制的顶点数，第三个参数指定索引的类型，最有一个指定EBO中的偏移量
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

//多个纹理单元
int openGLTest05()
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

	//创建索引缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTexture), indicesTexture, GL_STATIC_DRAW);

	//创建Shader
	CustomShader* shader = new CustomShader("vertexTexture.txt", "fragmentTexture.txt");

	//创建Texture
	unsigned int texture;
	loadTexture(&texture, "container.jpg", GL_RGB, GL_RGB);

	unsigned int textureFace;
	loadTexture(&textureFace, "awesomeface.png", GL_RGBA, GL_RGBA);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//应用纹理，这里使用顶点属性定义uv值的取样
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO);
	shader->use();

	//激活第0个纹理单元，默认激活第0个
	glActiveTexture(GL_TEXTURE0);
	//绑定纹理，第一个参数指定绑定的纹理的类型，第二个参数是纹理的id，会自动把纹理赋值给片段着色器的采样器
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureFace);

	//这个是设置sampler采样器的纹理单元，也就是可以将一个指定的Texture Unit绑定到指定的Uniform采样器
	glUniform1i(glGetUniformLocation(shader->ProgramID, "textureWood"), 0);  //手动设置，将纹理单元0绑定到名为textureWood的采样器
	shader->setInt("textureFace", 1);  //使用自定义的Shader类中的函数设置

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//第一个参数指定绘制的模式，第二个参数指定绘制的顶点数，第三个参数指定索引的类型，最有一个指定EBO中的偏移量
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

int openGLTest06()
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

	//创建索引缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTexture), indicesTexture, GL_STATIC_DRAW);

	//创建Shader
	CustomShader* shader = new CustomShader("vertexTransform.txt", "fragmentTexture.txt");

	//创建Texture
	unsigned int texture;
	loadTexture(&texture, "container.jpg", GL_RGB, GL_RGB);

	unsigned int textureFace;
	loadTexture(&textureFace, "awesomeface.png", GL_RGBA, GL_RGBA);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//应用纹理，这里使用顶点属性定义uv值的取样
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO);
	shader->use();

	//激活第0个纹理单元，默认激活第0个
	glActiveTexture(GL_TEXTURE0);
	//绑定纹理，第一个参数指定绑定的纹理的类型，第二个参数是纹理的id，会自动把纹理赋值给片段着色器的采样器
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureFace);

	//这个是设置sampler采样器的纹理单元，也就是可以将一个指定的Texture Unit绑定到指定的Uniform采样器
	glUniform1i(glGetUniformLocation(shader->ProgramID, "textureWood"), 0);  //手动设置，将纹理单元0绑定到名为textureWood的采样器
	shader->setInt("textureFace", 1);  //使用自定义的Shader类中的函数设置

	//设置transform
	glm::mat4 trans;  //单位矩阵
	trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 0.0f));  //构建位移矩阵，第一个参数是单位矩阵，第二个是位移
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));  //构建旋转矩阵，需要注意的是，在组合矩阵的时候，向量是最先和最右边的矩阵相乘的，也就是说到这里为止，会先旋转，再位移

	//获取Uniform的位置值
	unsigned int transformLocation = glGetUniformLocation(shader->ProgramID, "transform");

	//第1个参数是uniform的位置值
	//第2个参数是告诉OpenGL将要发送多少个矩阵
	//第3个参数询问是否希望对矩阵进行置换(Transpose)，也就是说交换我们矩阵的行和列
	//OpenGL开发者通常使用一种内部矩阵布局，叫做列主序(Column-major Ordering)布局，GLM的默认布局就是列主序，所以并不需要置换矩阵，所以填GL_FALSE
	//第4个参数就是真正的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望接受的那种，因此要先用GLM的自带的函数value_ptr来变换这些数据
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//第一个参数指定绘制的模式，第二个参数指定绘制的顶点数，第三个参数指定索引的类型，最有一个指定EBO中的偏移量
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

//进入3D效果
int openGLTest07()
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

	//创建索引缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTexture), indicesTexture, GL_STATIC_DRAW);

	//创建Shader
	CustomShader* shader = new CustomShader("threedVertex.txt", "fragmentTexture.txt");

	//创建Texture
	unsigned int texture;
	loadTexture(&texture, "container.jpg", GL_RGB, GL_RGB);

	unsigned int textureFace;
	loadTexture(&textureFace, "awesomeface.png", GL_RGBA, GL_RGBA);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//应用纹理，这里使用顶点属性定义uv值的取样
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO);
	shader->use();

	//激活第0个纹理单元，默认激活第0个
	glActiveTexture(GL_TEXTURE0);
	//绑定纹理，第一个参数指定绑定的纹理的类型，第二个参数是纹理的id，会自动把纹理赋值给片段着色器的采样器
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureFace);

	//这个是设置sampler采样器的纹理单元，也就是可以将一个指定的Texture Unit绑定到指定的Uniform采样器
	glUniform1i(glGetUniformLocation(shader->ProgramID, "textureWood"), 0);  //手动设置，将纹理单元0绑定到名为textureWood的采样器
	shader->setInt("textureFace", 1);  //使用自定义的Shader类中的函数设置

	//设置transform
	glm::mat4 trans;  //单位矩阵
	trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));  //构建位移矩阵，第一个参数是单位矩阵，第二个是位移
	trans = glm::rotate(trans, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));  //构建旋转矩阵，需要注意的是，在组合矩阵的时候，向量是最先和最右边的矩阵相乘的，也就是说到这里为止，会先旋转，再位移
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

	glm::mat4 model;
	model = trans;  //构建模型矩阵

	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  //OpenGL中使用右手坐标系，在观察矩阵中，将场景往后移动3个单位，因为一开始，我们将是位于世界原点的

	glm::mat4 projection;
	//第1个参数是fov，也就是视野大小，正常视觉指定为45，第2个参数是视图的宽高比，第3个参数是near距离，第4个参数是far距离
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);  //创建透视投影矩阵

	//获取Uniform的位置值
	unsigned int transformLocation = glGetUniformLocation(shader->ProgramID, "transform");
	unsigned int modelLocation = glGetUniformLocation(shader->ProgramID, "model");
	unsigned int viewLocation = glGetUniformLocation(shader->ProgramID, "view");
	unsigned int projectionLocation = glGetUniformLocation(shader->ProgramID, "projection");

	//第1个参数是uniform的位置值
	//第2个参数是告诉OpenGL将要发送多少个矩阵
	//第3个参数询问是否希望对矩阵进行置换(Transpose)，也就是说交换我们矩阵的行和列
	//OpenGL开发者通常使用一种内部矩阵布局，叫做列主序(Column-major Ordering)布局，GLM的默认布局就是列主序，所以并不需要置换矩阵，所以填GL_FALSE
	//第4个参数就是真正的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望接受的那种，因此要先用GLM的自带的函数value_ptr来变换这些数据
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//第一个参数指定绘制的模式，第二个参数指定绘制的顶点数，第三个参数指定索引的类型，最有一个指定EBO中的偏移量
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

//加载纹理
void loadTexture(unsigned int* texture, const char* path, GLint interFormat, GLenum format)
{
	//生成纹理（实际上应该是生成一个纹理的缓冲区，这个缓冲区还没有图像数据），需要使用一个unsigned int类型的id来存储
	glGenTextures(1, texture);
	//绑定生成的纹理缓冲到2D纹理，绑定之后，后续的纹理指令都将会对当前绑定纹理起作用
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  //设置s的也就是x轴的纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  //设置t的也就是y轴的纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  //设置缩小的纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //设置放大的纹理过滤方式

	//一般情况下，图片的y轴的0在图片的顶部，但是OpenGL的y轴的0坐标是在图片的底部的，stb_image库带有函数可以在加载时反转y轴
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	//使用stbi_load函数加载图片，第1个参数是图片的路径，2、3、4参数则是宽高和颜色通道的个数，该方法会返回图片的对应的值
	unsigned char* textureData = stbi_load(path, &width, &height, &nrChannels, 0);
	if (textureData)
	{
		//上面绑定过纹理之后，现在可以对纹理进行操作
		//这里载入图片数据到纹理
		/*第1个参数指定了纹理目标(Target)，设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）
		  第2个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话，这里我们填0，也就是基本级别
		  第3个参数告诉OpenGL我们希望把纹理储存为何种格式，载入的图像只有RGB值，因此也把纹理储存为RGB值
		  第4个和第5个参数设置最终的纹理的宽度和高度，之前加载图像的时候储存了它们，所以使用对应的变量
		  下个参数应该总是被设为0（历史遗留的问题）
		  第7第8个参数定义了源图的格式和数据类型，使用RGB值加载这个图像，并把它们储存为char(byte)数组，将会传入对应值
		  最后一个参数是真正的图像数据*/
		  //当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像
		  //目前只有基本级别(Base-level)的纹理图像被加载了，如果要使用多级渐远纹理，我们必须手动设置所有不同的图像（不断递增第二个参数）
		  //或者，直接在生成纹理之后调用glGenerateMipmap
		glTexImage2D(GL_TEXTURE_2D, 0, interFormat, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);

		//这会为当前绑定的纹理自动生成所有需要的多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//释放内存
	stbi_image_free(textureData);
}