/* #ident  "@(#)curses:screen/curses.ed    1.31" */
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/


/*
 * curses.h - this file is automatically made from caps and
 *	curses.ed. Don't make changes directly to curses.h!
 */
#ifndef CURSES_H

#define CURSES_H	/* define prevents multiple includes */

#include  <stdio.h>

  /*
   * This trick is used to distinguish between SYSV and V7 systems.
   * We assume that L_ctermid is only defined in stdio.h in SYSV
   * systems, but not in V7 or Berkeley UNIX.
   */
#ifdef L_ctermid
# define SYSV
#endif
   /* Some V7 systems define L_ctermid - we list those here */
#ifdef sun
# undef SYSV
#endif

#ifdef SYSV
# ifndef VINTR
#  include <termio.h>
# endif /* VINTR */
    typedef struct termio SGTTY;
#else /* !SYSV */
# ifndef _SGTTYB_
#  include <sgtty.h>
# endif /* _SGTTYB_ */
    typedef struct sgttyb SGTTY;
/*
 * Here we attempt to improve portability by providing some #defines
 * for SYSV functions on non-SYSV systems.
 */

# define memcpy(dst, src, len)	bcopy((src), (dst), (len))
# define strchr			index
# define strrchr		rindex
#endif /* SYSV */

typedef	char bool;

#define _VR3_COMPAT_CODE
/*
 * chtype is the type used to store a character together with attributes.
 * It can be set to "char" to save space, or "long" to get more attributes.
 */
#ifdef	CHTYPE
	typedef	CHTYPE chtype;
#else
	typedef unsigned long chtype;
#endif /* CHTYPE */

/*
	Define for the 'old' definition of chtype is required
	when we are running in compatibility mode
*/
#ifdef _VR3_COMPAT_CODE
typedef unsigned short _ochtype;
#endif

/* TRUE and FALSE get defined so many times, */
/* let's not get in the way of other definitions. */
#if	!defined(TRUE) || ((TRUE) != 1)
#define	TRUE	(1)
#endif
#if	!defined(FALSE) || ((FALSE) != 0)
#define	FALSE	(0)
#endif
#if	!defined(ERR) || ((ERR) != -1)
#define	ERR	(-1)
#endif
#if	!defined(OK) || ((OK) != 0)
#define	OK	(0)
#endif

/* short-hand notations */
typedef struct _win_st	WINDOW;
typedef struct screen	SCREEN;

struct _win_st
{
	short		_cury, _curx;	/* current coordinates */
	short		_maxy, _maxx;	/* max coordinates */
	short		_begy, _begx;	/* (0,0) screen coordinates */
	char		_flags;
	short		_yoffset;	/* actual begy is _begy+_yoffset */
	bool		_clear,	/* clearok() info */
			_leave,	/* leaveok() info */
			_immed,	/* window in immediate mode */
			_sync;		/* auto syncup of parent */
	WINDOW		*_padwin;	/* "pad" area for current window */
#ifdef	_VR3_COMPAT_CODE
	_ochtype	**_y16;	/* MUST stay at this offset in WINDOW */
#endif
	short		*_firstch;	/* first change in line */
	short		*_lastch;	/* last change in line */
	short		_tmarg, _bmarg;	/* scrolling region bounds */
				/* MUST stay at this offset in WINDOW */
	unsigned	_scroll		: 1;	/* scrollok() info */
	unsigned	_use_idl	: 1;	
	unsigned	_use_keypad	: 1;
	unsigned	_notimeout	: 1;
	unsigned	_use_idc	: 1;	
	chtype		_attrs;	/* current window attributes */
	chtype		_bkgd;		/* background, normally blank */
	int		_delay;		/* delay period on wgetch
					 * 0:  for nodelay
					 * <0: for infinite delay
					 * >0: delay time in units of millisec
					 */
	short		_ndescs;	/* number of descendants */
	short		_parx, _pary;	/* coords relative to parent (0,0) */
	WINDOW		*_parent;	/* the parent if this is a subwin */
	chtype		**_y;			/* lines of data */
};

