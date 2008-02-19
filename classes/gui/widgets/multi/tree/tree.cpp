 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/tree/tree.h>
#include <smooth/gui/widgets/multi/list/listbox.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>

const S::Int	 S::GUI::Tree::classID = S::Object::RequestClassID();

S::GUI::Tree::Tree(const String &iText) : ListEntry(iText)
{
	type	= classID;

	list.onSelectEntry.Connect(&onSelectEntry);

	Widget::Add(&list);
}

S::GUI::Tree::~Tree()
{
}

S::Int S::GUI::Tree::Add(Widget *widget)
{
	if (widget->GetObjectType() == ListEntry::classID) return list.Add(widget);

	return Widget::Add(widget);
}

S::Int S::GUI::Tree::Remove(Widget *widget)
{
	if (widget->GetObjectType() == ListEntry::classID) return list.Remove(widget);

	return Widget::Remove(widget);
}

S::Int S::GUI::Tree::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Int	 height	= 15;

	if (IsMarked())
	{
		for (Int i = 0; i < Length(); i++)
		{
			height += GetNthEntry(i)->GetHeight();
		}
	}

	SetHeight(height);

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
		case SP_MOUSEIN:
		case SP_MOUSEOUT:
			{
				Surface	*surface = container->GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetSize());
				Font	 nFont	 = font;
				Bool	 gotTabs = False;

				if (mouseOver)		nFont.SetColor(Setup::GradientTextColor);
				if (!active)		nFont.SetColor(Setup::GrayTextColor);

				for (Int r = 0; r < text.Length(); r++) if (text[r] == '\t') { gotTabs = True; break; }

				surface->StartPaint(frame);

				if (mouseOver)		surface->Gradient(Rect(Point(frame.left, frame.top), Size(GetWidth(), 15)), Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
				else			surface->Box(Rect(Point(frame.left, frame.top), Size(GetWidth(), 15)), Setup::ClientColor, Rect::Filled);

				Rect	 cbRect = Rect(GetRealPosition() + Point(2, 3), Size(9, 9));

				if (cbRect.top <= cbRect.bottom - 1)
				{
					surface->Box(cbRect, Setup::ClientColor, Rect::Filled);
					surface->Box(cbRect, Setup::GrayTextColor, Rect::Outlined);

					if (cbRect.top <= cbRect.bottom - 3)
					{
						Point	 p1 = Point(cbRect.left + 2 + (IsRightToLeft() ? 1 : 0), cbRect.top + 4);
						Point	 p2 = Point(cbRect.right - 2 + (IsRightToLeft() ? 1 : 0), cbRect.top + 4);

						Color	 darkColor = Setup::ClientTextColor;

						if (!active) darkColor = Setup::GrayTextColor;

						surface->Line(p1, p2, darkColor);

						if (!IsMarked())
						{
							p1 = Point(cbRect.left + 4 + (IsRightToLeft() ? 1 : 0), cbRect.top + 2);
							p2 = Point(cbRect.left + 4 + (IsRightToLeft() ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, darkColor);
						}
					}
				}

				if (container->GetObjectType() == ListBox::classID && ((ListBox *) container)->GetNOfTabs() > 0 && gotTabs)
				{
					for (Int i = 0; i < ((ListBox *) container)->GetNOfTabs(); i++)
					{
						Rect	 rect = Rect(GetRealPosition() + Point(1, 1), GetSize() - Size(3, 2));

						rect.left += ((ListBox *) container)->GetNthTabOffset(i);
						rect.left += (i == 0 ? 12 : 0);

						if (((ListBox *) container)->GetNOfTabs() >= i + 2) rect.right = rect.left + (((ListBox *) container)->GetNthTabOffset(i + 1) - ((ListBox *) container)->GetNthTabOffset(i)) - (i == 0 ? 12 : 0) - 3;

						String	 tabText = GetNthTabText(i);

						if (((ListBox *) container)->GetNthTabOrientation(i) == OR_RIGHT)
						{
							rect.left = Math::Max(rect.left, rect.right - nFont.GetTextSizeX(tabText));
						}

						surface->SetText(tabText, rect, nFont);
					}
				}
				else
				{
					surface->SetText(text, frame + Point(1 + 12, 1) - Size(2 + 12, 2), nFont);
				}

				if (IsMarked())
				{
					frame.top = 15;
					frame.left = 12;

					list.SetMetrics(Point(0, 0), GetSize());
					list.Show();

					for (Int i = 0; i < Length(); i++)
					{
						ListEntry	*operat = GetNthEntry(i);

						operat->SetMetrics(Point(frame.left, frame.top), Size(GetWidth() - frame.left, operat->GetHeight()));
						operat->Show();
						operat->Paint(SP_PAINT);

						frame.top += operat->GetHeight();
					}
				}

				surface->EndPaint();
			}

			break;
	}

	return Success();
}

S::Bool S::GUI::Tree::IsTypeCompatible(Int compType) const
{
	if (compType == Object::classID || compType == Widget::classID || compType == ListEntry::classID)	return True;
	else													return False;
}