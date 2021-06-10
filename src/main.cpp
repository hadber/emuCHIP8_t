#include <iostream>
#include <string>
#include "emuchip8_t.hpp"

int main(int argc, char* args[]) {

	if(argc < 2) {
		std::cout << "Usage: " << args[0] << " <chip8_prog.ch8>" << std::endl;
		exit(1);
	}

	Chip8 chip8inst;
	chip8inst.init();
	chip8inst.load(args[1]);
	for(int i = 0; i < 10; i++) {
		chip8inst.step();
	}
	return 0;
}