/* _lastch is initially set to this, _firstch is set to win->_maxx */
#define	_NOCHANGE	-1
#define _INFINITY	16000	/* no line can be longer than this */

/* values for win->_flags */
#define	_ISPAD		001
#define	_WINCHANGED	002
#define	_WINMOVED	004
#define	_WINSDEL	010
#define	_CANT_BE_IMMED	020
#define	_WIN_ADD_ONE	040
#define	_WIN_INS_ONE	100

#define STUPID		0
#define UNKNOWN		1
#define CURS_BAD_MALLOC	2

/*
 * Various tricks to shut up lint about things that are perfectly fine.
 */
#if	defined(lint) && !defined(CURSES) /* if not internal to curses source */
 struct screen {
	 int _nobody_;
 };
#endif /* lint */

/* common external variables */

extern	int	LINES, COLS, TABSIZE;

extern	short	curs_errno;

extern	WINDOW	*stdscr, *curscr;

extern	char	ttytype[];

/* Function declarations */

extern	SCREEN	*newscreen(/*termname,lines,cols,tabsiz,fout,fin*/),
		*setcurscreen(/*screenptr*/);

extern	WINDOW	*initscr(),
		*newwin(/*nlines,ncols,begy,begx*/),
		*newpad(/*nlines,ncols*/),
		*derwin(/*orig,nlines,ncols,begy,begx*/),
		*dupwin(/*orig*/),
		*getwin(/*file*/);

extern	chtype	winch();

extern	int	wgetch(); /* because it can return KEY_*, for instance. */

extern	char	*longname(),	/* long name of terminal */
		*termname(),	/* effective terminal name */
		*keyname(/*int*/), /* name of token returned by wgetch() */
		*slk_label(/*index*/),
		curs_parm_err[],
		*curs_err_strings[],
		erasechar(),
		killchar();


extern	void	vidupdate(), wsyncup(), wsyncdown(),
		delkeymap(), 
		_ring(), delscreen(), curserr(),
		_setqiflush(), 
		immedok(), 
		wtimeout(), wbkgdset(),
		wcursyncup();

extern	int	cbreak(), nocbreak(), 
		reset_prog_mode(), reset_shell_mode(), def_prog_mode(),
		_setecho(), _setnonl(),
		def_shell_mode(), raw(),
		savetty(), traceon(), _meta(), 
		traceoff(), noraw(), flushinp(), _getsyx(),
		nodelay(), resetty(), ripoffline(), setsyx(), slk_refresh(),
		slk_restore(), notimeout(), clearok(), leaveok(),
		scrollok(), wstandend(), wstandout(); 

extern	int	crmode(), nocrmode(), ungetch();

#define getsyx(y,x)		_getsyx(&(y),&(x))

/* 
 * Functions to get at the window structure.
 */

#define getyx(win,y,x)		((y) = getcury(win), (x) = getcurx(win))
#define	getbegyx(win,y,x)	((y) = getbegy(win), (x) = getbegx(win))
#define	getmaxyx(win,y,x)	((y) = getmaxy(win), (x) = getmaxx(win))
#define	getparyx(win,y,x)	((y) = getpary(win), (x) = getparx(win))

#if	defined(PERFORMANCE) && !defined(lint)
#define getcury(win)		((win)->_cury)
#define getcurx(win)		((win)->_curx)
#define	getbegy(win)		((win)->_begy)
#define	getbegx(win)		((win)->_begx)
#define	getmaxy(win)		((win)->_maxy)
#define	getmaxx(win)		((win)->_maxx)
#define	getpary(win)		((win)->_pary)
#define	getparx(win)		((win)->_parx)
#define getbkgd(win)		((win)->_bkgd)
#define getattrs(win)		((win)->_attrs)
#endif	/* defined(PERFORMANCE) && !defined(lint) */

#ifdef	_VR3_COMPAT_CODE

#define newterm		newterm32
extern	SCREEN	*newterm(/*termname,fout,fin*/),
		*set_term(/*screenptr*/);

