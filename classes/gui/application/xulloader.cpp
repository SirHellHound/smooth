 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/application/xulloader.h>
#include <smooth/xml/xul/renderer.h>
#include <smooth/loop.h>

int WINAPI S::GUI::LoadXUL(HWND shWnd, HINSTANCE shInstance, LPSTR sszCmdLine, int siCmdShow)
{
	hInstance	= shInstance;
	szCmdLine	= String(sszCmdLine);
	iCmdShow	= siCmdShow;

	Init();

	GUI::Application::GetStartupDirectory();
	GUI::Application::GetApplicationDirectory();

	XULLoader	*loader = new XULLoader(szCmdLine);

	Loop();

	Object::DeleteObject(loader);

	Free();

	return 0;
}

S::GUI::XULLoader::XULLoader(String xulFile)
{
	xulRenderer = new XML::XUL::Renderer();

	if (xulRenderer->LoadXUL(xulFile) == Success) RegisterObject(xulRenderer->GetWidget());
}

S::GUI::XULLoader::~XULLoader()
{
	delete xulRenderer;
}