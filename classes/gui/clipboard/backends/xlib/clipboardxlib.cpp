 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/clipboard/backends/xlib/clipboardxlib.h>
#include <smooth/gui/window/backends/xlib/windowxlib.h>
#include <smooth/backends/xlib/backendxlib.h>
#include <smooth/gui/window/window.h>

using namespace X11;

S::GUI::ClipboardBackend *CreateClipboardXLib(S::GUI::Window *window)
{
	return new S::GUI::ClipboardXLib(window);
}

S::Int	 clipboardXLibTmp = S::GUI::ClipboardBackend::SetBackend(&CreateClipboardXLib);

S::GUI::ClipboardXLib::ClipboardXLib(Window *iWindow)
{
	type	= CLIPBOARD_XLIB;

	display	= Backends::BackendXLib::GetDisplay();
	window	= iWindow;
}

S::GUI::ClipboardXLib::~ClipboardXLib()
{
}

unsigned char *S::GUI::ClipboardXLib::QueryAtom(Display *display, X11::Window self, Atom clipboard, Atom atom) const
{
	XConvertSelection(display, clipboard, atom, atom, self, CurrentTime);
	XFlush(display);

	/* Wait for SelectionNotify event to be sent.
	 */
	XEvent	 e;

	do
	{
		XNextEvent(display, &e);

		WindowXLib	*backend = GUI::WindowXLib::GetWindowBackend(e.xany.window);

		if (backend != NIL) backend->ProcessSystemMessages(&e);
	}
	while (e.type != SelectionNotify);

	Atom		 type;
	int		 format;
	unsigned long	 items, bytes;
	unsigned char	*data = NIL;

	/* Do not get any data yet, see how much data is there.
	 */
	if (XGetWindowProperty(display, self, atom, 0, 0, 0, AnyPropertyType, &type, &format, &items, &bytes, &data) == 0)
	{
		if (data != NIL) { XFree(data); data = NIL; }

		/* Data is there!
		 */
		if (bytes > 0) XGetWindowProperty(display, self, atom, 0, bytes, 0, AnyPropertyType, &type, &format, &items, &bytes, &data);
	}

	return data;
}

S::String S::GUI::ClipboardXLib::GetText(Atom clipboard) const
{
	if (window == NIL) return NIL;

	String		 text;

	X11::Window	 self  = (X11::Window) window->GetSystemWindow();
	X11::Window	 owner = XGetSelectionOwner(display, clipboard);

	if (owner != None)
	{
		unsigned char *data = QueryAtom(display, self, clipboard, XInternAtom(display, "UTF8_STRING", True));

		if (data != NIL)
		{
			text.ImportFrom("UTF-8", (char *) data);

			XFree(data);
		}
		else
		{
			unsigned char *data = QueryAtom(display, self, clipboard, XA_STRING);

			if (data != NIL)
			{
				text = (char *) data;

				XFree(data);
			}
		}
	}

	return text;
}

S::Bool S::GUI::ClipboardXLib::SetText(Atom clipboard, const String &text)
{
	if (window == NIL) return False;

	X11::Window	 self	 = (X11::Window) window->GetSystemWindow();
	WindowXLib	*backend = GUI::WindowXLib::GetWindowBackend(self);

	if (backend != NIL)
	{
		if	(clipboard == XA_PRIMARY)			       backend->SetSelection(text);
		else if (clipboard == XInternAtom(display, "CLIPBOARD", True)) backend->SetClipboard(text);
	}

	XSetSelectionOwner(display, clipboard, self, CurrentTime);
	XFlush(display);

	return True;
}

S::String S::GUI::ClipboardXLib::GetSelectionText() const
{
	return GetText(XA_PRIMARY);
}

S::Bool S::GUI::ClipboardXLib::SetSelectionText(const String &text)
{
	return SetText(XA_PRIMARY, text);
}

S::String S::GUI::ClipboardXLib::GetClipboardText() const
{
	if (display == NIL) return NIL;

	return GetText(XInternAtom(display, "CLIPBOARD", True));
}

S::Bool S::GUI::ClipboardXLib::SetClipboardText(const String &text)
{
	if (display == NIL) return False;

	return SetText(XInternAtom(display, "CLIPBOARD", True), text);
}
