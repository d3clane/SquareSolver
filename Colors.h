/// \file
/// \brief File contains console collors defines

#define REDTEXT      "\x1b[31;1m"
#define GREENTEXT    "\x1b[32;1m"
#define YELLOWTEXT   "\x1b[33;1m"
#define BLUETEXT     "\x1b[34;1m"
#define MAGENTATEXT  "\x1b[35;1m"
#define CYANTEXT     "\x1b[36;1m"
#define WHITETEXT    "\x1b[37;1m"
#define STDTEXT      "\x1b[30;0m"

#define REDCONSOLE      "\x1b[41;1m"
#define GREENCONSOLE    "\x1b[42;1m"
#define YELLOWCONSOLE   "\x1b[43;1m"
#define BLUECONSOLE     "\x1b[44;1m"
#define MAGENTACONSOLE  "\x1b[45;1m"
#define CYANCONSOLE     "\x1b[46;1m"
#define WHITECONSOLE    "\x1b[47;1m"
#define STDCONSOLE      "\x1b[40;0m"

#define RedText(X)     REDTEXT X STDTEXT
#define GreenText(X) GREENTEXT X STDTEXT
#define ColorText(Color, X) Color X STDTEXT

#define SetRedConsole printf(REDCONSOLE)
#define SetGreenConsole printf(GREENCONSOLE)
#define SetColorConsole(Color) printf(Color)
#define SetStandardConsole printf(STDCONSOLE)

#define PrintRedText(STRING_FORMATER, ...)   printf(REDTEXT STRING_FORMATER STDTEXT, __VA_ARGS__)
#define PrintGreenText(STRING_FORMATER, ...) printf(GREENTEXT STRING_FORMATER STDTEXT, __VA_ARGS__)
#define PrintColorText(Color, STRING_FORMATER, ...) printf(Color STRING_FORMATER STDTEXT, __VA_ARGS__)

// TODO: 1) color lib +
//       2) swap +
//       3) error and flag(struct + void*) +
//       4) type naming + 
//       5) make: +doxygen +objects +build 