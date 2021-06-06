#pragma once

namespace uiscale
{
	/**
	 * \brief Alignment of the canvas used for scaled UI elements.
	 */
	enum Align : Uint8
	{
		/**
		 * \brief Use default alignment behavior (top left).
		 */
		Align_Default = 0,

		/**
		 * \brief Align the canvas to the left side of the screen.
		 */
		Align_Left = 1 << 1,

		/**
		 * \brief Align the canvas to the right side of the screen.
		 */
		Align_Right = 1 << 2,

		/**
		 * \brief Align the canvas to the center of the screen horizontally.
		 */
		Align_Center_Horizontal = Align_Left | Align_Right,

		/**
		 * \brief Automatically align the canvas to a horizontal third of the
		 * screen based on the horizontal placement of the UI element.
		 */
		Align_Automatic_Horizontal = 1 << 3,

		/**
		 * \brief Align the canvas to the top of the screen.
		 */
		Align_Top = 1 << 4,

		/**
		 * \brief Align the canvas to the bottom of the screen.
		 */
		Align_Bottom = 1 << 5,

		/**
		 * \brief Align the canvas to the center of the screen vertically.
		 */
		Align_Center_Vertical = Align_Top | Align_Bottom,

		/**
		 * \brief Automatically align the canvas to a vertical third of the
		 * screen based on the vertical placement of the UI element.
		 */
		Align_Automatic_Vertical = 1 << 6,

		/**
		 * \brief Align the canvas to the center of the screen horizontally
		 * and vertically.
		 */
		Align_Center = Align_Center_Horizontal | Align_Center_Vertical,

		/**
		 * \brief Automatically align the canvas to horizontal and vertical thirds
		 * of the screen based on the placement of the UI element.
		 */
		Align_Automatic = Align_Automatic_Horizontal | Align_Automatic_Vertical
	};

	// The order of the values in this enum are important for ensuring
	// compatibility with the Mod Loader's configuration file. Do not
	// change these values without also updating the mod manager!
	enum FillMode : Uint8
	{
		/**
		 * \brief Stretch the canvas to match the screen size.
		 */
		FillMode_Stretch = 0,

		/**
		 * \brief Scale the canvas to the screen maintaining aspect ratio,
		 * displaying letter or pillar boxes where necessary.
		 */
		FillMode_Fit = 1,

		/**
		 * \brief Scale the canvas maintaining aspect ratio, and crops to
		 * fill all edges of the screen to eliminate letter or pillar boxing.
		 */
		FillMode_Fill = 2
	};
}