#include <iostream>
#include <string>
#include "emuchip8_t.hpp"

int main(int argc, char* args[]) {
		if(argc < 2) {
			std::cout << "Usage: " << args[0] << " <chip8_prog.ch8>" << std::endl;
			exit(1);
		}
		if(argc == 3) {
			
		}
		Chip8 chip8inst;
		chip8inst.init();
//		chip8inst.testDisplay();
		chip8inst.load(args[1]);
		chip8inst.run();
	return 0;
}
