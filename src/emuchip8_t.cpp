#include "emuchip8_t.hpp"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <iomanip>



Chip8::Chip8() {

}

void Chip8::init() {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 320;

	unsigned char chip8_fontset[80] =
	{
	    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
	    0x20, 0x60, 0x20, 0x20, 0x70, //1
	    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
	    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
	    0x90, 0x90, 0xF0, 0x10, 0x10, //4
	    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
	    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
	    0xF0, 0x10, 0x20, 0x40, 0x40, //7
	    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
	    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
	    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
	    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
	    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
	    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
	    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
	    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
	};

	for(int i = 0; i < 80; i++) // load the fontset in memory 0x000 - 0x1FF
		memory[i] = chip8_fontset[i];

	//Initialize SDL
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		std::cout <<  "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
  } else {
	//Create window
		window = SDL_CreateWindow( "CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL ) {
			std::cout <<  "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		}	else {
			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );

			//Fill the surface white
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );

			//Update the surface
			SDL_UpdateWindowSurface( window );

			//Wait two seconds
	//			SDL_Delay( 2000 );
		}
	}

	memset(this->memory, 0, sizeof(this->memory));
	this->pc = 0x200;

/*
	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();
*/
}

void Chip8::load(char* path) {
	std::ifstream ch8_prog(path, std::ifstream::in | std::ifstream::binary);
	if(!ch8_prog) {
		std::cout << "Failed to open file: " << path << std::endl;
		exit(2);
	}

	unsigned short inst = ch8_prog.get();
	for(int i = 0; ch8_prog.good(); i++) {
		this->memory[0x200 + i] = inst;
		inst = ch8_prog.get();
	}

	// debug below
/*
	std::cout << std::endl;
	int i = 0;
	for(auto a : this->memory) {
		std::cout << i++ << ": " << std::hex << (int)a << std::endl;
	}
	std::cout << std::endl;
*/
}

