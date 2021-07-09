#include "renderer.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <cassert>

void clear_error_stack() {
	while(glGetError());
}

void print_error_stack() {
	bool error = false;
	while(GLenum code = glGetError()) {
		error = true;
		std::cout << "Error code: " << code << std::endl;
	}

	assert(!error);
}