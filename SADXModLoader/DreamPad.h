#pragma once

// This class is based loosely on this gist: https://gist.github.com/urkle/6701236
// That code doesn't actually compile due to name discrepancies, but it was a good starting point.

// XInput default deadzones blatantly copied, pasted, and renamed.
#define GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define GAMEPAD_TRIGGER_THRESHOLD    30
// Limit is now 8, the maximum supported by the game (depending on where you look; sometimes it's 4). XInput is no longer a limiting factor.
#define GAMEPAD_COUNT 8

#include "SDL.h"
#include <SADXModLoader.h>
#include "typedefs.h"
#include "KeyboardMouse.h"


enum Motor : int8
{
	none,
	large,
	small_,
	both
};

class DreamPad
{
	static KeyboardMouse keyboard;

	ControllerData dc_pad = {};

	SDL_GameController* gamepad = nullptr;

	int controller_id_ = -1;

	bool  connected_ = false;
	bool  e_held = false;
	bool  half_press = false;
	Motor rumble_state = Motor::none;

	ushort large_magnitude = 0;
	ushort small_magnitude = 0;

	float normalized_l_ = 0.0f;
	float normalized_r_ = 0.0f;

public:
	static DreamPad controllers[GAMEPAD_COUNT];

	DreamPad(DreamPad&& other) noexcept;
	DreamPad(const DreamPad& other) = delete;
	DreamPad() = default;
	~DreamPad();

	DreamPad& operator=(DreamPad&& other) noexcept;
	DreamPad& operator=(const DreamPad& other) = delete;

	/**
	 * \brief Opens the specified controller ID.
	 * \param id Controller ID to open.
	 * \return \a true on success. Note that haptic can fail and the function will still return true.
	 */
	bool open(int id);

	/**
	 * \brief Closes this instance.
	 */
	void close();

	/**
	 * \brief Polls input for this instance.
	 */
	void poll();

	Motor active_motor() const;
	void  set_active_motor(Motor motor, bool enable);
	bool  connected() const;
	bool  e_held_pad() const;
	int   controller_id() const;
	float normalized_l() const;
	float normalized_r() const;

	// SDL -> Dreamcast converted input data (ControllerData).
	const ControllerData& dreamcast_data() const;

	struct Settings
	{
		Settings();

		bool   allow_keyboard;
		short  deadzone_l;        // Left stick deadzone
		bool   radial_l;          // Indicates if the stick is fully radial or semi-radial
		short  deadzone_r;        // Right stick deadzone
		bool   radial_r;          // Indicates if the stick is fully radial or semi-radial
		uint8  trigger_threshold; // Trigger threshold
		float  rumble_factor;     // Rumble intensity multiplier (1.0 by default)
		bool   mega_rumble;       // Always fire both motors
		ushort rumble_min_time;   // Minimum rumble time for controllers that have issues

		void set_deadzone_l(short deadzone);
		void set_deadzone_r(short deadzone);
	} settings;

	/**
	 * \brief Converts from SDL (-32768 to 32767) to Dreamcast (-127 to 127) axes, including scaled deadzone.
	 * \param dest The destination axes (Dreamcast).
	 * \param source The source axes (SDL).
	 * \param deadzone The deadzone.
	 * \param radial If set to \a true, the deadzone is treated as fully radial. (i.e one axis exceeding deadzone implies the other)
	 * \return The axis magnitude.
	 */
	static float convert_axes(NJS_POINT2I* dest, const NJS_POINT2I& source, short deadzone, bool radial);

	/**
	 * \brief Updates the various button fields in the provided
	 * Dreamcast controller structure with the provided buttons.
	 * \param pad The Dreamcast controller data to update.
	 * \param buttons The buttons to add.
	 */
	static void update_buttons(ControllerData& pad, Uint32 buttons);

private:
	void move_from(DreamPad&& other);
};

enum PDD_DEV_SUPPORT : uint32_t
{
	//	Right stick Y
	PDD_DEV_SUPPORT_AY2 = (1 << 21),
	//	Right stick X
	PDD_DEV_SUPPORT_AX2 = (1 << 20),
	//	Left stick Y
	PDD_DEV_SUPPORT_AY1 = (1 << 19),
	//	Left stick X
	PDD_DEV_SUPPORT_AX1 = (1 << 18),
	//	Analog trigger L
	PDD_DEV_SUPPORT_AL = (1 << 17),
	//	Analog trigger R
	PDD_DEV_SUPPORT_AR = (1 << 16),
	//	D-Pad B Right
	PDD_DEV_SUPPORT_KRB = (1 << 15),
	//	D-Pad B Left
	PDD_DEV_SUPPORT_KLB = (1 << 14),
	//	D-Pad B Down
	PDD_DEV_SUPPORT_KDB = (1 << 13),
	//	D-Pad B Up
	PDD_DEV_SUPPORT_KUB = (1 << 12),
	//	D button
	PDD_DEV_SUPPORT_TD = (1 << 11),
	//	X button
	PDD_DEV_SUPPORT_TX = (1 << 10),
	//	Y button
	PDD_DEV_SUPPORT_TY = (1 << 9),
	//	Z button
	PDD_DEV_SUPPORT_TZ = (1 << 8),
	//	D-Pad A Right
	PDD_DEV_SUPPORT_KR = (1 << 7),
	//	D-Pad A Left
	PDD_DEV_SUPPORT_KL = (1 << 6),
	//	D-Pad A Down
	PDD_DEV_SUPPORT_KD = (1 << 5),
	//	D-Pad A Up
	PDD_DEV_SUPPORT_KU = (1 << 4),
	//	Start button
	PDD_DEV_SUPPORT_ST = (1 << 3),
	//	A button
	PDD_DEV_SUPPORT_TA = (1 << 2),
	//	B button
	PDD_DEV_SUPPORT_TB = (1 << 1),
	//	C button
	PDD_DEV_SUPPORT_TC = (1 << 0),
};
