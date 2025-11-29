#include <iostream>

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"

#include "src/math/evector.h"

int main()
{
	SDL_Init(0);

	SDL_Window *window = SDL_CreateWindow("MyWindow", 800, 400, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

	// Pyramid is currently in "model space"
	// I need to position it somewhere in screen space?

	// Well, how the heck do I draw a face first
	SDL_Vertex triangle[3] = {};

	triangle[0].position.x = 100;
	triangle[0].position.y = 100;
	triangle[0].color.r = 255;

	triangle[1].position.x = 200;
	triangle[1].position.y = 200;
	triangle[0].color.g = 255;

	triangle[2].position.x = 100;
	triangle[2].position.y = 200;
	triangle[0].color.b = 255;

	// I could figure out all of the possible spots inbetween my triangle and draw that, at least...

	// So, can I loop over every single pixel in the window buffer? Seems a little overkill

	// Lets define a pyramid using evectors
	// We have a point at the top, and a square at the bottom
	// It also needs to be in model space first, we will xform it later
	// So really we need a list of points
	evector<float, 3> pyramid[5]
	{
		{0.0f, 1.0f, 0.0f}, // Top point
		{-1.0f, -1.0f, 1.0f}, // Front left
		{1.0f, -1.0f, 1.0f}, // Front right
		{-1.0f, -1.0f, -1.0f}, // Back left
		{1.0f, -1.0f, -1.0f} // Back right
	};

	// Since this is in model space, we need to transform it to world space, then view space, then screen space
	// Lets place it right at 0,0,0
	evector<float,3> origin{0.0f, 0.0f, 0.0f};
	
	// Now we need to translate the pyramid to the origin

	bool quit = false;
	SDL_Event e;
	while (!quit)
	{

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_RenderClear(renderer);

		SDL_RenderGeometry(renderer, NULL, triangle, 3, NULL, 0);

		SDL_RenderPresent(renderer);

		// SDL3 click thing that will print to stdout where the coord are
		// TODO
		// Transform them to screenspace?
		SDL_Event event;
		while (SDL_PollEvent(&event) && !quit)
		{
			switch(event.type)
			{
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					SDL_MouseButtonEvent& mouseEvent = (SDL_MouseButtonEvent&) event;
					std::cout << "x: "<<  mouseEvent.x  << " y: " << mouseEvent.y << std::endl;
				}
							// Listen for escape
				case SDL_EVENT_QUIT:
				{
					quit = true;
				}
			}
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}