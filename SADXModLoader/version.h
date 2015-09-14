// Current version.
// Make sure this is updated before every release!
#define VERSION_MAJOR 3
#define VERSION_MINOR 4
#define VERSION_BUILD 0
#define VERSION_REVISION 0
#define makeverstr(major,minor) #major "." #minor
#define VERSION_STRING makeverstr(VERSION_MAJOR, VERSION_MINOR)
/*#define VERSION_PRERELEASE 1*/
#define VERSION_WIN32 VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,VERSION_REVISION
