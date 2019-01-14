 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/cocoa/threadcocoa.h>

S::Threads::ThreadBackend *CreateThreadCocoa(S::Void *iThread)
{
	return new S::Threads::ThreadCocoa(iThread);
}

S::Int	 threadCocoaTmp = S::Threads::ThreadBackend::SetBackend(&CreateThreadCocoa);

S::Threads::ThreadCocoa::ThreadCocoa(Void *iThread)
{
	type		 = THREAD_COCOA;

	thread		 = NIL;
	myThread	 = False;

	info.threadProc	 = NIL;
	info.threadParam = NIL;

	if (iThread != NIL) thread = (pthread_t *) iThread;
}

S::Threads::ThreadCocoa::~ThreadCocoa()
{
	if (myThread) Stop();
}

S::Int S::Threads::ThreadCocoa::Start(Void (*threadProc)(Void *), Void *threadParam)
{
	Stop();

	info.threadProc	 = threadProc;
	info.threadParam = threadParam;

	thread	 = new pthread_t;
	myThread = True;

	pthread_create(thread, NULL, Caller, &info);

	return Success();
}

S::Int S::Threads::ThreadCocoa::Stop()
{
	if (thread == NIL) return Error();

	pthread_t	*thread	  = this->thread;
	Bool		 myThread = this->myThread;

	this->thread = NIL;

	pthread_cancel(*thread);
	pthread_detach(*thread);

	if (myThread) delete thread;

	return Success();
}

S::Int S::Threads::ThreadCocoa::Wait()
{
	if (thread == NIL) return Error();

	pthread_t	*thread	  = this->thread;
	Bool		 myThread = this->myThread;

	this->thread = NIL;

	pthread_join(*thread, NIL);

	if (myThread) delete thread;

	return Success();
}

S::Void S::Threads::ThreadCocoa::Exit()
{
	if (!this->thread || !pthread_equal(pthread_self(), *this->thread)) return;

	pthread_t	*thread	  = this->thread;
	Bool		 myThread = this->myThread;

	this->thread = NIL;

	pthread_detach(*thread);

	if (myThread) delete thread;
}

void *S::Threads::ThreadCocoa::Caller(void *param)
{
	ThreadInfo		*info = (ThreadInfo *) param;
	NSAutoreleasePool	*pool = [[NSAutoreleasePool alloc] init];

	info->threadProc(info->threadParam);

	[pool release];

	return NULL;
}
