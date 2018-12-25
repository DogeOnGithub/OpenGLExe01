#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window);

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
	 0.8f, 0.8f, 0.0f
};

unsigned int indices[] = {
	0, 1, 2,
	1, 3, 2
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
"void main(){                                         \n"
"    FragColor = vertexColor;}                        \n";

int main() {

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