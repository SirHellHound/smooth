 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/toolwindow.h>
#include <smooth/window/window.h>
#include <smooth/window/windowbackend.h>
#include <smooth/object.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/i18n.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>
#include <smooth/layer.h>
#include <smooth/misc/math.h>
#include <smooth/color.h>

const S::Int	 S::GUI::ToolWindow::classID = S::Object::RequestClassID();

S::GUI::ToolWindow::ToolWindow() : Window("smooth ToolWindow")
{
	type				= classID;
	containerType			= Window::classID;

	objectProperties->orientation	= OR_FREE;

	owner				= NIL;

	possibleContainers.RemoveAll();
	possibleContainers.AddEntry(Window::classID);

	SetFlags(WF_TOPMOST | WF_NOTASKBUTTON | WF_THINBORDER);
}

S::GUI::ToolWindow::~ToolWindow()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::ToolWindow::SetOwner(Widget *newOwner)
{
	owner = newOwner;

	return Success;
}

S::Int S::GUI::ToolWindow::FreeOwner()
{
	owner = NIL;

	return Success;
}

S::Int S::GUI::ToolWindow::Paint(Int message)
{
	if (!registered)	return Error;
	if (!created)		return Success;
	if (!visible)		return Success;

	EnterProtectedRegion();

	Surface	*surface = GetDrawSurface();

	if (message == SP_UPDATE)
	{
		surface->PaintRect(updateRect);
	}
	else
	{
		Int	 color = CombineColor(192, 192, 192);

#ifdef __WIN32__
		color = GetSysColor(COLOR_BTNFACE);
#endif

		surface->Box(updateRect, color, FILLED);

		onPaint.Emit();

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			Object	*object = assocObjects.GetNthEntry(i);

			if (object->GetObjectType() == Widget::classID)
			{
				((Widget *) object)->Paint(SP_PAINT);
			}
		}
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ToolWindow::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	EnterProtectedRegion();

	if (!(message == SM_MOUSEMOVE && wParam == 1)) onEvent.Emit(message, wParam, lParam);

#ifdef __WIN32__
	switch (message)
	{
		case WM_CLOSE:
			if (doQuit.Call())
			{
				backend->Close();
			}

			LeaveProtectedRegion();

			return 0;
		case WM_DESTROY:
			destroyed = True;

			if (nOfActiveWindows == 0 && loopActive)
			{
				if (S::Setup::enableUnicode)	::SendMessageW((HWND) backend->GetSystemWindow(), WM_QUIT, 0, 0);
				else				::SendMessageA((HWND) backend->GetSystemWindow(), WM_QUIT, 0, 0);
			}
			else
			{
				nOfActiveWindows--;
			}

			LeaveProtectedRegion();

			return 0;
		case WM_QUIT:
			destroyed = True;

			nOfActiveWindows--;

			LeaveProtectedRegion();

			PostQuitMessage(0);

			return 0;
		case WM_PAINT:
			{
				RECT	 uRect = { 0, 0, 0, 0 };

				updateRect = uRect;

				if (::GetUpdateRect((HWND) backend->GetSystemWindow(), &uRect, 0))
				{
					updateRect = uRect;

					updateRect.right	+= 5;
					updateRect.bottom	+= 5;

					PAINTSTRUCT	 ps;

					BeginPaint((HWND) backend->GetSystemWindow(), &ps);

					if (Math::Abs((updateRect.right - updateRect.left) - objectProperties->size.cx) < 20 && Math::Abs((updateRect.bottom - updateRect.top) - objectProperties->size.cy) < 20)	Paint(SP_PAINT);
					else																						Paint(SP_UPDATE);

					EndPaint((HWND) backend->GetSystemWindow(), &ps);
				}
			}

			LeaveProtectedRegion();

			return 0;
		case WM_WINDOWPOSCHANGED:
			{
				WINDOWPOS	*wndPos = (LPWINDOWPOS) lParam;
				Object		*object = NIL;

				for (Int i = 0; i < GetNOfObjects(); i++)
				{
					object = assocObjects.GetNthEntry(i);

					if (object->GetObjectProperties()->orientation == OR_CENTER)
					{
						object->GetObjectProperties()->size.cx = wndPos->cx;
						object->GetObjectProperties()->size.cy = wndPos->cy;
					}
				}

				objectProperties->pos.x		= wndPos->x;
				objectProperties->pos.y		= wndPos->y;
				objectProperties->size.cx	= wndPos->cx;
				objectProperties->size.cy	= wndPos->cy;

				drawSurface->SetSize(objectProperties->size);
			}

			updateRect.left		= 0;
			updateRect.top		= 0;
			updateRect.right	= updateRect.left + objectProperties->size.cx;
			updateRect.bottom	= updateRect.top + objectProperties->size.cy;

			break;
	}
#endif

	for (Int j = GetNOfObjects() - 1; j >= 0; j--)
	{
		Object	*object = assocObjects.GetNthEntry(j);

		if (object->GetObjectType() == Widget::classID)
		{
			if (((Widget *) object)->Process(message, wParam, lParam) == Break)
			{
				LeaveProtectedRegion();

				return Break;
			}
		}
	}

	LeaveProtectedRegion();

	if (owner != NIL) owner->Process(message, wParam, lParam);
	
	return Success;
}

S::Int S::GUI::ToolWindow::RegisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);

			object->SetContainer(this);
			object->SetRegisteredFlag();

			switch (object->GetObjectProperties()->orientation)
			{
				case OR_CENTER:
					object->GetObjectProperties()->pos.x	= 0;
					object->GetObjectProperties()->pos.y	= 0;

					object->GetObjectProperties()->size.cy	= objectProperties->size.cy;
					object->GetObjectProperties()->size.cx	= objectProperties->size.cx;

					break;
			}

			if (object->GetObjectType() == ToolWindow::classID)
			{
				if (Setup::rightToLeft)	object->GetObjectProperties()->pos.x = objectProperties->size.cx - ((object->GetObjectProperties()->pos.x - objectProperties->pos.x) + object->GetObjectProperties()->size.cx) + objectProperties->pos.x;
				((ToolWindow *) object)->Create();
			}

			if (object->GetObjectType() == Widget::classID)
			{
				((Widget *) object)->onRegister.Emit(this);
				((Widget *) object)->Show();
			}

			return Success;
		}
	}
	else
	{
		return mainLayer->RegisterObject(object);
	}

	return Error;
}

S::Bool S::GUI::ToolWindow::IsTypeCompatible(Int compType)
{
	if (compType == Object::classID || compType == Widget::classID || compType == Window::classID)	return True;
	else												return False;
}