extern	WINDOW	*initscr32(),
		*subwin(/*orig,nlines,ncols,sbegy,sbegx*/),
		*subpad(/*orig,nlines,ncols,begy,begx*/);

extern	_ochtype	*acs_map;

extern	chtype	*acs32map;

/* definitions for Vr3 source compatibility */

#define initscr		initscr32
#define waddch		w32addch
#define wechochar	w32echochar
#define pechochar	p32echochar
#define winsch		w32insch
#define vidputs		vid32puts
#define vidattr		vid32attr
#define wattroff	w32attroff
#define wattron		w32attron
#define wattrset	w32attrset
#define acs_map		acs32map
#define box		box32

#else	/* _VR3_COMPAT_CODE */

extern	chtype	*acs_map;

#endif	/* _VR3_COMPAT_CODE */

extern	int	wattroff(), wattron(), wattrset();

#if	defined(NOMACROS) || defined(lint)

extern	SCREEN	*newterm(/*termname,fout,fin*/),
		*set_term(/*screenptr*/);

extern	WINDOW	*subpad(),
		*subwin();

extern	char	*unctrl();

extern	chtype	inch(),
		mvinch(),
		mvwinch();

extern	int	intrflush(), meta(), noecho(), echo(), nl(), nonl(), beep(),
		flash(), attroff(), attron(), attrset(), standend(),
		standout();

extern	void	bkgdset(), timeout();

#else	/* NOMACROS || lint */

/*
 * The defines from here down to the #endif for NOMACROS
 * define macros, which may be more efficient than their
 * function versions. The functions may be accessed by defining
 * NOMACROS in C code. This is useful for debugging purposes.
 */

#include  <unctrl.h>

/* pseudo functions for stdscr */
#define	addch(ch)	waddch(stdscr, ch)
#define	addnstr(s,n)	waddnstr(stdscr,s,n)
#define	addstr(str)	waddstr(stdscr, str)
#define	attroff(at)	wattroff(stdscr,at)
#define	attron(at)	wattron(stdscr,at)
#define	attrset(at)	wattrset(stdscr,at)
#define	bkgd(c)		wbkgd(stdscr,c)
#define	border(lc,rc,tc,bc)	wborder(stdscr,lc,rc,tc,bc)
#define	clear()		wclear(stdscr)
#define	clrtobot()	wclrtobot(stdscr)
#define	clrtoeol()	wclrtoeol(stdscr)
#define	delch()		wdelch(stdscr)
#define	deleteln()	wdeleteln(stdscr)
#define	echochar(ch)	wechochar(stdscr, ch)
#define	erase()		werase(stdscr)
#define	getch()		wgetch(stdscr)
#define	getstr(str)	wgetstr(stdscr, str)
#define	inch()		winch(stdscr)
#define	insch(c)	winsch(stdscr,c)
#define	insdelln(id)	winsdelln(stdscr,id)
#define	insertln()	winsertln(stdscr)
#define	insnstr(s,n)	winsnstr(stdscr,s,n)
#define	insstr(s)	winsstr(stdscr,s)
#define	move(y, x)	wmove(stdscr, y, x)
#define	refresh()	wrefresh(stdscr)
#define	scrl(n)		wscrl(stdscr,n)
#define	setscrreg(t,b)	wsetscrreg(stdscr, t, b)
#define	standend()	wstandend(stdscr)
#define	standout()	wstandout(stdscr)
#define	timeout(tm)	wtimeout(stdscr,tm)
#define	hline(c,num)	whline(stdscr,c,num)
#define	vline(c,num)	wvline(stdscr,c,num)
#define	addchstr(str)	waddchstr(stdscr,str)
#define	addchnstr(str,n)	waddchnstr(stdscr,str,n)
#define	instr(str)	winstr(stdscr,(str))
#define	innstr(str,n)	winnstr(stdscr,(str),(n))
#define	inchstr(str)	winchstr(stdscr,str)
#define	inchnstr(str,n)	winchnstr(stdscr,str,n)
#define	bkgdset(c)	wbkgdset(stdscr,c)

