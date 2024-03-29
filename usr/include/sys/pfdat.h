/* @(#)pfdat.h	1.1 */


typedef struct pfdat {
	ulong           pf_blkno : 24,  /* Disk block nummber.  */
			pf_flags : 8;	/* page flags		*/
	cnt_t		pf_use;		/* share use count	*/
	dev_t		pf_dev;		/* Disk device code.	*/
	char		pf_swpi;	/* Index into swaptab.	*/
	long            pf_inumber;     /* inode number for DBD_FILE */
	struct pfdat	*pf_next;	/* Next free pfdat.	*/
	struct pfdat	*pf_prev;	/* Previous free pfdat.	*/
	struct pfdat	*pf_hchain;	/* Hash chain link.	*/
} pfd_t;

#define	P_QUEUE		0x01	/* Page on free queue		*/
#define	P_BAD		0x02	/* Bad page (ECC error, etc.)	*/
#define	P_HASH		0x04	/* Page on hash queue		*/
#define	P_DONE		0x08	/* IO done on page		*/
#define	P_WANT		0x10	/* page needed			*/

extern struct pfdat phead;
extern struct pfdat pbad;
extern struct pfdat *pfdat;
extern struct pfdat *phash;
extern struct pfdat ptfree;
extern int phashmask;

extern struct pfdat	*pfind();



#define BLKNULL         0L      /* pf_blkno null value          */
