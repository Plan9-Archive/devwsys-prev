#include <lib9.h>
#include <draw.h>
#include <memdraw.h>
#include <memlayer.h>
#include <cursor.h>
#include "dat.h"
#include "fns.h"
#include "inc.h"
#include "x.h"

#define debugev(...) {if(debug&Debugevent) fprint(2, __VA_ARGS__);}

void configevent(Window*, XEvent);
void exposeevent(Window*, XEvent);
void kbdevent(Window*, XEvent);
void mouseevent(Window*, XEvent);

void
xnextevent(void) {
	int i;
	Window *w;
	Xwin *xw;
	XEvent xev;

	xw = nil;

NextEvent:
	if(!XPending(xconn.display))
		return;
	XNextEvent(xconn.display, &xev);
	if(xev.xany.window == xconn.w){
		if(xev.type == SelectionRequest)
			xselect(&xev);
		goto NextEvent;
	}
	for(i = 0; i < nwindow; i++){
		xw = window[i]->x;
		if(xw && xw->drawable == xev.xany.window)
			break;
	}
	if(i == nwindow || window[i]->deleted)
		goto NextEvent;
	w = window[i];
	switch(xev.type){
	case ClientMessage:
		if(xev.xclient.data.l[0] == xw->wmdelmsg)
			deletewin(w);
		break;

	case ConfigureNotify:
		configevent(w, xev);
		break;

	case Expose:
		exposeevent(w, xev);
		break;
	
	case DestroyNotify:
		/* Nop */
		break;

	case ButtonPress:
	case ButtonRelease:
	case MotionNotify:
		mouseevent(w, xev);
		break;

	case KeyPress:
		kbdevent(w, xev);
		break;
	
	case FocusIn:
		w->current = 1;
	case FocusOut:
		w->current = 0;
		/*
		 * Stop alting when window loses focus.
		 */
		writekbd(w, -1);;
		break;
	case VisibilityNotify:
		if(w == xconn.fullscreen && xev.xvisibility.state != VisibilityUnobscured)
			XRaiseWindow(xconn.display, xw->drawable);	
		break;
	default:
		break;
	}
	goto NextEvent;
}

void
configevent(Window *w, XEvent xev)
{
	int rx, ry;
	Mouse m;
	Rectangle r;
	Xwin *xw;
	XConfigureEvent *xe;

	xe = (XConfigureEvent*)&xev;
	xw = w->x;
	if(!xconn.fullscreen){
		XWindow xwin;
		if(XTranslateCoordinates(xconn.display, xw->drawable, DefaultRootWindow(xconn.display), 0, 0, &rx, &ry, &xwin)) {
			w->orig.x = rx;
			w->orig.y = ry;
		}
	}

	if(Dx(w->screenr) == xe->width && Dy(w->screenr) == xe->height && !w->mouse.resized)
		return;
	if(xe->width != Dx(xconn.screenrect) || xe->height != Dy(xconn.screenrect))
		w->screenr = Rect(rx, ry, rx+xe->width, ry+xe->height);
	r = Rect(0, 0, xe->width, xe->height);

	if(xw->screenpm != xw->nextscreenpm){
		XCopyArea(xconn.display, xw->screenpm, xw->drawable, xconn.gccopy, r.min.x, r.min.y,
			Dx(r), Dy(r), r.min.x, r.min.y);
		XSync(xconn.display, False);
	}
	w->newscreenr = r;

	m.xy.x = Dx(r);
	m.xy.y = Dy(r);
	xreplacescreenimage(w);
	debugev("Configure event at window %d: w=%d h=%d\n", w->id, m.xy.x, m.xy.y);
	writemouse(w, m, 1);
}

void
exposeevent(Window *w, XEvent xev)
{
	XExposeEvent *xe;
	Rectangle r;
	Xwin *xw;

	debugev("Expose event at window %d\n", w->id);
	xw = w->x;
	// qlock(&_x.screenlock);
	if(xw->screenpm != xw->nextscreenpm){
		// qunlock(&_x.screenlock);
		return;
	}
	xe = (XExposeEvent*)&xev;
	r.min.x = xe->x;
	r.min.y = xe->y;
	r.max.x = xe->x+xe->width;
	r.max.y = xe->y+xe->height;
	XCopyArea(xconn.display, xw->screenpm, xw->drawable, xconn.gccopy, r.min.x, r.min.y,
		Dx(r), Dy(r), r.min.x, r.min.y);
	XSync(xconn.display, False);
	// qunlock(&_x.screenlock);
}

void
kbdevent(Window *w, XEvent xev)
{
	KeySym k;

	XLookupString((XKeyEvent*)&xev, NULL, 0, &k, NULL);
	if(k == XK_F11){
		xtogglefullscreen(w);
		if(xconn.fullscreen)
			debugev("Window %d fullscreen\n", w->id);
		return;
	}

	k = xtoplan9kbd(&xev);
	debugev("Keyboard event at window %d. rune=%C (%d)\n", w->id, k, k);
	writekbd(w, k);
}

void
mouseevent(Window *w, XEvent xev)
{
	Mouse m;

	if(xtoplan9mouse(&xev, &m) < 0)
		return;
	if(m.buttons)
		debugev("Mouse event at window %d: x=%d y=%d b=%d\n", w->id, m.xy.x, m.xy.y, m.buttons);
	writemouse(w, m, 0);
}
