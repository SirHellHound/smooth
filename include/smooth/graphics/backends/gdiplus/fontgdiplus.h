 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_FONTGDIPLUS
#define H_OBJSMOOTH_FONTGDIPLUS

#include <windows.h>

namespace smooth
{
	namespace GUI
	{
		class FontGDIPlus;
	};
};

#include "../fontbackend.h"

namespace smooth
{
	namespace GUI
	{
		const Short	 FONT_GDIPLUS	= 5;

		class FontGDIPlus : public FontBackend
		{
			public:
				static Int	 Initialize();

						 FontGDIPlus(const String &, Short, Short, Short, const Color &);
						~FontGDIPlus();

				Size		 GetTextSize(const String &) const;
		};
	};
};

#endif