/* functions to define environment flags of a window */
#ifdef	PERFORMANCE
#define	wbkgdset(win,c)		(((win)->_attrs = (((win)->_attrs & ~((win)->_bkgd & A_ATTRIBUTES)) | (c & A_ATTRIBUTES))), ((win)->_bkgd = (c)))
#define syncok(win,bf)		((win)->_parent?(win)->_sync=(bf):ERR)
#define	notimeout(win,bf)	((win)->_notimeout = ((bf) ? TRUE : FALSE))
#define	clearok(win,bf)		((win)->_clear = (bf))
#define	leaveok(win,bf)		((win)->_leave = (bf))
#define	scrollok(win,bf)	((win)->_scroll = ((bf) ? TRUE : FALSE))
#define	idcok(win,bf)		((win)->_use_idc = ((bf) ? TRUE : FALSE))
#define	nodelay(win,bf)		((win)->_delay = (bf) ? 0 : -1)
#define is_wintouched(win)	((win)->_flags & _WINCHANGED)
#define is_linetouched(win,line)	((((line) >= (win)->_maxy) || \
				     ((line) < 0)) ? ERR : \
				    (((win)->_firstch[(line)] == _INFINITY) ? \
								  FALSE : TRUE))
#endif	/* PERFORMANCE */

/* functions for handling video attributes */

#ifdef	PERFORMANCE
#if	defined(_VR3_COMPAT_CODE)
#undef	wattroff
#undef	wattron
#undef	wattrset
#endif	/* _VR3_COMPAT_CODE */
#define wattroff(w,a)	((w)->_attrs &= ((~(a) | (w)->_bkgd) & A_ATTRIBUTES))
#define wattron(w,a)	((w)->_attrs |= ((a) & A_ATTRIBUTES))
#define wattrset(w,a)	((w)->_attrs = ((a) | (w)->_bkgd) & A_ATTRIBUTES)
#endif	/* PERFORMANCE */

/* functions for move and update */
#define	mvaddch(y,x,ch)		mvwaddch(stdscr,y,x,ch)
#define	mvaddnstr(y,x,s,n)	mvwaddnstr(stdscr,y,x,s,n)
#define	mvaddstr(y,x,str)	mvwaddstr(stdscr,y,x,str)
#define	mvdelch(y,x)		mvwdelch(stdscr,y,x)
#define	mvgetch(y,x)		mvwgetch(stdscr,y,x)
#define	mvgetstr(y,x,str)	mvwgetstr(stdscr,y,x,str)
#define	mvinch(y,x)		mvwinch(stdscr,y,x)
#define	mvinsch(y,x,c)		mvwinsch(stdscr,y,x,c)
#define	mvinsnstr(y,x,s,n)	mvwinsnstr(stdscr,y,x,s,n)
#define	mvinsstr(y,x,s)		mvwinsstr(stdscr,y,x,s)
#define	mvaddchstr(y,x,str)	mvwaddchstr(stdscr,y,x,str)
#define	mvaddchnstr(y,x,str,n)	mvwaddchnstr(stdscr,y,x,str,n)
#define	mvinstr(y,x,str)	mvwinstr(stdscr,y,x,(str))
#define	mvinnstr(y,x,str,n)	mvwinnstr(stdscr,y,x,(str),(n))
#define	mvinchstr(y,x,str)	mvwinchstr(stdscr,y,x,str)
#define	mvinchnstr(y,x,str,n)	mvwinchnstr(stdscr,y,x,str,n)
#define	mvhline(y,x,c,num)	mvwhline(stdscr,y,x,c,num)
#define	mvvline(y,x,c,num)	mvwvline(stdscr,y,x,c,num)

