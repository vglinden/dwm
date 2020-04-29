/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad            = 0;       /* vertical padding of bar */
static const int sidepad            = 0;       /* horizontal padding of bar */
static const char *fonts[]          = { "monospace:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true"  };
static char dmenufont[]             = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       1 << 8,            0,           -1 },
	/* { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 }, */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
#include "gaplessgrid.c"
static const Layout layouts[] = {
	/* symbol	arrange function */
	{ "[]=",	tile },						// 0 tiled (multiple neightboring clients)
	{ "><>",	NULL },						// 1 floating (floating client)
	{ "|M|",	centeredmaster },			// 2 centered master (neightboring clients)
	{ ">M>",	centeredfloatingmaster },	// 3 floating centered master
 	{ "[@]",	spiral },					// 4 fibonacci (neightboring clients)
 	{ "[\]",	dwindle },					// 5 modified fibonacci (neightboring clients)
	{ "###",	gaplessgrid },				// 6 grid layout
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_q,     ACTION##stack, {.i = 0 } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

// regular symbols are defined at /user/include/X11/keysymdef.h
// special symbols are defined at /usr/include/X11/XF86keysym.h

#include <X11/XF86keysym.h>
#include "shiftview.c"
static Key keys[] = {
	TAGKEYS(				XK_1,						0) // using TAGKEYS definition above
	TAGKEYS(				XK_2,						1) // using TAGKEYS definition above
	TAGKEYS(				XK_3,						2) // using TAGKEYS definition above
	TAGKEYS(				XK_4,						3) // using TAGKEYS definition above
	TAGKEYS(				XK_5,						4) // using TAGKEYS definition above
	TAGKEYS(				XK_6,						5) // using TAGKEYS definition above
	TAGKEYS(				XK_7,						6) // using TAGKEYS definition above
	TAGKEYS(				XK_8,						7) // using TAGKEYS definition above
	TAGKEYS(				XK_9,						8) // using TAGKEYS definition above
	STACKKEYS(MODKEY,									focus) // focus on tag
	STACKKEYS(MODKEY|ShiftMask,							push) // push client to tag
	/* modifier				key							function		argument */
	{ MODKEY|ShiftMask,		XK_Escape,					spawn,			SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Close Xorg?\")\" = Yes ] && killall Xorg") },
	{ MODKEY,				XK_apostrophe,				spawn,			SHCMD("dmenuunicode") },
	{ MODKEY,				XK_0,						view,			{.ui = ~0 } }, // displays every open client
	{ MODKEY|ShiftMask,		XK_0,						tag,			{.ui = ~0 } }, // show current client in every tag
	{ MODKEY|ShiftMask,		XK_BackSpace,				spawn,			SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Reboot computer?\")\" = Yes ] && sudo -A reboot") }, // reboot
	{ MODKEY,				XK_Tab,						view,			{0} }, // alternate tags
	{ MODKEY,				XK_q,						killclient,		{0} }, // exit client
	{ MODKEY,				XK_w,						spawn,			SHCMD("$BROWSER") }, // opens browser
	{ MODKEY|ShiftMask,		XK_w,						spawn,			SHCMD("st -e sudo nmtui") }, // opens wifi connector assistance
	{ MODKEY,				XK_r,						spawn,			SHCMD("st -e lf") }, // open file manager
	{ MODKEY,				XK_t,						setlayout,		{.v = &layouts[0]} }, // change layout (see above)
	{ MODKEY,				XK_i,						setlayout,		{.v = &layouts[3]} }, // change layout (see above)
	{ MODKEY,				XK_y,						setlayout,		{.v = &layouts[5]} }, // change layout (see above)
	{ MODKEY,				XK_g,						setlayout,		{.v = &layouts[6]} }, // change layout (see above)
	{ MODKEY,				XK_o,						incnmaster,		{.i = +1 } }, // horizontal mode
	{ MODKEY|ShiftMask,		XK_o,						incnmaster,		{.i = -1 } }, // vertical mode
	{ MODKEY,				XK_p,						spawn,			SHCMD("mpc toggle") }, // music toggle
	{ MODKEY|ShiftMask,		XK_p,						spawn,			SHCMD("mpc pause ; pauseallmpv") }, // music pause
	{ MODKEY,				XK_bracketleft,				spawn,			SHCMD("mpc seek -10") },
	{ MODKEY|ShiftMask,		XK_bracketleft,				spawn,			SHCMD("mpc seek -120") },
	{ MODKEY,				XK_bracketright,			spawn,			SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,		XK_bracketright,			spawn,			SHCMD("mpc seek +120") },
	{ MODKEY,				XK_a,						spawn,			SHCMD("st -e lmc control") }, // opens volume mixer
	{ MODKEY,				XK_s,						togglesticky,	{0} }, // toggle client follow
	{ MODKEY,				XK_d,						spawn,			{.v = dmenucmd } }, // dmenu command
	{ MODKEY|ShiftMask,		XK_d,						togglegaps,		{0} }, // toggle gaps between clients
	{ MODKEY,				XK_z,						incrgaps,		{.i = +1 } }, // increases the gaps between clients
	{ MODKEY|ShiftMask,		XK_z,						incrgaps,		{.i = -1 } }, // decreases the gaps between clients
	{ MODKEY,				XK_f,						togglefullscr,	{0} }, // toggle full screeen
	//{ MODKEY,				XK_g,						shiftview,		{ .i = -1 } }, // go to tab on the left
	//{ MODKEY,				XK_semicolon,				shiftview,		{ .i = 1 } }, // go to tab on the right
	{ MODKEY,				XK_h,						setmfact,		{.f = -0.05} }, // move vertical separator to the left
	{ MODKEY,				XK_l,						setmfact,		{.f = +0.05} }, // move vertical separator to the right
	{ MODKEY,				XK_Return,					spawn,			{.v = termcmd } }, // spawns a terminal
	{ MODKEY,				XK_x,						spawn,			SHCMD("slock & xset dpms force off; mpc pause ; pauseallmpv") }, // locks the screen with password
	{ MODKEY|ShiftMask,		XK_x,						spawn,			SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Shutdown computer?\")\" = Yes ] && sudo -A shutdown -h now") }, // shutdown computer
	{ MODKEY|ShiftMask,		XK_c,						spawn,			SHCMD("mpv --no-cache --no-osc --no-input-default-bindings --input-conf=/dev/null --title=mpvfloat $(ls /dev/video[0,2,4,6,8] | tail -n 1)") }, // opens-up the video player
	{ MODKEY|ShiftMask,		XK_v,						spawn,			SHCMD("{ killall xcompmgr || setsid xcompmgr & } ; xwallpaper --zoom ~/.config/wall.png") }, // reloads the wallpaper
	{ MODKEY,				XK_b,						togglebar,		{0} }, // toggles the upper bar
	{ MODKEY|ShiftMask,		XK_b,						spawn,			SHCMD("hover left") }, // move floating client to the bottom-left corner
	{ MODKEY|ShiftMask,		XK_n,						spawn,			SHCMD("hover right") }, // move floating client to the bottom-right corner
	{ MODKEY,				XK_Page_Up,					shiftview,		{ .i = -1 } },
	{ MODKEY,				XK_Page_Down,				shiftview,		{ .i = 1 } },
	{ MODKEY,				XK_Insert,					spawn,			SHCMD("notify-send \"Clipboard:\" \"$(xclip -o -selection clipboard)\"") }, // shows contents of clipboard
	{ MODKEY,				XK_F1,						spawn,			SHCMD("zathura /usr/local/share/dwm/help.pdf") }, // opens-up help PDF
	//{ MODKEY,				XK_F2,						quit,			{0} },
	{ MODKEY,				XK_F3,						spawn,			SHCMD("displayselect") }, // selects display
	{ MODKEY,				XK_F4,						spawn,			SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Hibernate computer?\")\" = Yes ] && sudo -A zzz") }, // hibernate computer
	{ MODKEY,				XK_F5,						spawn,			SHCMD("cnote") }, // opens notes
	{ MODKEY|ShiftMask,		XK_F5,						spawn,			SHCMD("cnote -c") }, // compiles & opens notes
	{ MODKEY,				XK_F7,						spawn,			SHCMD("td-toggle") }, // Toggles torrent daemon
	{ MODKEY,				XK_F9,						spawn,			SHCMD("dmenumount") }, // interactive mount script
	{ MODKEY,				XK_F10,						spawn,			SHCMD("dmenuumount") }, // interactive unmount script
	{ MODKEY,				XK_space,					zoom,			{0} }, // make the current client the master client
	{ MODKEY|ShiftMask,		XK_space,					togglefloating,	{0} }, // float current client
	{ 0,					XK_Print,					spawn,			SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") }, // prints whole screen to a .ong file
	{ ShiftMask,			XK_Print,					spawn,			SHCMD("maimpick") }, // queries user to select a printscreen mode
	{ MODKEY,				XK_Print,					spawn,			SHCMD("dmenurecord") }, // queria user to select a recording mode
	{ MODKEY|ShiftMask,		XK_Print,					spawn,			SHCMD("dmenurecord kill") }, // kills recording
	{ MODKEY,				XK_Scroll_Lock,				spawn,			SHCMD("killall screenkey || screenkey &") },
	{ 0,					XF86XK_AudioMute,			spawn,			SHCMD("lmc toggle") }, // toggles audio
	{ 0,					XF86XK_AudioRaiseVolume,	spawn,			SHCMD("lmc up 2") }, // increases audio volume
	{ 0,					XF86XK_AudioLowerVolume,	spawn,			SHCMD("lmc down 2") }, // decreases audio volume
	{ 0,					XF86XK_AudioPrev,			spawn,			SHCMD("mpc prev") }, // previous music
	{ 0,					XF86XK_AudioNext,			spawn,			SHCMD("mpc next") }, // next music
	{ 0,					XF86XK_AudioPause,			spawn,			SHCMD("mpc pause") }, // pause music
	{ 0,					XF86XK_AudioPlay,			spawn,			SHCMD("mpc play") }, // play music
	{ 0,					XF86XK_AudioStop,			spawn,			SHCMD("mpc stop") }, // stop music
	{ 0,					XF86XK_AudioRewind,			spawn,			SHCMD("mpc seek -10") }, // rollsback music
	{ 0,					XF86XK_AudioForward,		spawn,			SHCMD("mpc seek +10") }, // forwards music
	{ 0,					XF86XK_AudioMedia,			spawn,			SHCMD("st -e ncmpcpp") }, // toggles the music deamon player?
	{ MODKEY,				XK_m,						spawn,			SHCMD("st -e ncmpcpp") }, // toggles the music deamon player?
	{ MODKEY|ShiftMask,		XK_m,						spawn,			SHCMD("lmc toggle") }, // mute audio
	{ MODKEY,				XK_comma,					spawn,			SHCMD("mpc prev") }, // previous music
	{ MODKEY|ShiftMask,		XK_comma,					spawn,			SHCMD("mpc seek 0%") }, // music restart
	{ MODKEY,				XK_period,					spawn,			SHCMD("mpc next") }, // next music
	{ MODKEY|ShiftMask,		XK_period,					spawn,			SHCMD("mpc repeat") }, // repeat music
	{ 0,					XF86XK_PowerOff,			spawn,			SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Shutdown computer?\")\" = Yes ] && sudo -A shutdown -h now") }, // shutsdown computer
	{ 0,					XF86XK_Calculator,			spawn,			SHCMD("st -e bc -l") }, // calculator
	{ 0,					XF86XK_Sleep,				spawn,			SHCMD("[ \"$(printf \"No\\nYes\" | dmenu -i -nb darkred -sb red -sf white -nf gray -p \"Hibernate computer?\")\" = Yes ] && sudo -A zzz") }, // hibernates computer
	{ 0,					XF86XK_WWW,					spawn,			SHCMD("$BROWSER") }, // opens-up the browser
	{ 0,					XF86XK_DOS,					spawn,			SHCMD("st") }, // opens-up the terminal
	{ 0,					XF86XK_ScreenSaver,			spawn,			SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") }, // locks the screen and stops all media
	{ 0,					XF86XK_TaskPane,			spawn,			SHCMD("st -e htop") }, // opens a terminal, brings htop
	//{ 0,					XF86XK_Mail,				spawn,			SHCMD("st -e neomutt ; pkill -RTMIN+12 dwmblocks") }, // opens neomutt
	{ 0,					XF86XK_MyComputer,			spawn,			SHCMD("st -e lf") }, // opens file explorer at root
	{ 0,					XF86XK_Launch1,				spawn,			SHCMD("xset dpms force off") }, // shutsdown screen??
	{ 0,					XF86XK_TouchpadToggle,		spawn,			SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") }, // toggles touchpad
	{ 0,					XF86XK_TouchpadOff,			spawn,			SHCMD("synclient TouchpadOff=1") }, // enables touchpas
	{ 0,					XF86XK_TouchpadOn,			spawn,			SHCMD("synclient TouchpadOff=0") }, // disables touchpas
	{ 0,					XF86XK_MonBrightnessUp,		spawn,			SHCMD("backlight +100") }, // increases screen brightness (runs script)
	{ 0,					XF86XK_MonBrightnessDown,	spawn,			SHCMD("backlight -100") }, // decreases screen brightness (runs script)
	/* { MODKEY,                       XK_space,  setlayout,      {0} }, */
	/* { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } }, */
	/* { MODKEY,                       XK_period, focusmon,       {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
	/* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

void
setlayoutex(const Arg *arg)
{
	setlayout(&((Arg) { .v = &layouts[arg->i] }));
}

void
viewex(const Arg *arg)
{
	view(&((Arg) { .ui = 1 << arg->ui }));
}

void
viewall(const Arg *arg)
{
	view(&((Arg){.ui = ~0}));
}

void
toggleviewex(const Arg *arg)
{
	toggleview(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagex(const Arg *arg)
{
	tag(&((Arg) { .ui = 1 << arg->ui }));
}

void
toggletagex(const Arg *arg)
{
	toggletag(&((Arg) { .ui = 1 << arg->ui }));
}

void
tagall(const Arg *arg)
{
	tag(&((Arg){.ui = ~0}));
}

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signame> [<type> <value>]"` */
static Signal signals[] = {
	/* signum           function */
	{ "focusstack",     focusstack },
	{ "setmfact",       setmfact },
	{ "togglebar",      togglebar },
	{ "incnmaster",     incnmaster },
	{ "togglefloating", togglefloating },
	{ "focusmon",       focusmon },
	{ "tagmon",         tagmon },
	{ "zoom",           zoom },
	{ "view",           view },
	{ "viewall",        viewall },
	{ "viewex",         viewex },
	{ "toggleview",     view },
	{ "toggleviewex",   toggleviewex },
	{ "tag",            tag },
	{ "tagall",         tagall },
	{ "tagex",          tagex },
	{ "toggletag",      tag },
	{ "toggletagex",    toggletagex },
	{ "killclient",     killclient },
	{ "quit",           quit },
	{ "setlayout",      setlayout },
	{ "setlayoutex",    setlayoutex },
	{ "xrdb",			xrdb },
};


