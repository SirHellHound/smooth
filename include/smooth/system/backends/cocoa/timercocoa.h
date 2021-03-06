 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_OBJSMOOTH_TIMERCOCOA
#define H_OBJSMOOTH_TIMERCOCOA

#include <Cocoa/Cocoa.h>

namespace smooth
{
	namespace System
	{
		class TimerCocoa;
	};
};

#include "../timerbackend.h"

namespace smooth
{
	namespace System
	{
		const Short	 TIMER_COCOA = 3;

		class TimerCocoa : public TimerBackend
		{
			private:
				NSTimer	*timer;

				void	*receiver;
			public:
					 TimerCocoa(Timer *);
					~TimerCocoa();

				Int	 Start(Int);
				Int	 Stop();

				Int	 GetID() const;
		};
	};
};

#endif
