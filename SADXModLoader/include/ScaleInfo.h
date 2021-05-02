#pragma once

namespace uiscale
{

	// Alignement of scaled sprites (top left by default)
	enum Align : Uint8
	{
		automatic         = 0,      // Guess the anchor based on where the sprite is drawn on screen.
		horizontal_center = 1 << 0,
		vertical_center	  = 1 << 1,
		center            = horizontal_center | vertical_center,
		left              = 1 << 2,
		top               = 1 << 3,
		right             = 1 << 4,
		bottom            = 1 << 5
	};

	// Filling method of scaled sprites
	enum FillMode : Uint8
	{
		stretch = 0, // Fill screen by stretching the frame to the sides
		fit     = 1, // Fit the scaled frame into the screen
		fill    = 2  // Scale the frame until it fills the screen
	};
}