void Chip8::step() {

	this->opcode = (memory[this->pc]<<8) | memory[this->pc+1];

	unsigned char vx = this->opcode & 0x0F00;
	unsigned char vy = this->opcode & 0x00F0;
	unsigned char n = this->opcode & 0x000F;
	unsigned char nn = this->opcode & 0x00FF;
	unsigned short nnn = this->opcode & 0x0FFF;

	switch(this->opcode & 0xF000) {
		case 0x0000: {// clear the display, return from subroutine
			if((this->opcode & 0x000F) == 0x000E) { // return from subroutine
				this->pc = this->stack[--sp];
				this->pc -= 2;
			} else { // clear the display
				clearScreen();
			}
			break;
		}
		case 0x1000: {// JP addr
			this->pc = nnn;
			this->pc -= 2;
			break;
		}
		case 0x2000: { // CALL addr
			this->stack[sp++] = this->pc;
			this->pc = nnn;
			this->pc -= 2;
			break;
		}
		case 0x3000: {// skip next inst if vx == nn
			if(V[vx] == nn)
				this->pc += 2;
			break;
		}
		case 0x4000: {// skip next inst if vx != nn
			if(V[vx] != nn)
				this->pc += 2;
			break;
		}
		case 0x5000: { // skip next inst if vx == vy
			if(V[vx] == V[vy])
				this->pc += 2;
			break;
		}
		case 0x6000: {
			V[vx] = nn;
			break;
		}
		case 0x7000: {
			V[vx] += nn;
			break;
		}
		case 0x8000: {
			switch(this->opcode & 0x000F) {
				case 0x0000: {
					V[vx] = V[vy];
					break;
				}
				case 0x0001: {
					V[vx] = V[vx] | V[vy];
					break;
				}
				case 0x0002: {
					V[vx] = V[vx] & V[vy];
					break;
				}
				case 0x0003: {
					V[vx] = V[vx] ^ V[vy];
					break;
				}
				case 0x0004: {
					unsigned short res = V[vx] + V[vy];
					V[vx] = res;
					V[0xF] = (res & 0x0100) >> 8;
					break;
				}
				case 0x0005: {
					unsigned char res = V[vx] - V[vy];
					V[0xF] = V[vx] > V[vy] ? 0 : 1;
					V[vx] = res;
					break;
				}
				case 0x0006: {
					V[0xF] = V[vy] & 0x01;
					V[vx] = V[vy]>>1;
					break;
				}
				case 0x0007: {
					unsigned char res = V[vy] - V[vx];
					V[0xF] = V[vx] < V[vy] ? 0 : 1;
					V[vx] = res;
					break;
				}
				case 0x000E: {
					V[0xF] = V[vy] & 0x80;
					V[vx] = V[vy]<<1;
					break;
				}
			}
			break;
		}
		case 0x9000: {
			if(V[vx] != V[vy])
				this->pc += 2;
			break;
		}
		case 0xA000: {
			this->I = nnn;
			break;
		}
		case 0xB000: {
			this->pc = nnn + V[0];
			this->pc -= 2;
			break;
		}
		case 0xC000: {
			srand(time(NULL));
			V[vx] = (rand() % 256) ^ nn;
			break;
		}
		case 0xD000: {
			int pos_x, pos_y;
			unsigned short addr = this->I;
			pos_x = V[vx] > 0 ? V[vx] % 64 : 0;
			pos_y = V[vy] > 0 ? V[vy] % 64 : 0;
			V[0xF] = 0x00;
			for(int i = 0; i < n; i++) {
				V[0xF] = 0x01;
				for(int j = 0; j < 8; j++) {
					unsigned char the_bit = (memory[addr+i] & (0x80>>(j-1))) > 0 ? 1 : 0;

					if(display[pos_x+i][pos_y+j] > the_bit)
						V[0xF] = 0x01;
					display[pos_x+i][pos_y+j] = the_bit ^ display[pos_x+i][pos_y+j];
				}
			}
			break;
		}
		case 0xE000: {
			switch(this->opcode & 0x000F) {
				case 0x000E:
					if(retKey() == V[vx])
						this->pc += 2;
					break;
				case 0x0001:
					if(retKey() != V[vx])
						this->pc += 2;
					break;
			}
			break;
		}
		case 0xF000: {
			switch(this->opcode & 0x00FF) {
				case 0x0007:
					V[vx] = delay_timer;
					break;
				case 0x000A: {
					SDL_Event e;
					SDL_PollEvent(&e);
					bool pressed = false;
					while(!pressed) {
						if( e.type == SDL_KEYDOWN ) {
							unsigned char pKey = retKey();
							if(pKey != 0xFF) {
								pressed = true;
								V[vx] = pKey;
							}
						}
					}
					break;
				}
				case 0x0015: {
					this->delay_timer = V[vx];
					break;
				}
				case 0x0018: {
					this->sound_timer = V[vx];
					break;
				}
				case 0x001E: {
					this->I += V[vx];
					break;
				}
				case 0x0029: {
					this->I = V[vx] * 5;
					break;
				}
				case 0x0033: {
					for(int i = 0; i < 3; i++) {
						memory[(this->I)+3-i] = (V[vx] % 10);
						V[vx] = V[vx] / 10;
					}
					break;
				}
				case 0x0055: {

				}
			}

			break;
		}
		default: {
			std::cout << "Unknown opcode: " << this->opcode << std::endl;
			break;
		}
	}
	this->pc += 2;
}

unsigned char Chip8::retKey() {
	SDL_Event e;
	SDL_PollEvent(&e);

	if(e.type == SDL_KEYDOWN)	{
		switch( e.key.keysym.sym ) {
			case SDLK_1: return 0x01;
			case SDLK_2: return 0x02;
			case SDLK_3: return 0x03;
			case SDLK_q: return 0x04;
			case SDLK_w: return 0x05;
			case SDLK_e: return 0x06;
			case SDLK_a: return 0x07;
			case SDLK_s: return 0x08;
			case SDLK_d: return 0x09;
			case SDLK_z: return 0x0A;
			case SDLK_x: return 0x00;
			case SDLK_c: return 0x0B;
			case SDLK_4: return 0x0C;
			case SDLK_r: return 0x0D;
			case SDLK_f: return 0x0E;
			case SDLK_v: return 0x0F;
			default: return 0xFF; // figure something out here
		}
	}
	return 0xFF;
}

void Chip8::clearScreen() {

}
