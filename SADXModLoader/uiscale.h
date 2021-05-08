#pragma once

#include <type_traits>

namespace uiscale
{
	enum Align : Uint8
	{
		automatic,
		horizontal_center = 1 << 0,
		vertical_center   = 1 << 1,
		center            = horizontal_center | vertical_center,
		left              = 1 << 2,
		top               = 1 << 3,
		right             = 1 << 4,
		bottom            = 1 << 5
	};
	
	enum FillMode : Uint8
	{
		stretch = 0,
		fit     = 1,
		fill    = 2
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
	 * \param align The alignment to use for the next drawn elements.
	 * \param is_background If \c true, treat the drawn elements as backgrounds.
	 * \param h Horizontal scale. When in doubt, leave as \c 1.0f
	 * \param v Vertical scale. When in doubt, leave as \c 1.0f
	 */
	void scale_push(Uint8 align, bool is_background, float h = 1.0f, float v = 1.0f);

	void scale_disable();
	void scale_enable();

	/**
	 * \brief Pop a UI scale element off of the top of the scale stack.
	 */
	void scale_pop();

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

		if (is_background && bg_fill == FillMode::stretch)
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
