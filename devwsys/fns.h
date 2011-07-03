#define fatal(...) sysfatal("devwsys: fatal: " __VA_ARGS__)
#define debug(...) if(debuglevel) fprint(2, "devwsys: " __VA_ARGS__)

/* Devdraw */
Client* drawnewclient(Draw*);
Memimage* drawinstall(Client*, int, Memimage*, DScreen*);
void drawfree(Client*);
int drawattach(Window*, char*);
int readdrawctl(char*, Client*);
int readrefresh(char*, long, Client*);
int drawmesg(Client*, void*, int);
char* drawerr(void);

/* Keyboard */
void writekbd(Window*, int);
void readkbd(Window*, void*);

/* Mouse */
void writemouse(Window*, Mouse, int);
void readmouse(Window*, void*);

/* Reqbuf */
void addreq(Reqbuf*, void*);
void* nextreq(Reqbuf *reqs);

/* Window */
void deletewin(Window*);
Window* newwin(char*, char*);
int parsewinsize(char*, Rectangle*, int*);
void drawreplacescreenimage(Window*, Memimage*);
void setlabel(Window*, char*);

/* Ixp */
void ixprread(void*, char*);
int ixpserve(char*);

/* X connection */
int xinit(void);
int xfd(void);
void xnextevent(void);
void xclose(void);

/* X window */
int xattach(Window*, char*);
// Memimage* xallocmemimage(Window*, Rectangle, ulong, int);
Memimage* xallocmemimage(Window*, Rectangle, ulong, int, void**);
void xfreememimage(Memimage*);
void xdeletewin(Window*);
void xflushmemscreen(Window*, Rectangle);
int xreplacescreenimage(Window*);
int xupdatelabel(Window*);
void xmovewindow(Window*, Rectangle);
