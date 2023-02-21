#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <SDL2/SDL.h>

class Chip8 {

public:
	Chip8();
	~Chip8() = default;
	bool step(); // one cpu cycle
	void init(); // reset everything
	void load(char* path); // load new program into memory
	void clearScreen();
	void drawSprite(int, int, int);
	void printMemory(unsigned short, unsigned short);
	void draw();
	bool quit();
	void run();
	void testDisplay();
	void printScreen();
	unsigned char retKey();
private:
	unsigned char V[15]; // general purpose register
	unsigned short I; // register I, generally used for memory

	unsigned char delay_timer;
	unsigned char sound_timer; // timer registers

	unsigned short pc; // program counter
	unsigned char sp; // stack pointer
	unsigned short opcode;

	unsigned short stack[16];

	bool display[64][32];

	unsigned char memory[4096];

	bool done;
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
};

#endif
