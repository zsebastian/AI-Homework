#pragma once

#include <SDL.h>
#include <memory>
#include <string>
#include <unordered_map>
#include "Key.h"
#include "Color.h"

class Window
{
public:
	Window(const std::string& title, int w, int h);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	bool Open() const;
	void PollEvents();
	void Display();
	void Clear();
	void PrintScreen(const std::string& file);

	void RenderRectangle(int x, int y, int w, int h, Color color);

	void MapKey(SDL_Keycode sym, std::string str);
	const Key& GetKey(std::string mapping);
	SDL_Surface* NewSurface(int w, int h);
	void DestroySurface(SDL_Surface* surface);

	void Exit();

	void SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
	Uint32 ToPixel(Color color);

	void RenderSurface(SDL_Surface* surface, int x, int y, int w, int h);

private:
	void DrawRectangle(SDL_Surface *surface, int w, int h, Uint32 pixel);
	bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer);
	void HandleEvent(SDL_Event&);
	
	int width_;
	int height_;

	SDL_Window* win_;
	SDL_Renderer* renderer_;
	bool open_;
	std::unordered_map<std::string, Key> keys_;
	std::unordered_map<SDL_Keycode, std::string> keymaps_;
};
