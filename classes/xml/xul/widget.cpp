 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/xml/xul/renderer.h>
#include <smooth/xml/node.h>
#include <smooth/gui/widgets/widget.h>

const S::Short	 S::XML::XUL::Widget::HORIZONTAL = 0;
const S::Short	 S::XML::XUL::Widget::VERTICAL	 = 1;

S::XML::XUL::Widget::Widget(Node *node)
{
	defaultWidth  = 100;
	defaultHeight = 20;

	if (node != NIL)
	{
		id	    = GetXMLAttributeValue(node, "id");

		orient	    = (GetXMLAttributeValue(node, "orient") == "vertical") ? VERTICAL : HORIZONTAL;

		left	    = GetXMLAttributeValue(node, "left").ToInt();
		top	    = GetXMLAttributeValue(node, "top").ToInt();

		width	    = GetXMLAttributeValue(node, "width").ToInt();
		height	    = GetXMLAttributeValue(node, "height").ToInt();

		minwidth    = GetXMLAttributeValue(node, "minwidth").ToInt();
		minheight   = GetXMLAttributeValue(node, "minheight").ToInt();

		maxwidth    = GetXMLAttributeValue(node, "maxwidth").ToInt();
		maxheight   = GetXMLAttributeValue(node, "maxheight").ToInt();

		flex	    = GetXMLAttributeValue(node, "flex").ToInt();

		statustext  = GetXMLAttributeValue(node, "statustext");
		tooltiptext = GetXMLAttributeValue(node, "tooltiptext");
	}
	else
	{
		orient	    = HORIZONTAL;

		left	    = 0;
		top	    = 0;

		width	    = 0;
		height	    = 0;

		minwidth    = 0;
		minheight   = 0;

		maxwidth    = 0;
		maxheight   = 0;

		flex	    = 0;
	}
}

S::XML::XUL::Widget::~Widget()
{
}

S::GUI::Widget *S::XML::XUL::Widget::GetWidget() const
{
	return NIL;
}

S::Int S::XML::XUL::Widget::SetMetrics(const GUI::Point &pos, const GUI::Size &size)
{
	return GetWidget()->SetMetrics(pos, size);
}

S::String S::XML::XUL::Widget::GetXMLAttributeValue(Node *node, const String &attribute) const
{
	if (node != NIL)
	{
		if (node->GetAttributeByName(attribute) != NIL)
		{
			return node->GetAttributeByName(attribute)->GetContent();
		}
	}

	return NIL;
}
