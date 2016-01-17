// Current version.
// Make sure this is updated before every release!
#define VERSION_MAJOR 3
#define VERSION_MINOR 7
#define VERSION_BUILD 0
#define VERSION_REVISION 0
//#define VERSION_PRERELEASE 1

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define makeverstr(major,minor) STRINGIFY(major) "." STRINGIFY(minor)

#define VERSION_STRING makeverstr(VERSION_MAJOR, VERSION_MINOR)
#define VERSION_WIN32 VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,VERSION_REVISION
