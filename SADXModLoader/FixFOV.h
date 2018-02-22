#pragma once

namespace fov
{
	void check_aspect_ratio();
	void initialize();

	/**
	 * \brief Gets the current unmodified horizontal field of view provided by the game code.
	 */
	Angle get_fov();
}
