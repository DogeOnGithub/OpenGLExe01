#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window);

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f, 0.5f, 0.0f,
	 0.8f, 0.8f, 0.0f
};

const char* vertexShaderSource =
"#version 330 core                                    \n"
"layout (location = 0) in vec3 aPos;                  \n"
"void main(){                                         \n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);}\n";

const char* fragmentShaderSource =
"#version 330 core                                    \n"
"out vec4 FragColor;                                  \n"
"void main(){                                         \n"
"    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);}       \n";

int main()
{
	//OpenGL���ţ���������

	glfwInit();

	//�����޶�OpenGL�汾�����Բ�������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL01", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "fail" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Init GLEW
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "fail" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	//OpenGLĬ�����������ʱ����涥��ģ���������ͱ��涼����棬�������Ķ��������У�һ��������������ģ�һ���Ǳ���ģ����Ƕ����Կ���
	//�������޳�����
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //�޳�����

	//���ɶ����������
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//���ɶ��㻺�����
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//���붥����ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//����Ƭ����ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//���ӳ���ͱ�������ɫ�������ƴ���
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);//���Ӷ�����ɫ��
	glAttachShader(shaderProgram, fragmentShader);//����Ƭ����ɫ��
	glLinkProgram(shaderProgram);

	//ʹ���������������ν���VBO�е�����
	//��һ������0��ʾ��Ҫ���õĶ��㣬���������vertexShaderSource�е�location��ֵ����ָ��vertextShaderȥ����ȡ�������Ե�ֵ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//ʹ�øú��������������ԣ�����������ĵ�һ������������ָ��λ�õĶ�������
	glEnableVertexAttribArray(0);



	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6); //����ָ����6�����㣬ͨ����2��������������һ��4����

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