#define	mvwaddch(win,y,x,ch)	(wmove(win,y,x)==ERR?ERR:waddch(win,ch))
#define	mvwaddnstr(win,y,x,s,n)	(wmove(win,y,x)==ERR?ERR:waddnstr(win,s,n))
#define	mvwaddstr(win,y,x,str)	(wmove(win,y,x)==ERR?ERR:waddstr(win,str))
#define	mvwdelch(win,y,x)	(wmove(win,y,x)==ERR?ERR:wdelch(win))
#define	mvwgetch(win,y,x)	(wmove(win,y,x)==ERR?ERR:wgetch(win))
#define	mvwgetstr(win,y,x,str)	(wmove(win,y,x)==ERR?ERR:wgetstr(win,str))
#define	mvwinch(win,y,x)	(wmove(win,y,x)==ERR?(chtype) ERR:winch(win))
#define	mvwinsch(win,y,x,c)	(wmove(win,y,x)==ERR?ERR:winsch(win,c))
#define	mvwinsnstr(win,y,x,s,n)	(wmove(win,y,x)==ERR?ERR:winsnstr(win,s,n))
#define	mvwinsstr(win,y,x,s)	(wmove(win,y,x)==ERR?ERR:winsstr(win,s))
#define	mvwhline(win,y,x,c,num)	(wmove(win,y,x)==ERR?ERR:whline(win,c,num))
#define	mvwvline(win,y,x,c,num)	(wmove(win,y,x)==ERR?ERR:wvline(win,c,num))
#define	mvwaddchstr(win,y,x,str)	(wmove(win,y,x)==ERR?ERR:waddchstr(win,str))
#define	mvwaddchnstr(win,y,x,str,n)	(wmove(win,y,x)==ERR?ERR:waddchnstr(win,str,n))
#define	mvwinstr(win,y,x,str)	(wmove(win,y,x)==ERR?ERR:winstr(win,str))
#define	mvwinnstr(win,y,x,str,n)	(wmove(win,y,x)==ERR?ERR:winnstr(win,str,n))
#define	mvwinchstr(win,y,x,str)	(wmove(win,y,x)==ERR?ERR:winchstr(win,str))
#define	mvwinchnstr(win,y,x,str,n)	(wmove(win,y,x)==ERR?ERR:winchnstr(win,str,n))

#ifdef CURSES_MACROS
#define overlay(src,dst)	_overlap((src),(dst),TRUE)
#define overwrite(src,dst)	_overlap((src),(dst),FALSE)
#define wdeleteln(win)		winsdelln((win),-1)
#define winsertln(win)		winsdelln((win),1)
#define wstandend(win)		wattrset((win),A_NORMAL)
#define wstandout(win)		wattron((win),A_STANDOUT)
#define beep()			_ring(TRUE)
#define flash()			_ring(FALSE)
#define scroll(win)		wscrl((win),1)
#define slk_init(f)		slk_start(((f == 0) ? 3 : 2),NULL)
#define	scr_init(file)		_scr_all(file,0)
#define	scr_restore(file)	_scr_all(file,1)
#define subpad(win,nl,nc,by,bx)	derwin((win),(nl),(nc),(by),(bx))
#define box(win,v,h)		wborder((win),(v),(v),(h),(h),0,0,0,0)
#define newterm(type,fout,fin)	newscreen((type),0,0,0,(fout),(fin))
#define touchline(win,y,n)	wtouchln((win),(y),(n),TRUE)
#endif /* CURSES_MACROS */

#define garbagedlines		wredrawln
#define garbagedwin		redrawwin
#ifdef	PERFORMANCE
#define subwin(win,nl,nc,by,bx)	derwin((win),(nl),(nc),(by-win->_begy),(bx-win->_begx))
#define touchwin(win)		wtouchln((win),0,(win)->_maxy,TRUE)
#define redrawwin(win)		wredrawln((win),0,(win)->_maxy)
#define	winchstr(win,str)	winchnstr((win),str,((win)->_maxx - (win)->_curx))
#define	winstr(win,str)		winnstr((win),str,((win)->_maxx - (win)->_curx))
#define untouchwin(win)		wtouchln((win),0,((win)->_maxy),FALSE)
#define	winch(win)		((win)->_y[(win)->_cury][(win)->_curx])
#endif	/* PERFORMANCE */

