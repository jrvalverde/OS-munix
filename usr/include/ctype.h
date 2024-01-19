/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*#ident  "@(#)head:ctype.h       1.9"  */
/*	3.0 SID #	1.2	*/
#define	_U	01	/* Upper case */
#define	_L	02	/* Lower case */
#define	_N	04	/* Numeral (digit) */
#define	_S	010	/* Spacing character */
#define	_P	020	/* Punctuation */
#define	_C	040	/* Control character */
#define	_B	0100	/* Blank */
#define	_X	0200	/* heXadecimal digit */

#ifndef lint
extern char     _ctype_[];

#define	isalpha(c)	((_ctype_ + 1)[c] & (_U | _L))
#define	isupper(c)	((_ctype_ + 1)[c] & _U)
#define	islower(c)	((_ctype_ + 1)[c] & _L)
#define	isdigit(c)	((_ctype_ + 1)[c] & _N)
#define	isxdigit(c)	((_ctype_ + 1)[c] & _X)
#define	isalnum(c)	((_ctype_ + 1)[c] & (_U | _L | _N))
#define	isspace(c)	((_ctype_ + 1)[c] & _S)
#define	ispunct(c)	((_ctype_ + 1)[c] & _P)
#define	isprint(c)	((_ctype_ + 1)[c] & (_P | _U | _L | _N | _B))
#define	isgraph(c)	((_ctype_ + 1)[c] & (_P | _U | _L | _N))
#define	iscntrl(c)	((_ctype_ + 1)[c] & _C)
#define	isascii(c)	(!((c) & ~0177))
#define	_toupper(c)     ((_ctype_ + 258)[c])
#define	_tolower(c)	((_ctype_ + 258)[c])
#define	toascii(c)	((c) & 0177)
#endif