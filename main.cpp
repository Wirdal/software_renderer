#include <iostream>

#include "SDL3/SDL_init.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_timer.h"

#include "src/math/evector.h"
#include "src/math/matrix.h"
#include "src/math/transformation_matrix.h"


int main()
{
	SDL_Init(0);
	// Store window dimensions so we can map NDC -> screen
	const int screenW = 800;
	const int screenH = 400;

	SDL_Window *window = SDL_CreateWindow("MyWindow", screenW, screenH, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, nullptr);

	// Define a triangle in model space (local coordinates)
	// Define an equilateral triangle centered around the origin.
	// Using side length = 1.0, height h = sqrt(3)/2. Coordinates chosen so centroid is at (0,0,0):
	// Top vertex: (0, 2/3*h), Bottom-left: (-1/2, -1/3*h), Bottom-right: (1/2, -1/3*h)
	constexpr float side = 1.0f;
	constexpr float h = 0.8660254037844386f; // sqrt(3)/2
	evector<float, 3> triangleModel[3] = {
		{0.0f, (2.0f/3.0f) * h, 0.0f},
		{-0.5f * side, (-1.0f/3.0f) * h, 0.0f},
		{0.5f * side, (-1.0f/3.0f) * h, 0.0f}
	};

	// Compute the centroid of the model and translate the model so its centroid
	// sits at the world origin (0,0,0). This ensures that a rotation applied
	// around the world origin behaves like a wheel spinning about its center
	// rather than orbiting around the origin.
	{
		float cx = 0.0f, cy = 0.0f, cz = 0.0f;
		for (int i = 0; i < 3; ++i)
		{
			cx += triangleModel[i][0];
			cy += triangleModel[i][1];
			cz += triangleModel[i][2];
		}
		cx /= 3.0f; cy /= 3.0f; cz /= 3.0f;

		for (int i = 0; i < 3; ++i)
		{
			triangleModel[i][0] -= cx;
			triangleModel[i][1] -= cy;
			triangleModel[i][2] -= cz;
		}
	}


	// Place the triangle's model in the world at the origin (0,0,0)
	// World transform is identity for this example (no translation needed).
	const matrix<4,4,float> worldTransform = matrix<4,4,float>::identity();

	// Camera is positioned at (0,0,-1) looking toward +Z in this simple setup.
	evector<float,3> cameraPosition{0.0f, 0.0f, -1.0f};

	// Angle (radians) used to rotate the model each frame. We rotate clockwise, so we subtract the delta.
	float angle = 0.0f;
	const float angleDelta = 0.02f; // radians per frame

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		// Clear the screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Poll events first
		while (SDL_PollEvent(&event) && !quit)
		{
			switch(event.type)
			{
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					SDL_MouseButtonEvent& mouseEvent = (SDL_MouseButtonEvent&) event;
					std::cout << "x: "<<  mouseEvent.x  << " y: " << mouseEvent.y << std::endl;
					break;
				}
				case SDL_EVENT_QUIT:
				{
					quit = true;
					break;
				}
				default: break;
			}
		}

		// Update model rotation (clockwise)
		angle -= angleDelta;

		// ROTATION: build a rotation matrix for Z.
		// NOTE: to rotate around the world origin (0,0,0) we must apply this rotation in *world space* —
		// i.e. after the model has been placed into the world. Previously we applied rotation as part
		// of the model transform which rotates in model-local space. That causes the triangle to orbit
		// if the model's local origin isn't at the world origin. By applying rotation after the
		// `worldTransform` we guarantee rotation around the true world origin.
		matrix<4,4,float> rotationWorld = matrix<4,4,float>::rotation_z(angle);

		// VIEW: transform world -> camera space by applying the inverse camera transform.
		// Our camera transform would normally be a translation by cameraPosition; to move the world
		// into camera space we translate by -cameraPosition. Build this matrix manually here
		// (keep changes local to main.cpp per request).
		matrix<4,4,float> view = matrix<4,4,float>::identity();
		view[3][0] = -cameraPosition[0];
		view[3][1] = -cameraPosition[1];
		view[3][2] = -cameraPosition[2];

		// Transform each vertex through: model -> world -> view -> NDC (normalized device coordinates) -> screen
		// We'll perform a simple orthographic projection mapping X/Y from [-1,1] to screen pixels.
		// NOTE: To avoid apparent "stretching" while rotating when the window is not square,
		// we must use a uniform pixels-per-unit scale for both X and Y. Previously we mapped
		// X -> [0, screenW] and Y -> [0, screenH] independently which causes non-uniform scaling
		// when screenW != screenH. Here we compute pixelsPerUnit from the smaller screen dimension
		// so one model unit maps to the same number of pixels in X and Y, preserving shape.
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		float sx_f[3];
		float sy_f[3];

		// pixels per unit (use smaller dimension to preserve aspect)
		const float pixelsPerUnit = (screenW < screenH ? static_cast<float>(screenW) : static_cast<float>(screenH)) * 0.5f;
		const float screenCenterX = static_cast<float>(screenW) * 0.5f;
		const float screenCenterY = static_cast<float>(screenH) * 0.5f;

		for (int i = 0; i < 3; ++i)
		{
			// extend to homogeneous coordinate
			evector<float,4> v4{ triangleModel[i][0], triangleModel[i][1], triangleModel[i][2], 1.0f };

			// Model -> World: place the model in the world first
			matrix<1,4,float> v_world = v4.transpose() * worldTransform;

			// WORLD ROTATION: now rotate in world space around the world origin
			v_world = v_world * rotationWorld;

			// World -> Camera
			matrix<1,4,float> v_camera = v_world * view;

			// Now perform a simple orthographic projection (drop Z)
			const float nx = v_camera[0][0]; // normalized x in model units
			const float ny = v_camera[0][1]; // normalized y in model units

			// Map normalized coordinates [-1,1] to screen pixels using a uniform scale
			sx_f[i] = screenCenterX + nx * pixelsPerUnit;
			// Flip Y because SDL's origin is top-left
			sy_f[i] = screenCenterY - ny * pixelsPerUnit;
		}

		// Draw triangle edges (SDL3 uses SDL_RenderLine with float coordinates)
		SDL_RenderLine(renderer, sx_f[0], sy_f[0], sx_f[1], sy_f[1]);
		SDL_RenderLine(renderer, sx_f[1], sy_f[1], sx_f[2], sy_f[2]);
		SDL_RenderLine(renderer, sx_f[2], sy_f[2], sx_f[0], sy_f[0]);

		// Draw a debug circle around the world origin (0,0,0).
		// This is useful to visually confirm the triangle rotates exactly around the world origin.
		// The circle lives in world space (not rotated with the model). We'll sample it and
		// map the points through world->view->screen the same way we map triangle vertices.
		const int circleSegments = 64;
		const float circleRadius = 0.75f; // world units
		float prev_x = 0.0f, prev_y = 0.0f;
		float first_x = 0.0f, first_y = 0.0f;
		for (int s = 0; s <= circleSegments; ++s)
		{
			const float t = static_cast<float>(s) / static_cast<float>(circleSegments);
			const float theta = t * 2.0f * 3.14159265358979323846f;

			// point in world space on the circle (z=0)
			evector<float,4> pc{ circleRadius * std::cos(theta), circleRadius * std::sin(theta), 0.0f, 1.0f };

			// world -> camera (worldTransform is identity)
			matrix<1,4,float> p_world = pc.transpose() * worldTransform;
			matrix<1,4,float> p_camera = p_world * view;

			const float nx = p_camera[0][0];
			const float ny = p_camera[0][1];

			const float sx_c = screenCenterX + nx * pixelsPerUnit;
			const float sy_c = screenCenterY - ny * pixelsPerUnit;

			if (s == 0)
			{
				first_x = sx_c; first_y = sy_c;
			}
			else
			{
				SDL_RenderLine(renderer, prev_x, prev_y, sx_c, sy_c);
			}

			prev_x = sx_c; prev_y = sy_c;
		}


		SDL_RenderPresent(renderer);

	// Small delay to cap CPU usage (approx ~60fps)
	SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}