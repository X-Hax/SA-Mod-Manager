#pragma once

namespace uiscale
{
	enum FillMode : Uint8
	{
		stretch = 0,
		fit     = 1,
		fill    = 2
	};

	extern FillMode bg_fill;
	extern FillMode fmv_fill;

	void update_parameters();
	void initialize();
	void setup_background_scale();
	void setup_fmv_scale();
}
