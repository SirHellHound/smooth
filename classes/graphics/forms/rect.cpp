 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/forms/rect.h>
#include <smooth/misc/math.h>

S::Int	  S::GUI::Rect::Outlined = 0;
S::Int	  S::GUI::Rect::Filled	 = 1;
S::Int	  S::GUI::Rect::Rounded	 = 2;
S::Int	  S::GUI::Rect::Inverted = 4;
S::Int	  S::GUI::Rect::Dotted	 = 8;

S::Bool S::GUI::Rect::DoRectsOverlap(const Rect &rect1, const Rect &rect2)
{
	if ((rect1.left < rect2.right ) && (rect1.right  > rect2.left) &&
	    (rect1.top  < rect2.bottom) && (rect1.bottom > rect2.top )) return True;
	else								return False;
}

S::GUI::Rect S::GUI::Rect::OverlapRect(const Rect &rect1, const Rect &rect2)
{
	if (DoRectsOverlap(rect1, rect2))
	{
		Rect	 oRect;

		oRect.left   = (Int) Math::Max(rect1.left, rect2.left);
		oRect.top    = (Int) Math::Max(rect1.top, rect2.top);
		oRect.right  = (Int) Math::Min(rect1.right, rect2.right);
		oRect.bottom = (Int) Math::Min(rect1.bottom, rect2.bottom);

		return oRect;
	}

	return Rect();
}

S::GUI::Rect S::GUI::Rect::EncloseRect(const Rect &rect1, const Rect &rect2)
{
	Rect	 eRect;

	eRect.left   = (Int) Math::Min(rect1.left, rect2.left);
	eRect.top    = (Int) Math::Min(rect1.top, rect2.top);
	eRect.right  = (Int) Math::Max(rect1.right, rect2.right);
	eRect.bottom = (Int) Math::Max(rect1.bottom, rect2.bottom);

	return eRect;
}
