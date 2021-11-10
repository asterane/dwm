/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=7" };
static const char dmenufont[]       = "monospace:size=7";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

static const char *const autostart[] = {
	"gammastep", NULL,
        "sh", "-c", "while :; do ~/.config/dwm/dwmstatus.sh -; sleep 20; done", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "f", "g", "c", "r", "h", "t", "s", "v", "z" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
        { "zoom",     NULL,       "zoom",     0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* convenience functions */
void
nextlayout(const Arg *arg)
{
	Layout *l;

	for (l=(Layout *)layouts;l != selmon->lt[selmon->sellt]; l++);
	if (l->symbol && (l + 1)->symbol)
		setlayout(&((Arg) { .v = (l + 1) }));
	else
		setlayout(&((Arg) { .v = layouts }));
}

void
prevlayout(const Arg *arg)
{
	Layout *l;

	for (l=(Layout *)layouts; l != selmon->lt[selmon->sellt]; l++);
	if (l != layouts && (l - 1)->symbol)
		setlayout(&((Arg) { .v = (l - 1) }));
	else
		setlayout(&((Arg) { .v = &layouts[LENGTH(layouts) - 2] }));
}

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask,              KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask|ShiftMask,    KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* constant TODO: put on top? */
#define TERM "alacritty"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERM, NULL };

static const char *lockcmd[]  = { "slock", NULL};
static const char *emacscmd[] = { "emacs", NULL};
static const char *webcmd[]   = { "vivaldi-stable", NULL};
static const char *htopcmd[]  = { TERM, "-e", "htop"};

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_m,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_u,      spawn,          {.v = termcmd } },
        { MODKEY,                       XK_l,      spawn,          {.v = lockcmd } },
        { MODKEY,                       XK_e,      spawn,          {.v = emacscmd } },
        { MODKEY,                       XK_w,      spawn,          {.v = webcmd } },
        { MODKEY,                       XK_x,      spawn,          {.v = htopcmd } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_n,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_p,      focusstack,     {.i = -1 } },
        { MODKEY|Mod1Mask,              XK_n,      nextlayout,     {0} },
        { MODKEY|Mod1Mask,              XK_p,      prevlayout,     {0} },

	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_y,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_o,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_a,      zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_i,      incrgaps,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_space,  togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_space,  defaultgaps,    {0} },

	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_k,      killclient,     {0} },
	{ MODKEY,                       XK_semicolon, setlayout,   {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_semicolon, setlayout,   {.v = &layouts[1]} },
	{ MODKEY|Mod1Mask,              XK_semicolon, setlayout,   {.v = &layouts[LENGTH(layouts) - 2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_ampersand, view,        {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_ampersand, tag,         {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	TAGKEYS(                        XK_f,                      0)
	TAGKEYS(                        XK_g,                      1)
	TAGKEYS(                        XK_c,                      2)
	TAGKEYS(                        XK_r,                      3)
	TAGKEYS(                        XK_h,                      4)
	TAGKEYS(                        XK_t,                      5)
	TAGKEYS(                        XK_s,                      6)
	TAGKEYS(                        XK_v,                      7)
	TAGKEYS(                        XK_z,                      8)

	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

        { 0, XF86XK_AudioRaiseVolume,  spawn, SHCMD("amixer -D pulse sset Master 5%+")},
        { 0, XF86XK_AudioLowerVolume,  spawn, SHCMD("amixer -D pulse sset Master 5%-")},
        { 0, XF86XK_AudioMute,         spawn, SHCMD("amixer -D pulse sset Master toggle")},
        { 0, XF86XK_MonBrightnessUp,   spawn, SHCMD("brightnessctl -d 'amdgpu_bl0' s 10%+ -d 'acpi_video0' s 10%+")},
        { 0, XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl -d 'amdgpu_bl0' s 10%- -d 'acpi_video0' s 10%-")},
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

