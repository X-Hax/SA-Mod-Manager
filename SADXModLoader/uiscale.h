#pragma once

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

	extern float scale_min;
	extern float scale_max;
	extern float scale_h;
	extern float scale_v;

	void update_parameters();
	void initialize();
	void setup_background_scale();
	void setup_fmv_scale();
	bool is_top_background();
	bool is_scale_enabled();

	/**
	 * \brief 
	 * \param align The alignment to use for the next drawn elements.
	 * \param is_background If \c true, treat the drawn elements as backgrounds.
	 * \param h Horizontal scale. When in doubt, leave as \c 1.0f
	 * \param v Vertical scale. When in doubt, leave as \c 1.0f
	 */
	void scale_push(Uint8 align, bool is_background, float h = 1.0f, float v = 1.0f);

	void scale_disable();
	void scale_enable();
	void scale_pop();
	float get_scale();

	/**
	 * \brief Calls a trampoline function.
	 * \tparam T Function type
	 * \tparam Args
	 * \param align Alignment mode
	 * \param is_background Enables background scaling mode.
	 * \param pfn The function to call.
	 * \param args Option arguments for function.
	 */
	template <typename T, typename... Args>
	void scale_function(Uint8 align, bool is_background, T* const pfn, Args ... args)
	{
		if (is_background && bg_fill == FillMode::stretch)
		{
			scale_disable();
			pfn(args...);
			scale_enable();
			return;
		}

		scale_push(align, is_background);
		pfn(args...);
		scale_pop();
	}

	/**
	 * \brief Calls a trampoline function with a return type.
	 * \tparam R Return type
	 * \tparam T Function type
	 * \tparam Args
	 * \param align Alignment mode
	 * \param is_background Enables background scaling mode.
	 * \param pfn The function to call.
	 * \param args Optional arguments for function.
	 * \return Return value of function.
	 */
	template <typename R, typename T, typename... Args>
	R scale_function(Uint8 align, bool is_background, T* const pfn, Args ... args)
	{
		if (is_background && bg_fill == FillMode::stretch)
		{
			scale_disable();
			R result = pfn(args...);
			scale_enable();
			return result;
		}

		scale_push(align, is_background);
		R result = pfn(args...);
		scale_pop();
		return result;
	}

	/**
	 * \brief Calls a trampoline function.
	 * \tparam T Function type
	 * \tparam Args
	 * \param align Alignment mode
	 * \param is_background Enables background scaling mode.
	 * \param t Trampoline
	 * \param args Option arguments for function
	 */
	template <typename T, typename... Args>
	void scale_trampoline(Uint8 align, bool is_background, const T&, const Trampoline* t, Args ... args)
	{
		T* const pfn = reinterpret_cast<T*>(t->Target());
		scale_function(align, is_background, pfn, args...);
	}

	/**
	 * \brief Calls a trampoline function with a return type.
	 * \tparam R Return type
	 * \tparam T Function type
	 * \tparam Args
	 * \param align Alignment mode
	 * \param is_background Enables background scaling mode.
	 * \param t Trampoline
	 * \param args Optional arguments for function
	 * \return Return value of trampoline function
	 */
	template <typename R, typename T, typename... Args>
	R scale_trampoline(Uint8 align, bool is_background, const T&, const Trampoline* t, Args ... args)
	{
		T* const pfn = reinterpret_cast<T*>(t->Target());
		return scale_function<R>(align, is_background, pfn, args...);
	}
}
