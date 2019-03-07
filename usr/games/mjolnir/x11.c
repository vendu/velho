#include <mjolnir/conf.h>

#if (MJOLNIR_X11)

#include <stdlib.h>
#include <stdio.h>

#define DNG_TEXTCELLS 1

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>
#include <corewar/x11.h>
#include <mjolnir/cell.h>

struct cellx11 {
    long                   candraw;
    Display               *disp;
    Pixmap                 pixbuf;
    Window                 mainwin;
    GC                     cavegc;
    GC                     corgc;
    int                    depth;
    Colormap               colormap;
    Visual                *visual;
    int                    screen;
    int                    w;
    int                    h;
    XFontStruct           *font;
    int                    fontw;
    int                    fonth;
    int                    fontasc;
};

static struct cellx11  cellx11;
static struct celldng *testdng;

void
cellbuttonpress(void)
{
    exit(1);
}

void
cellinitx11(struct cellx11 *x11)
{
    Display *disp;

    XInitThreads();
    disp = XOpenDisplay(NULL);
    if (!disp) {
        fprintf(stderr, "failed to open display\n");

        exit(1);
    }
    x11->disp = disp;
    x11->screen = DefaultScreen(disp);
    x11->colormap = DefaultColormap(disp, x11->screen);
    x11->depth = DefaultDepth(disp, x11->screen);
    x11->visual = DefaultVisual(disp, x11->screen);

    return;
}

void
cellinitx11win(struct cellx11 *x11)
{
    XSetWindowAttributes atr = { 0 };
    Window               win;
    Window               parent = RootWindow(x11->disp, x11->screen);

    atr.background_pixel = BlackPixel(x11->disp, x11->screen);
    win = XCreateWindow(x11->disp,
                        parent,
                        0, 0,
                        1024, 768,
                        0,
                        x11->depth,
                        InputOutput,
                        x11->visual,
                        CWBackPixel,
                        &atr);
    if (!win) {
        fprintf(stderr, "failed to create main window\n");

        exit(1);
    }
    x11->mainwin = win;
    x11->w = 1024;
    x11->h = 768;
    XMapRaised(x11->disp, win);

    return;
}

void
cellinitx11font(struct cellx11 *x11)
{
    XFontStruct *font;

    font = XLoadQueryFont(x11->disp, "fixed");
    if (font) {
        x11->font = font;
        x11->fontw = font->max_bounds.width;
        x11->fonth = font->ascent + font->descent;
        x11->fontasc = font->ascent;
    } else {
        fprintf(stderr, "failed to load font\n");

        exit(1);
    }

    return;
}

void
cellinitx11ev(struct cellx11 *x11)
{
    XSelectInput(x11->disp, x11->mainwin, ExposureMask | ButtonPressMask);

    return;
}

void
cellinitx11gc(struct cellx11 *x11)
{
    XGCValues gcval;
    GC        gc;
    XColor    color;

    gcval.foreground = WhitePixel(x11->disp, DefaultScreen(x11->disp));
    gcval.graphics_exposures = False;
    gc = XCreateGC(x11->disp,
                   x11->mainwin,
                   GCForeground,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->cavegc = gc;
    if (!XParseColor(x11->disp,
                     x11->colormap,
                     "green",
                     &color)) {
        fprintf(stderr, "failed to parse color\n");

        exit(1);
    }
    gcval.foreground = color.pixel;
    gc = XCreateGC(x11->disp,
                   x11->mainwin,
                   GCForeground,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->corgc = gc;

    return;
}

void
celldrawx11win(XEvent *ev)
{
    void             *map;
    void             *cormap;
    long              lim1;
    long              lim2;
    long              x;
    long              y;
    long              w = testdng->width;

    map = testdng->map;
    cormap = testdng->cormap;
#if (DNG_PIXELCELLS)
    lim1 = 768;
    lim2 = 1024;
#else
    lim1 = 768 / 8;
    lim2 = 1024 / 8;
#endif
    for (y = 0 ; y < lim1 ; y++) {
        for (x = 0 ; x < lim2 ; x++) {
            if (bitset(map, y * w + x)) {
#if (DNG_TEXTCELLS)
                XDrawString(cellx11.disp,
                            cellx11.mainwin,
                            cellx11.cavegc,
                            x * 8,
                            y * 8,
                            ".",
                            1);
#elif (DNG_PIXELCELLS)
                XDrawPoint(cellx11.disp,
                           cellx11.mainwin,
                           cellx11.cavegc,
                           x, y);
#else
                XDrawRectangle(cellx11.disp,
                               cellx11.mainwin,
                               cellx11.cavegc,
                               x * 8, y * 8,
                               7, 7);
#endif
//                fprintf(stderr, "#");
            } else if (bitset(cormap, y * w + x)) {
#if (DNG_TEXTCELLS)
                XDrawString(cellx11.disp,
                            cellx11.mainwin,
                            cellx11.cavegc,
                            x * 8,
                            y * 8,
                            "#",
                            1);
#elif (DNG_PIXELCELLS)
                XDrawPoint(cellx11.disp,
                           cellx11.mainwin,
                           cellx11.corgc,
                           x, y);
#else
                XDrawRectangle(cellx11.disp,
                               cellx11.mainwin,
                               cellx11.corgc,
                               x * 8, y * 8,
                               7, 7);
#endif
            }
        }
    }
#if 0
    /* draw corridors */
    n = testdng->ncor;
    while (n) {
        n--;
        cor = testdng->cortab[n];
        lim1 = cor->ncell;
        coord = cor->celltab;
        for (id = 0 ; id < lim1 ; id++) {
            x1 = coord->xval;
            y1 = coord->xval;
            XDrawRectangle(cellx11.disp,
                           cellx11.mainwin,
                           cellx11.corgc,
                           x1 * 8, y1 * 8,
                           7, 7);
            coord++;
        }
    }
#endif

    return;
}

int
cellx11loop(struct celldng *dng)
{
    XEvent ev;

    testdng = dng;
    cellinitx11(&cellx11);
    cellinitx11win(&cellx11);
    cellinitx11ev(&cellx11);
    cellinitx11gc(&cellx11);
    while (1) {
        XNextEvent(cellx11.disp, &ev);
        switch (ev.type) {
            case Expose:
                cellx11.candraw = 1;
                celldrawx11win(&ev);

                break;
            case ButtonPress:

                exit(0);
            default:

                break;
        }
    }

    exit(0);
}

struct mjolscrx11 {
    Display *disp;
    Window  *gamewin;
    Window  *msgwin;
    Window  *statwin;
};

int
mjolgetch(void)
{
    XEvent ev;
    int    retval;

    XNextEvent(mjolscrx11.display, &ev);
    if (XFilterEvent(&ev, None)) {
        XUngrabKeyboard(mjolsrc11.display, CurrentTime);
    } else {
        /* FIXME: do the stuff */
    }

    return retval;
}

#endif

