#include <iostream>

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"

#include "src/math/evector.h"

template<size_t N, typename T>
static void rotate_a_little(SDL_Vertex(&vertices)[N])
{
	// Make a rotation matrix
	for (size_t i = 0; i < N; i++)
	{

	}
}

int main()
{
	SDL_Init(0);

	SDL_Window *window = SDL_CreateWindow("MyWindow", 800, 400, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

	// Pyramid is currently in "model space"
	// I need to position it somewhere in screen space?

	// Well, how the heck do I draw a face first
	SDL_Vertex triangle[3] = {};

	// This is the triangle in world space, no need to xform to world space
	triangle[0].position.x = 100;
	triangle[0].position.y = 100;
	triangle[0].color.r = 255;

	triangle[1].position.x = 200;
	triangle[1].position.y = 200;
	triangle[1].color.g = 255;

	triangle[2].position.x = 100;
	triangle[2].position.y = 200;
	triangle[2].color.b = 255;

	// Lets place it right at 0,0,0
	evector<float,3> origin{0.0f, 0.0f, 0.0f};
	
	// Now we need to translate the pyramid to the origin

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_RenderClear(renderer);

		// Transform to screen space, then draw

		SDL_RenderGeometry(renderer, NULL, triangle, 3, NULL, 0);

		SDL_RenderPresent(renderer);

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