 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/background.h>
#include <smooth/objectmanager.h>
#include <smooth/graphics/window.h>
#include <smooth/timer.h>
#include <smooth/threads/thread.h>

S::BackgroundApplication	*S::backgroundApplication = NIL;

S::BackgroundApplication::BackgroundApplication()
{
	backgroundTimer = new Timer();

	backgroundTimer->onInterval.Connect(&BackgroundApplication::TimerProc, this);
	backgroundTimer->Start(50);
}

S::BackgroundApplication::~BackgroundApplication()
{
	backgroundTimer->Stop();

	delete backgroundTimer;
}
S::Void S::BackgroundApplication::TimerProc()
{
	for (Int i = 0; i < mainObjectManager->GetNOfObjects(); i++)
	{
		Object	*object = mainObjectManager->GetNthObject(i);

		if (object != NIL)
		{
			if (!object->IsObjectInUse())
			{
				if (object->IsObjectDeleteable())
				{
					delete object;

					i = -1;

					continue;
				}
			}

			if (object->GetObjectType() == GUI::Window::classID)
			{
				if (((GUI::Window *) object)->IsInUse()) ((GUI::Window *) object)->Process(SM_MOUSEMOVE, 1, 0);
			}

			if (object->GetObjectType() == Threads::Thread::classID)
			{
				if (((Threads::Thread *) object)->GetStatus() == Threads::THREAD_RUNNING)
				{
					if (Setup::enableUnicode)	PostThreadMessageW(((Threads::Thread *) object)->GetThreadID(), SM_MOUSEMOVE, 1, 0);
					else				PostThreadMessageA(((Threads::Thread *) object)->GetThreadID(), SM_MOUSEMOVE, 1, 0);
				}
			}
		}
	}
}
