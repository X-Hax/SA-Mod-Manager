#include "stdafx.h"
#include "minmax.h"
#include "input.h"
#include "typedefs.h"
#include "rumble.h"

namespace rumble
{
	struct RumbleTimer
	{
		bool applied;
		Uint8 port;
		Motor motor;
		Sint32 frames;
	};

	static ObjectMaster* instances[GAMEPAD_COUNT][2] = {};

	inline ObjectMaster* get_instance(Uint32 port, Motor motor)
	{
		return instances[port][static_cast<int>(motor) - 1];
	}

	inline void set_instance(Uint32 port, Motor motor, ObjectMaster* ptr)
	{
		const auto i = static_cast<int>(motor) - 1;
		instances[port][i] = ptr;
	}

	Sint32 __cdecl pdVibMxStop_r(Uint32)
	{
		for (auto& i : instances)
		{
			if (i[0])
			{
				DeleteObject_(i[0]);
				i[0] = nullptr;
			}

			if (i[1])
			{
				DeleteObject_(i[1]);
				i[1] = nullptr;
			}
		}

		for (auto& pad : DreamPad::controllers)
		{
			if (pad.active_motor() != Motor::none)
			{
				pad.set_active_motor(Motor::both, false);
			}
		}

		return 0;
	}

	static void __cdecl Rumble_Main_hook(ObjectMaster* _this)
	{
		auto data = static_cast<RumbleTimer*>(_this->UnknownA_ptr);
		auto& pad = DreamPad::controllers[data->port];

		if (data->frames-- <= 0)
		{
			DeleteObject_(_this);
			pad.set_active_motor(data->motor, false);
		}
		else if (!data->applied)
		{
			pad.set_active_motor(data->motor, true);
			data->applied = true;
		}
	}

	static void __cdecl Rumble_Delete(ObjectMaster* _this)
	{
		auto data = static_cast<RumbleTimer*>(_this->UnknownA_ptr);
		_this->UnknownA_ptr = nullptr;

		auto instance = get_instance(data->port, data->motor);
		if (instance == _this)
		{
			set_instance(data->port, data->motor, nullptr);
		}

		delete data;
	}

	static void __cdecl Rumble_Load_hook(Uint32 port, Uint32 time, Motor motor)
	{
		if (port >= GAMEPAD_COUNT)
		{
			for (ushort i = 0; i < GAMEPAD_COUNT; i++)
			{
				Rumble_Load_hook(i, time, motor);
			}
			return;
		}

		auto time_scaled = max(4 * time, static_cast<Uint32>(DreamPad::controllers[port].settings.rumble_min_time / (1000.0f / 60.0f)));
		auto instance = get_instance(port, motor);

		// HACK: Fixes tornado in Windy Valley, allowing it to queue rumble requests and pulse the motor.
		if (!(motor & Motor::small_) && instance != nullptr)
		{
			auto data = static_cast<RumbleTimer*>(instance->UnknownA_ptr);

			data->frames = time_scaled;
			data->applied = false;

			if (FrameCounter % 60 < 2)
			{
				DreamPad::controllers[port].set_active_motor(motor, false);
			}
		}
		else
		{
			auto _this = LoadObject(static_cast<LoadObj>(0), 0, Rumble_Main_hook);

			if (_this == nullptr)
			{
				return;
			}

			set_instance(port, motor, _this);

			auto data = new RumbleTimer;

			data->applied = false;
			data->port = port;
			data->motor = motor;
			data->frames = time_scaled;

			_this->DeleteSub = Rumble_Delete;
			_this->UnknownA_ptr = data;

			if (input::debug)
			{
				auto time_ms = static_cast<Uint32>(time_scaled * (1000.0f / (60.0f / static_cast<float>(FrameIncrement))));
				PrintDebug("[Input] [%u] Rumble %u: %s, %u frames (%ums)\n",
					FrameCounter, data->port, (motor == Motor::small_ ? "R" : "L"), time_scaled, time_ms);
			}
		}
	}

	/**
	 * \brief Rumbles the large motor. Used for things like explosions, springs, dash panels, etc.
	 * \param port Controller port.
	 * \param time Time to rumble.
	 */
	void __cdecl RumbleA_r(Uint32 port, Uint32 time)
	{
		if (RumbleEnabled && !DemoPlaying && input::controller_enabled[port])
		{
			Rumble_Load_hook(port, clamp(time, 1u, 255u), Motor::large);
		}
	}

	/**
	 * \brief Rumbles the small motor. Used for things like taking damage.
	 * \param port Controller port.
	 * \param time Time to rumble.
	 * \param a3 Unknown.
	 * \param a4 Unknown.
	 */
	void __cdecl RumbleB_r(Uint32 port, Uint32 time, int a3, int a4)
	{
		if (RumbleEnabled && !DemoPlaying && input::controller_enabled[port])
		{
			Uint32 idk = time;
			if (static_cast<signed int>(time) <= 4)
			{
				if (static_cast<signed int>(time) >= -4)
				{
					if (time == 1)
					{
						idk = 2;
					}
					else if (static_cast<Sint32>(time) == -1)
					{
						idk = -2;
					}
				}
				else
				{
					idk = -4;
				}
			}
			else
			{
				idk = 4;
			}

			int _a3 = a3;
			if (a3 <= 59)
			{
				if (a3 < 7)
				{
					_a3 = 7;
				}
			}
			else
			{
				_a3 = 59;
			}

			int _time = a4 * _a3 / static_cast<signed int>(4 * idk);
			if (_time <= 0)
			{
				_time = 1;
			}

			Rumble_Load_hook(port, _time, Motor::small_);
		}
	}

	// ReSharper disable once CppDeclaratorNeverUsed
	static const auto loc_0042D534 = reinterpret_cast<const void*>(0x0042D534);

	/**
	 * \brief Enables rumble by default when a new save file is created.
	 */
	void __declspec(naked) default_rumble()
	{
		__asm
		{
			mov[esp + 26Ah], 1
			jmp loc_0042D534
		}
	}
}
