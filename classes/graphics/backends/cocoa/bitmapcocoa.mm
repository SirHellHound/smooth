 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/graphics/backends/cocoa/bitmapcocoa.h>

S::GUI::BitmapBackend *CreateBitmapCocoa_pV(S::Void *iBitmap)
{
	return new S::GUI::BitmapCocoa(iBitmap);
}

S::GUI::BitmapBackend *CreateBitmapCocoa_III(S::Int cx, S::Int cy, S::Int bpp)
{
	return new S::GUI::BitmapCocoa(cx, cy, bpp);
}

S::GUI::BitmapBackend *CreateBitmapCocoa_cI(const int null)
{
	return new S::GUI::BitmapCocoa(null);
}

S::GUI::BitmapBackend *CreateBitmapCocoa_crS(const S::GUI::BitmapBackend &iBitmap)
{
	return new S::GUI::BitmapCocoa((const S::GUI::BitmapCocoa &) iBitmap);
}

S::Int	 bitmapCocoaTmp_pV	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapCocoa_pV);
S::Int	 bitmapCocoaTmp_III	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapCocoa_III);
S::Int	 bitmapCocoaTmp_cI	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapCocoa_cI);
S::Int	 bitmapCocoaTmp_crS	= S::GUI::BitmapBackend::SetBackend(&CreateBitmapCocoa_crS);

S::GUI::BitmapCocoa::BitmapCocoa(Void *iBitmap)
{
	Initialize();

	SetSystemBitmap(iBitmap);
}

S::GUI::BitmapCocoa::BitmapCocoa(Int cx, Int cy, Int bpp)
{
	Initialize();

	CreateBitmap(cx, cy, bpp);
}

S::GUI::BitmapCocoa::BitmapCocoa(const int null)
{
	Initialize();

	SetSystemBitmap(NIL);
}

S::GUI::BitmapCocoa::BitmapCocoa(const BitmapCocoa &iBitmap)
{
	Initialize();

	SetSystemBitmap((Void *) iBitmap.bitmap);

	depth = iBitmap.depth;
}

S::GUI::BitmapCocoa::~BitmapCocoa()
{
	DeleteBitmap();
}

S::Void S::GUI::BitmapCocoa::Initialize()
{
	type	= BITMAP_COCOA;
	bitmap	= NIL;
}

S::Bool S::GUI::BitmapCocoa::CreateBitmap(Int cx, Int cy, Int bpp)
{
	DeleteBitmap();

	if (bpp == -1)		    bpp = [[NSScreen mainScreen] depth];
	if (bpp != 24 && bpp != 32) bpp = 24;

	bitmap	= [[[NSBitmapImageRep alloc] initWithBitmapDataPlanes: nil
							   pixelsWide: cx
							   pixelsHigh: cy
							bitsPerSample: 8
						      samplesPerPixel: bpp == 32 ? 4 : 3
							     hasAlpha: bpp == 32
							     isPlanar: NO
						       colorSpaceName: NSCalibratedRGBColorSpace
							 bitmapFormat: 0
							  bytesPerRow: ((4 - ((cx * (bpp == 32 ? 4 : 3)) & 3)) & 3) + cx * (bpp == 32 ? 4 : 3)
							 bitsPerPixel: bpp] retain];

	if (bitmap == NIL) return False;

	bytes	= [bitmap bitmapData];

	size	= Size(cx, cy);
	depth	= bpp;

	align	= cx * (bpp / 8);

	return True;
}

S::Bool S::GUI::BitmapCocoa::DeleteBitmap()
{
	if (bitmap != NIL)
	{
		[bitmap release];

		bitmap	= NIL;

		size	= Size(0, 0);
		depth	= 0;

		bytes	= NIL;
		align	= 0;
	}

	return True;
}

S::Bool S::GUI::BitmapCocoa::SetSystemBitmap(Void *nBitmap)
{
	if (nBitmap == GetSystemBitmap()) return True;

	if (nBitmap == NIL)
	{
		DeleteBitmap();
	}
	else
	{
		CreateBitmap([((NSBitmapImageRep *) nBitmap) pixelsWide], [((NSBitmapImageRep *) nBitmap) pixelsHigh], [((NSBitmapImageRep *) nBitmap) bitsPerPixel]);

		/* Copy source bitmap to destination.
		 */
		NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];

		for (Int x = 0; x < size.cx; x++)
		{
			for (Int y = 0; y < size.cy; y++)
			{
				[bitmap setColor: [((NSBitmapImageRep *) nBitmap) colorAtX: x y: y] atX: x y: y];
			}
		}

		[pool release];
	}

	return True;
}

S::Void *S::GUI::BitmapCocoa::GetSystemBitmap() const
{
	return (Void *) bitmap;
}

S::Bool S::GUI::BitmapCocoa::SetPixel(const Point &point, const Color &color)
{
	if (bytes == NIL)			      return False;
	if (point.y >= size.cy || point.x >= size.cx) return False;

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - point.y - 1) * (((4 - ((size.cx * 3) & 3)) & 3) + size.cx * 3) + point.x * 3;

			data[offset + 2] = (color >> 16) & 255;
			data[offset + 1] = (color >>  8) & 255;
			data[offset + 0] =  color	 & 255;

			return True;
		case 32:
			offset = (size.cy - point.y - 1) * (				      size.cx * 4) + point.x * 4;

			data[offset + 2] = (color >> 16) & 255;
			data[offset + 1] = (color >>  8) & 255;
			data[offset + 0] =  color	 & 255;
			data[offset + 3] = (color >> 24) & 255;

			return True;
	}

	return False;
}

S::GUI::Color S::GUI::BitmapCocoa::GetPixel(const Point &point) const
{
	if (bytes == NIL)			      return 0;
	if (point.y >= size.cy || point.x >= size.cx) return 0;

	UnsignedByte	*data	= ((UnsignedByte *) bytes);
	Int		 offset = 0;

	switch (depth)
	{
		case 24:
			offset = (size.cy - point.y - 1) * (((4 - ((size.cx * 3) & 3)) & 3) + size.cx * 3) + point.x * 3;

			return Color(			      data[offset + 2] << 16 | data[offset + 1] << 8 | data[offset + 0]);
		case 32:
			offset = (size.cy - point.y - 1) * (				      size.cx * 4) + point.x * 4;

			return Color(data[offset + 3] << 24 | data[offset + 2] << 16 | data[offset + 1] << 8 | data[offset + 0]);
	}

	return 0;
}

S::GUI::BitmapBackend &S::GUI::BitmapCocoa::operator =(const BitmapBackend &newBitmap)
{
	SetSystemBitmap((Void *) ((BitmapCocoa &) newBitmap).bitmap);

	return *this;
}

S::Bool S::GUI::BitmapCocoa::operator ==(const int null) const
{
	if (bitmap == NIL)	return True;
	else			return False;
}

S::Bool S::GUI::BitmapCocoa::operator !=(const int null) const
{
	if (bitmap == NIL)	return False;
	else			return True;
}