#ifdef CURSES_MACROS
#define waddstr(win,str)	waddnstr((win),(str),-1)
#define werase(win)		(wmove((win),0,0), wclrtobot(win))
#define wclear(win)		(clearok((win),TRUE), werase(win))
#define intrflush(win,flag)	_setqiflush(flag)
#define meta(win,flag)		_meta(flag)
#endif /* CURSES_MACROS */

#define crmode			cbreak
#define nocrmode		nocbreak
#define saveterm		def_prog_mode
#define fixterm			reset_prog_mode
#define resetterm		reset_shell_mode

#ifdef CURSES_MACROS
#define setterm(name)		setupterm((name),1,(char*)NULL)
#define gettmode()		(OK)
#define halfdelay(tens)		ttimeout((tens)*100)
#endif /* CURSES_MACROS */

#define	waddchstr(win,str)	waddchnstr((win),(str),-1)
#define winsstr(win,str)	winsnstr((win),(str),-1)

/* iocontrol functions */
#define qiflush()		_setqiflush(TRUE)
#define noqiflush()		_setqiflush(FALSE)

#ifdef CURSES_MACROS
#define echo()			_setecho(TRUE)
#define noecho()		_setecho(FALSE)
#define nl()			_setnonl(FALSE)
#define nonl()			_setnonl(TRUE)
#endif /* CURSES_MACROS */

/* functions for setting time-out length on inputs */
#ifdef	PERFORMANCE
#define wtimeout(win,tm)	((win)->_delay = (tm))
#endif	/* PERFORMANCE */

#define set_term		setcurscreen
#define	scr_set(file)		_scr_all(file,2)

#endif	/* NOMACROS || lint */

/*
 * Standard alternate character set.  The current ACS world is evolving,
 * so we support only a widely available subset: the line drawing characters
 * from the VT100, plus a few from the Teletype 5410v1.  Eventually there
 * may be support of more sophisticated ACS line drawing, such as that
 * in the Teletype 5410, the HP line drawing set, and the like.  There may
 * be support for some non line oriented characters as well.
 *
 * Line drawing ACS names are of the form ACS_trbl, where t is the top, r
 * is the right, b is the bottom, and l is the left.  t, r, b, and l might
 * be B (blank), S (single), D (double), or T (thick).  The subset defined
 * here only uses B and S.
 */

#define ACS_BSSB	(acs_map['l'])
#define ACS_SSBB	(acs_map['m'])
#define ACS_BBSS	(acs_map['k'])
#define ACS_SBBS	(acs_map['j'])
#define ACS_SBSS	(acs_map['u'])
#define ACS_SSSB	(acs_map['t'])
#define ACS_SSBS	(acs_map['v'])
#define ACS_BSSS	(acs_map['w'])
#define ACS_BSBS	(acs_map['q'])
#define ACS_SBSB	(acs_map['x'])
#define ACS_SSSS	(acs_map['n'])

/*
 * Human readable names for the most commonly used characters.
 * "Upper", "right", etc. are chosen to be consistent with the vt100 manual.
 */

#define ACS_ULCORNER	ACS_BSSB
#define ACS_LLCORNER	ACS_SSBB
#define ACS_URCORNER	ACS_BBSS
#define ACS_LRCORNER	ACS_SBBS
#define ACS_RTEE	ACS_SBSS
#define ACS_LTEE	ACS_SSSB
#define ACS_BTEE	ACS_SSBS
#define ACS_TTEE	ACS_BSSS
#define ACS_HLINE	ACS_BSBS
#define ACS_VLINE	ACS_SBSB
#define ACS_PLUS	ACS_SSSS
#define ACS_S1		(acs_map['o'])	/* scan line 1 */
#define ACS_S9		(acs_map['s'])	/* scan line 9 */
#define ACS_DIAMOND	(acs_map['`'])	/* diamond */
#define ACS_CKBOARD	(acs_map['a'])	/* checker board (stipple) */
#define ACS_DEGREE	(acs_map['f'])	/* degree symbol */
#define ACS_PLMINUS	(acs_map['g'])	/* plus/minus */
#define ACS_BULLET	(acs_map['~'])	/* bullet */
	/* Teletype 5410v1 symbols */
