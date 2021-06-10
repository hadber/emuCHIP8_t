#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <SDL2/SDL.h>

class Chip8 {

public:
	Chip8();
	~Chip8() = default;
	void step(); // one cpu cycle
	void init(); // reset everything
  void load(char*); // load new program into memory
	void clearScreen();
	unsigned char retKey();

	unsigned char V[15]; // general purpose register
	unsigned short I; // register I, generally used for memory

	unsigned char delay_timer;
	unsigned char sound_timer; // timer registers

	unsigned short pc; // program counter
	unsigned char sp; // stack pointer
	unsigned short opcode;

	unsigned short stack[16];

	unsigned char display[64][32];

	unsigned char memory[4096];

  SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;

};

#endif
