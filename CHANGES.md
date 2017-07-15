# Sonic Adventure DX PC Mod Loader
## Changelog

### Version 3.7-17q2 [commits up to 2017/06/30]

* Fix a HUD scaling oversight that could cause sprites to either disappear
  or be locked in the middle of the screen.

* Fixed a performance issue on some systems when window resizing is
  disabled.

* SADX Mod Manager:
  * Added support for changelogs in modular updates.
  * Handle symbolic links correctly.

* Added scaling for menus, which includes the custom background image,
  character select, options, and sound test.

* Fixed a half-pixel offset error caused by SADX adding a half-pixel
  instead of subtracting a half-pixel.

* Scale title cards as well.

* Added options to control FMV, background, and HUD scaling independently.

* Fixed mipmap generation issues.

* Use templates for WriteData() to avoid dynamic array allocations.

### Version 3.7-17q1 [commits up to 2017/03/31]

* Fixed a bug that prevented the Goal Ring mod from loading its PVM.

* Added a code for longer title card display.

* SADX Mod Manager: Added a manifest file to enable DPI awareness.

* Fixed INI data processing for `triallevellist`, `soundtestlist`,
  `musiclist`, `soundlist` and `stringarray` types.

* Use ANSI functions when handling the main SADX window.
  This fixes the window title.
  FIXME: Need to make sure we do this for accelerators if loading
  from the resource file? commit e1bcb951

* Added a mod.ini option to use a custom window title.

* Added a code to enable the unused "Tornado 2" health bar in
  Sky Chase Act 2.

* Upgraded the compiler to MSVC 2017. (was previously MSVC 2015)

* Various updates to SADX Mod Manager, including fixes for the
  automatic updater.

* Make sure texpack folders have `index.txt` before registering them.

* SADX Mod Manager: Added a "Window Resize" checkbox.

* Enabling windowed mode should not make the window Always On Top.

* Allow switching from initial fullscreen mode to windowed mode.

* New HUD scaling infrastructure that rescales UI elements to match
  their expected size when using resolutions larger than 640x480.
  (Currently disabled for Chao Garden due to issues.)

### Version 3.7-16q4 [commits up to 2016/12/31]

* Upgraded the compiler to MSVC 2015. (was previously MSVC 2013)

* Fixed another NPC text INI bug caused by incorrect variable usage.

* SADX Mod Manager:
  * Improve performance of mod.ini searching by stopping immediately
    once mod.ini is found.
  * Added automatic update notifications.

* Texture replacement improvements, including a reference count fix.

* Added more function definitions to SADXFunctions.h.

* Prevent the "Super Sonic Flag" code from taking effect on the
  Character Select screen.

### Version 3.7-16q3 [commits up to 2016/09/30]

* Added proper types to several SADX function declarations.

* Initial C++ classes for encapsulating game objects.
  See GameObject.cpp and GoalRing.cpp in libmodutils.

* Disable the first-person camera when camera controls are disabled.

* Fixed an off-by-one error in recap and NPC text INI replacements.

* Added handling for trampolines on trampolines.

### Version 3.7-16q2 [commits up to 2016/06/30]

* Added support for folder-based sound effect packs.

* Improve sprite alignment in the HUD scaling code.

### Version 3.7-16q1 [commits up to 2016/03/31]

* Added new code types: `writenop`, `writenopreg`

* Added a code for "Disable Debug Spam".

* Increased the maximum mod count from 998 to 999!

* Added support for writing to INI files.

* Added a mipmap blacklist to prevent UI textures from being mipmapped,
  since that results in blurriness in menus and other UI elements.

* Upgraded SADX Mod Manager to .NET 4.0. Replaced MDX with SharpDX.

* Added a V-sync toggle.

* Added a code for Infinite Tails flight.

* Added documentation for codev4 and codev5. (@DankRank)

* Implemented scaling for HUD objects when using resolutions higher
  than 640x480.

* Added lots of new enum, function, struct, and variable definitions
  to their respective SADX header files.

* Moved several classes out of SADX Mod Loader and into a new
  git submodule, `mod-loader-common`.

### Version 3.7 [released 2016/01/17]

* Added support for per-mod code lists.

* Added patches to the code system.

### Version 3.6 [released 2016/01/02]

* Imported the SADX Mod Manager frontend into the SADX Mod Loader repository.

* Added a fix to maintain aspect ratio when playing videos.

* Fixed vertical offsets for Pause sub-menus.

* Fixed video scaling with videos of sizes other than 640x480.

### Version 3.5 [released 2015/12/14]

* Added a toggle for automatic mipmap generation. (default is on)
  Auto Mipmap generation now respects textures that already have mipmaps
  built-in, so it won't re-generate them.

* Added mipmap generation for palettized textures. (default is off)

* Allow exclusive fullscreen on secondary displays if multiple monitors
  are connected.

* Added a toggle for texture filtering.

* Added an option to set a custom window size.

### Version 3.4 [released 2015/09/14]

* Split the dllmain.cpp file into multiple files in order to make it
  easier to maintain.

* Fix the FOV when using widescreen resolutions. When using a widescreen
  resolution, the non-4:3 area will now extend past what you were able to
  see before, instead of simply cropping the top and bottom of the 4:3
  image to fit the screen.

* Added more PVR texture replacement functionality, including support for
  stray textures and subdirectories. Implemented a GBIX cache as well.

* Fixed compilation with MSVC 2015.

### Version 3.3 [released 2015/07/12]

* Added support for DLL data replacement INIs.

* Added a texture cache using PVR Global Indexes. (GBIX)

* Switched from D3D9 to D3D8 for better compatibility with the base
  SADX code, which was written for D3D8.

* Added internal support for extra Dreamcast buttons:
  * C, D, second D-pad

* Added support for texture packs.

* Added switching between fullscreen and windowed modes at runtime.
  This includes the ability to run at any resolution in windowed
  and windowed fullscreen modes, as well as multi-monitor support.

* Upgraded the compiler to MSVC 2013. (was previously MSVC 2010)

* More support for reading data from INI files, including `startpos`,
  `texlist`, `leveltexlist`, and `triallevellist`.

* Removed the CMake build system, since the project will only ever
  be built using MSVC.

### Version 3.2 [released 2015/02/24]

* Added new cheat code types for type conversion.

* Added support for loading data from INI files. This initially only
  supports deathzones.

* New module function API with separated data types.

### Version 3.1 [released 2015/02/08]

* Rewrote the build system using CMake instead of relying on
  Visual Studio projects.

* Refactored much of the source code into separate classes in order to
  reduce the size of dllmain.cpp.

* Initial support for Unicode filenames.

* Split headers into multiple files to make things easier to manage.
  SADXModLoader.h includes all of the individual files, so it's
  effectively the same as before.

* Lots of optimizations and fixes.

* Initial documentation for the binary code file format.

### What's new in Version 3.0?

[this section needs to be filled out later]

### Notes

Release notes for v3.2 and v3.4 through v3.7 are estimated, since these
versions were not been tagged. The tags used (v3.x-estimated) indicate
when the version number was incremented in version.h.

v3.7 was released on 2016/01/17. There hasn't been a version bump since,
so changes since v3.7 are listed at the end of each quarter.

Changes are listed in ascending order, chronologically.