#define ACS_LARROW	(acs_map[','])	/* arrow pointing left */
#define ACS_RARROW	(acs_map['+'])	/* arrow pointing right */
#define ACS_DARROW	(acs_map['.'])	/* arrow pointing down */
#define ACS_UARROW	(acs_map['-'])	/* arrow pointing up */
#define ACS_BOARD	(acs_map['h'])	/* board of squares */
#define ACS_LANTERN	(acs_map['i'])	/* lantern symbol */
#define ACS_BLOCK	(acs_map['0'])	/* solid square block */

/* Funny "characters" enabled for various special function keys for input */
/* This list is created from caps and curses.ed. Do not edit it! */
#define KEY_MIN		0401		/* Minimum curses key */
#define KEY_BREAK	0401		/* break key (unreliable) */
#define KEY_DOWN	0402		/* Sent by terminal down arrow key */
#define KEY_UP		0403		/* Sent by terminal up arrow key */
#define KEY_LEFT	0404		/* Sent by terminal left arrow key */
#define KEY_RIGHT	0405		/* Sent by terminal right arrow key */
#define KEY_HOME	0406		/* Sent by home key. */
#define KEY_BACKSPACE	0407		/* Sent by backspace key */
#define KEY_F0		0410		/* function key f0. */
#define KEY_F(n)	(KEY_F0+(n))	/* Space for 64 function keys is reserved. */
#define KEY_DL		0510		/* Sent by delete line key. */
#define KEY_IL		0511		/* Sent by insert line. */
#define KEY_DC		0512		/* Sent by delete character key. */
#define KEY_IC		0513		/* Sent by ins char/enter ins mode key. */
#define KEY_EIC		0514		/* Sent by rmir or smir in insert mode. */
#define KEY_CLEAR	0515		/* Sent by clear screen or erase key. */
#define KEY_EOS		0516		/* Sent by clear-to-end-of-screen key. */
#define KEY_EOL		0517		/* Sent by clear-to-end-of-line key. */
#define KEY_SF		0520		/* Sent by scroll-forward/down key */
#define KEY_SR		0521		/* Sent by scroll-backward/up key */
#define KEY_NPAGE	0522		/* Sent by next-page key */
#define KEY_PPAGE	0523		/* Sent by previous-page key */
#define KEY_STAB	0524		/* Sent by set-tab key */
#define KEY_CTAB	0525		/* Sent by clear-tab key */
#define KEY_CATAB	0526		/* Sent by clear-all-tabs key. */
#define KEY_ENTER	0527		/* Enter/send (unreliable) */
#define KEY_SRESET	0530		/* soft (partial) reset (unreliable) */
#define KEY_RESET	0531		/* reset or hard reset (unreliable) */
#define KEY_PRINT	0532		/* print or copy */
#define KEY_LL		0533		/* Sent by home-down key */
					/* The keypad is arranged like this: */
					/*    a1    up    a3   */
					/*   left   b2  right  */
					/*    c1   down   c3   */
