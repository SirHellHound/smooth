 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OBJSMOOTH_TREE_
#define _H_OBJSMOOTH_TREE_

namespace smooth
{
	class Tree;
};

#include "object.h"
#include "rect.h"

namespace smooth
{
	const Int ST_ENTRY	= 0;
	const Int ST_SUB	= 1;

	class SMOOTHAPI Tree
	{
		public: class SMOOTHAPI Entry
		{
			public:
				Int		 type;
				Int		 code;
				Bool		 last;

				String		 text;
				Tree		*sub;

				Bool		 chk;
				Bool		 clk;
				Bool		 open;
				Int		 size;
				Bool		 sizeset;
				Rect		 rect;

						 Entry(Int, Int);
						~Entry();
			signals:
				Signal0<>	 onClick;
		};

		protected:
			Size		 subsize;
			Bool		 sizeset;
			Bool		 entrysizesset;
			Int		 nOfEntries;
			Void		 GetSize();
			Int		 GetSizeY();
			Int		 GetSizeX();
			Void		 GetTreeEntriesSize();
		public:
			Array<Entry *>	 entries;

					 Tree();
			virtual		~Tree();

			Entry		*AddEntry(String, Tree * = NIL);
			Int		 RemoveEntry(Entry *);
			Int		 RemoveEntry(Int);

			Int		 GetNOfEntries();
	};
};

#endif
