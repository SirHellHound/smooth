 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#import <Cocoa/Cocoa.h>

#include <smooth/gui/dialogs/file/filedlg_cocoa.h>
#include <smooth/files/file.h>
#include <smooth/misc/number.h>
#include <smooth/foreach.h>

const Error &S::GUI::Dialogs::FileSelection::ShowDialog()
{
	/* Create file chooser dialog.
	 */
	if (mode == SFM_OPEN)
	{
		NSOpenPanel	*openPanel = [NSOpenPanel openPanel];

		if (flags & SFD_ALLOWMULTISELECT) [openPanel setAllowsMultipleSelection: true];

		SetFilters(openPanel);

		[openPanel runModal];

		NSArray *URLs = [openPanel URLs];

		for (unsigned int i = 0; i < [URLs count]; i++)
		{
			String	 file;

			file.ImportFrom("UTF-8", [[[URLs objectAtIndex: i] path] UTF8String]);
			files.Add(file);
		}

		[openPanel release];
	}
	else if (mode == SFM_SAVE)
	{
		NSSavePanel	*savePanel = [NSSavePanel savePanel];

		SetFilters(savePanel);

		[savePanel runModal];

		String	 file;

		file.ImportFrom("UTF-8", [[[savePanel URL] path] UTF8String]);
		files.Add(file);

		[savePanel release];
	}

	if (files.Length() == 0) error = Error();

	return error;
}

S::Bool S::GUI::Dialogs::FileSelection::SetFilters(void *iSavePanel)
{
	NSSavePanel *savePanel = (NSSavePanel *) iSavePanel;

	/* Add file filters.
	 */
	NSMutableArray	*fileTypes = [NSMutableArray arrayWithCapacity: filters.Length()];

	for (int i = 0; i < filters.Length(); i++)
	{
		const Array<String>	&patterns = filters.GetNth(i).Explode(";");

		foreach (String pattern, patterns)
		{
			if (pattern.FindLast(".") >= 0) pattern = pattern.Tail(pattern.Length() - pattern.FindLast(".") - 1);

			if (pattern.Trim() != NIL) [fileTypes addObject: [NSString stringWithUTF8String: pattern.Trim()]];
		}
	}

	[savePanel setAllowedFileTypes: fileTypes];

	return True;
}