#define KEY_A1		0534		/* Upper left of keypad */
#define KEY_A3		0535		/* Upper right of keypad */
#define KEY_B2		0536		/* Center of keypad */
#define KEY_C1		0537		/* Lower left of keypad */
#define KEY_C3		0540		/* Lower right of keypad */
#define KEY_BTAB	0541		/* Back tab key */
#define KEY_BEG		0542		/* beg(inning) key */
#define KEY_CANCEL	0543		/* cancel key */
#define KEY_CLOSE	0544		/* close key */
#define KEY_COMMAND	0545		/* cmd (command) key */
#define KEY_COPY	0546		/* copy key */
#define KEY_CREATE	0547		/* create key */
#define KEY_END		0550		/* end key */
#define KEY_EXIT	0551		/* exit key */
#define KEY_FIND	0552		/* find key */
#define KEY_HELP	0553		/* help key */
#define KEY_MARK	0554		/* mark key */
#define KEY_MESSAGE	0555		/* message key */
#define KEY_MOVE	0556		/* move key */
#define KEY_NEXT	0557		/* next object key */
#define KEY_OPEN	0560		/* open key */
#define KEY_OPTIONS	0561		/* options key */
#define KEY_PREVIOUS	0562		/* previous object key */
#define KEY_REDO	0563		/* redo key */
#define KEY_REFERENCE	0564		/* ref(erence) key */
#define KEY_REFRESH	0565		/* refresh key */
#define KEY_REPLACE	0566		/* replace key */
#define KEY_RESTART	0567		/* restart key */
#define KEY_RESUME	0570		/* resume key */
#define KEY_SAVE	0571		/* save key */
#define KEY_SBEG	0572		/* shifted beginning key */
#define KEY_SCANCEL	0573		/* shifted cancel key */
#define KEY_SCOMMAND	0574		/* shifted command key */
#define KEY_SCOPY	0575		/* shifted copy key */
#define KEY_SCREATE	0576		/* shifted create key */
#define KEY_SDC		0577		/* shifted delete char key */
#define KEY_SDL		0600		/* shifted delete line key */
#define KEY_SELECT	0601		/* select key */
#define KEY_SEND	0602		/* shifted end key */
#define KEY_SEOL	0603		/* shifted clear line key */
#define KEY_SEXIT	0604		/* shifted exit key */
#define KEY_SFIND	0605		/* shifted find key */
#define KEY_SHELP	0606		/* shifted help key */
#define KEY_SHOME	0607		/* shifted home key */
#define KEY_SIC		0610		/* shifted input key */
#define KEY_SLEFT	0611		/* shifted left arrow key */
#define KEY_SMESSAGE	0612		/* shifted message key */
#define KEY_SMOVE	0613		/* shifted move key */
#define KEY_SNEXT	0614		/* shifted next key */
#define KEY_SOPTIONS	0615		/* shifted options key */
#define KEY_SPREVIOUS	0616		/* shifted prev key */
#define KEY_SPRINT	0617		/* shifted print key */
#define KEY_SREDO	0620		/* shifted redo key */
#define KEY_SREPLACE	0621		/* shifted replace key */
#define KEY_SRIGHT	0622		/* shifted right arrow */
#define KEY_SRSUME	0623		/* shifted resume key */
#define KEY_SSAVE	0624		/* shifted save key */
#define KEY_SSUSPEND	0625		/* shifted suspend key */
#define KEY_SUNDO	0626		/* shifted undo key */
#define KEY_SUSPEND	0627		/* suspend key */
#define KEY_UNDO	0630		/* undo key */
#define KEY_SCLEAR	0631		/* shifted clear key */
#define KEY_SDOWN	0632		/* shifted down arrow key */
#define KEY_SUP		0633		/* shifted up arrow key */
#define KEY_MAX		0777		/* Maximum curses key */

/*
	The definition for 'reg' is not standard, and is provided for
	compatibility reasons. User's are discouraged from using this.
*/
#define	reg	register

/* Various video attributes */
#define A_STANDOUT	000000200000L
#define	_STANDOUT	A_STANDOUT    /* for compatability with old curses */
#define A_UNDERLINE	000000400000L
#define A_REVERSE	000001000000L
#define A_BLINK		000002000000L
#define A_DIM		000004000000L
#define A_BOLD		000010000000L
#define A_ALTCHARSET	000100000000L

/* The next two are subject to change so don't depend on them */
#define A_INVIS		000020000000L
#define A_PROTECT	000040000000L

#define A_NORMAL	000000000000L
#define A_ATTRIBUTES	037777600000L	/* 0xFFFF0000 */
#define A_CHARTEXT	000000177777L	/* 0x0000FFFF */

#endif /* CURSES_H */
