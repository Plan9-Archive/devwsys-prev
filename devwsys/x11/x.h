typedef struct Xconn Xconn;
typedef struct Xmem Xmem;
typedef struct Xwin Xwin;

struct Xconn {
	ulong		chan;
	XColormap	cmap;
	XDisplay		*display;
	int			fd;		/* of display */
	int			depth;	/* of screen */
	XColor		map[256];
	XColor		map7[128];
	uchar		map7to8[128][2];
	int			toplan9[256];
	int			tox11[256];
	int			usetable;
	Rectangle		screenrect;
	XScreen		*screen;
	XVisual		*vis;
	XWindow		root;
};

struct Xmem
{
	int		pixmap;	/* pixmap id */
	XImage	*xi;		/* local image */
	int		dirty;	/* is the X server ahead of us?  */
	Rectangle	dirtyr;	/* which pixels? */
	Rectangle	r;		/* size of image */
	Window	*w;		/* associated window */
};

struct Xwin {
	Atom 		wmdelmsg;
	XCursor		cursor;
	XDrawable	drawable;
	XDrawable	screenpm;
	XDrawable	nextscreenpm;
	XGC			gccopy;
	XGC			gccopy0;
	XGC			gcfill;
	ulong		gcfillcolor;
	XGC			gcfill0;
	ulong		gcfill0color;
	XGC			gcreplsrc;
	ulong		gcreplsrctile;
	XGC			gcreplsrc0;
	ulong		gcreplsrc0tile;
	XGC			gcsimplesrc;
	ulong		gcsimplesrccolor;
	ulong		gcsimplesrcpixmap;
	XGC			gcsimplesrc0;
	ulong		gcsimplesrc0color;
	ulong		gcsimplesrc0pixmap;
	XGC			gczero;
	ulong		gczeropixmap;
	XGC			gczero0;
	ulong		gczero0pixmap;
};

extern Xconn xconn;

int xtoplan9kbd(XEvent*);
int xtoplan9mouse(XEvent*, Mouse*);
int xcloadmemimage(Memimage*, Rectangle, uchar*, int);
void xfillcolor(Memimage*, Rectangle, ulong);
XImage* xgetxdata(Memimage*, Rectangle);
int xreplacescreenimage(Window*);
void xmovewindow(Window*, Rectangle);
void xputxdata(Memimage*, Rectangle);
void xmemimagedraw(Memimage*, Rectangle, Memimage*, Point,
	Memimage*, Point, int);
void xinitclipboard(void);
void xsetsnarfowner(Window*);
void xselect(XEvent*);
