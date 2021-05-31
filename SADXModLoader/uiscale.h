#pragma once

#include <type_traits>
#include "../include/ninja.h"

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
		 * \brief Automatically align the canvas to 
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

	extern FillMode bg_fill;
	extern FillMode fmv_fill;

	/**
	 * \brief The minimum of the horizontal and vertical scale factors.
	 */
	extern float scale_min;

	/**
	 * \brief The maximum of the horizontal and vertical scale factors.
	 */
	extern float scale_max;

	/**
	 * \brief Horizontal UI scale factor relative to 640x480.
	 */
	extern float scale_h;

	/**
	 * \brief Vertical UI scale factor relative to 640x480.
	 */
	extern float scale_v;

	void update_parameters();
	void initialize_common();
	void initialize();
	void setup_background_scale();
	void setup_fmv_scale();
	bool is_top_background();
	bool is_scale_enabled();
	void scale_texmemlist(NJS_TEXTURE_VTX* list, Int count);

	/**
	 * \brief Push a UI scale element onto the scale stack.
	 * \param align Combination of \sa Align bits to align the next drawn UI elements.
	 * \param is_background If \c true, treat the drawn elements as backgrounds.
	 * \param h Horizontal scale. When in doubt, leave as \c 1.0f
	 * \param v Vertical scale. When in doubt, leave as \c 1.0f
	 */
	void scale_push(Uint8 align, bool is_background, float h = 1.0f, float v = 1.0f);

	/**
	 * \brief Disable UI scaling for the next drawn UI element.
	 * Must have matching call to scale_enable()
	 */
	void scale_disable();

	/**
	 * \brief Enable UI scaling for the next drawn UI element.
	 */
	void scale_enable();

	/**
	 * \brief Pop a UI scale element off of the top of the scale stack.
	 */
	void scale_pop();

	/**
	 * \brief Get the current uniform scale factor for UI elements.
	 */
	float get_scale();

	/**
	 * \brief Calls a function with UI scaling enabled.
	 * \tparam T Function type.
	 * \tparam Args Optional arguments for the function.
	 * \param align Alignment mode.
	 * \param is_background Enables background scaling mode.
	 * \param pfn The function to call.
	 * \param args Optional arguments for the function.
	 */
	template <typename T, typename... Args>
	auto scale_function(Uint8 align, bool is_background, T* const pfn, Args ... args)
	{
		constexpr bool return_void = std::is_void<std::invoke_result_t<T, Args...>>::value;

		if (is_background && bg_fill == FillMode_Stretch)
		{
			scale_disable();

			if constexpr (return_void)
			{
				pfn(args...);
				scale_enable();
				return;
			}
			else
			{
				typename std::invoke_result<T, Args...>::type result = pfn(args...);
				scale_enable();
				return result;
			}
		}

		scale_push(align, is_background);

		if constexpr (return_void)
		{
			pfn(args...);
			scale_pop();
		}
		else
		{
			typename std::invoke_result<T, Args...>::type result = pfn(args...);
			scale_pop();
			return result;
		}
	}

	/**
	 * \brief Calls a trampoline function with UI scaling enabled.
	 * \tparam T Function type.
	 * \tparam Args Optional arguments for the function.
	 * \param align Alignment mode.
	 * \param is_background Enables background scaling mode.
	 * \param t Trampoline
	 * \param args Optional arguments for the function.
	 */
	template <typename T, typename... Args>
	auto scale_trampoline(Uint8 align, bool is_background, const T&, const Trampoline* t, Args ... args)
	{
		T* const pfn = reinterpret_cast<T*>(t->Target());
		return scale_function(align, is_background, pfn, args...);
	}
}
