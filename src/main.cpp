#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

#include "renderer.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "vertex_array.hpp"

struct ShaderSource {
	std::string m_vertex;
	std::string m_fragment;
};

static ShaderSource parse_shader(const std::string& filepath) {
	std::ifstream stream(filepath);
	
	enum ShaderType { None = -1, Vertex = 0, Fragment = 1 };

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::None;

	while(getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {

			if (line.find("vertex") != std::string::npos)
				type = ShaderType::Vertex;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::Fragment;
			else
				type = ShaderType::None;

		} else if (type != ShaderType::None) {
			ss[type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int compile_shader(const std::string& source, unsigned int type) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);

	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "[ERROR] - Failed shader compilation: " << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int create_shader(const std::string& vertex, const std::string& fragment) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compile_shader(vertex, GL_VERTEX_SHADER);
	unsigned int fs = compile_shader(fragment, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

// TODO: remove from global scope

float positions[] = {
	-0.05f, -0.8f,
	0.05f, -0.8f,
	0.0f, -0.7f,
};

unsigned int indices[] = {
	0, 1, 2,
};

float movex = 0.0f;
float movey = 0.0f;
void update_position() {
	positions[0] += movex;
	positions[2] += movex;	
	positions[4] += movex;	

	positions[1] += movey;
	positions[3] += movey;	
	positions[5] += movey;	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_D)
		movex = action == GLFW_RELEASE ? 0.0f : 0.02f;
	if (key == GLFW_KEY_A)
		movex = action == GLFW_RELEASE ? 0.0f : -0.02f;

	if (key == GLFW_KEY_W)
		movey = action == GLFW_RELEASE ? 0.0f : 0.02f;
	if (key == GLFW_KEY_S)
		movey = action == GLFW_RELEASE ? 0.0f : -0.02f;
}

int main() {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	std::cout << glGetString(GL_VERSION) << std::endl;

	VertexArray va;
	VertexBuffer vb(positions, sizeof(positions));

	VertexBufferLayout layout;
	layout.push<float>(2);
	va.add_buffer(vb, layout);
	
	IndexBuffer ib(indices, 6);

	ShaderSource source = parse_shader("res/shaders/basic.shader");
	unsigned int shader = create_shader(source.m_vertex, source.m_fragment);
	glUseProgram(shader);
	int location = glGetUniformLocation(shader, "u_color");
	assert(location != -1);

	// unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);

	glfwSetKeyCallback(window, key_callback);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f);

		va.bind();
		ib.bind();

		clear_error_stack();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
		print_error_stack();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		update_position();
		vb.update(positions, 0, sizeof(positions));
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}