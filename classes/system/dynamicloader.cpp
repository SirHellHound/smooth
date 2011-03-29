 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/dynamicloader.h>
#include <smooth/files/directory.h>
#include <smooth/gui/application/application.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <dlfcn.h>
#endif

const S::Short	 S::System::DynamicLoader::classID = S::Object::RequestClassID();

S::System::DynamicLoader::DynamicLoader(const String &module)
{
#if defined __WIN32__
	static String	 dllExt		= ".dll";
	static String	 versionPattern	= dllExt;
#elif defined __APPLE__
	static String	 dllExt		= ".dylib";
	static String	 versionPattern	= String(".*").Append(dllExt);
#else
	static String	 dllExt		= ".so";
	static String	 versionPattern	= String(dllExt).Append(".*");
#endif

#ifdef __WIN32__
	/* Try the supplied module name in application directory.
	 */
	if (Setup::enableUnicode)	handle = LoadLibraryW(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(dllExt) ? String() : dllExt));
	else				handle = LoadLibraryA(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(dllExt) ? String() : dllExt));

	if (handle == NIL)
	{
		/* Try the supplied module name system wide.
		 */
		if (Setup::enableUnicode)	handle = LoadLibraryW(String(module).Append(module.EndsWith(dllExt) ? String() : dllExt));
		else				handle = LoadLibraryA(String(module).Append(module.EndsWith(dllExt) ? String() : dllExt));
	}
#else
	/* Try the supplied module name in application directory.
	 */
	handle = dlopen(GUI::Application::GetApplicationDirectory().Append(module).Append(module.EndsWith(dllExt) || module.Find(String(dllExt).Append(".")) >= 0 ? String() : dllExt), RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE);

	if (handle == NIL)
	{
		/* Try the supplied module name system wide.
		 */
		handle = dlopen(String(module).Append(module.EndsWith(dllExt) || module.Find(String(dllExt).Append(".")) >= 0 ? String() : dllExt), RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE);
	}

	if (handle == NIL)
	{
#ifdef __APPLE__
		const char	*directories[] = { "/usr/lib", "/opt/local/lib", NIL };
#else
		const char	*directories[] = { "/usr/lib", "/usr/local/lib", NIL };
#endif

		/* Try loading an unversioned library.
		 */
		for (Int i = 0; directories[i] != NIL; i++)
		{
			Directory		 directory(directories[i]);
			const Array<File>	&files = directory.GetFilesByPattern(String(module.StartsWith("lib") || module.Find("/") >= 0 ? String() : "lib").Append(module).Append(module.EndsWith(dllExt) || module.Find(String(dllExt).Append(".")) >= 0 ? String() : dllExt));

			if (files.Length() > 0)
			{
				handle = dlopen((String) files.GetFirst(), RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE);
			}

			if (handle != NIL) return;
		}

		/* Try loading a versioned library.
		 */
		for (Int i = 0; directories[i] != NIL; i++)
		{
			Directory		 directory(directories[i]);
			const Array<File>	&files = directory.GetFilesByPattern(String(module.StartsWith("lib") || module.Find("/") >= 0 ? String() : "lib").Append(module).Append(module.EndsWith(dllExt) || module.Find(String(dllExt).Append(".")) >= 0 ? String() : versionPattern));

			if (files.Length() > 0)
			{
				handle = dlopen((String) files.GetFirst(), RTLD_NOW | RTLD_LOCAL | RTLD_NODELETE);
			}

			if (handle != NIL) return;
		}
	}
#endif
}

S::System::DynamicLoader::~DynamicLoader()
{
#ifdef __WIN32__
	if (handle != NIL) FreeLibrary((HINSTANCE) handle);
#else
	if (handle != NIL) dlclose(handle);
#endif
}

S::Void *S::System::DynamicLoader::GetFunctionAddress(const String &functionName) const
{
	if (handle == NIL) return NIL;

#ifdef __WIN32__
	return (Void *) GetProcAddress((HINSTANCE) handle, functionName);
#else
	return (Void *) dlsym(handle, functionName);
#endif
}

S::Void *S::System::DynamicLoader::GetSystemModuleHandle() const
{
	return handle;
}
