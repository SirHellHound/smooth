 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#if (defined UNICODE && !defined _UNICODE) || (defined _UNICODE && !defined UNICODE)
#error Either none or both of UNICODE and _UNICODE must be defined to compile SMOOTH applications!
#endif

#ifndef _H_OBJSMOOTH__
#define _H_OBJSMOOTH__

#define __SMOOTH__

#include "iolib-cxx.h"

#include "smooth/application.h"
#include "smooth/definitions.h"
#include "smooth/i18n.h"
#include "smooth/object.h"
#include "smooth/container.h"
#include "smooth/array.h"
#include "smooth/titlebar.h"
#include "smooth/statusbar.h"
#include "smooth/client.h"
#include "smooth/divider.h"
#include "smooth/layer.h"
#include "smooth/button.h"
#include "smooth/checkbox.h"
#include "smooth/optionbox.h"
#include "smooth/groupbox.h"
#include "smooth/text.h"
#include "smooth/activearea.h"
#include "smooth/hyperlink.h"
#include "smooth/image.h"
#include "smooth/arrows.h"
#include "smooth/tabwidget.h"
#include "smooth/slider.h"
#include "smooth/editbox.h"
#include "smooth/scrollbar.h"
#include "smooth/tree.h"
#include "smooth/list.h"
#include "smooth/progressbar.h"
#include "smooth/listbox.h"
#include "smooth/combobox.h"
#include "smooth/treeview.h"
#include "smooth/objectproperties.h"
#include "smooth/metrics.h"
#include "smooth/loop.h"
#include "smooth/version.h"
#include "smooth/mdiclient.h"
#include "smooth/mdiwindow.h"
#include "smooth/toolwindow.h"
#include "smooth/timer.h"
#include "smooth/string.h"
#include "smooth/tooltip.h"
#include "smooth/point.h"
#include "smooth/size.h"
#include "smooth/rect.h"
#include "smooth/line.h"
#include "smooth/drag.h"
#include "smooth/datetime.h"
#include "smooth/binary.h"
#include "smooth/math.h"
#include "smooth/widget.h"
#include "smooth/system.h"
#include "smooth/setup.h"
#include "smooth/input.h"
#include "smooth/droparea.h"
#include "smooth/shortcut.h"
#include "smooth/extended.h"
#include "smooth/config.h"

#include "smooth/types/bool.h"
#include "smooth/types/float.h"
#include "smooth/types/int.h"
#include "smooth/types/void.h"

#include "smooth/threads/mutex.h"
#include "smooth/threads/semaphore.h"
#include "smooth/threads/thread.h"

#include "smooth/graphics/bitmap.h"
#include "smooth/graphics/font.h"
#include "smooth/graphics/surface.h"
#include "smooth/graphics/window.h"

#include "smooth/menu/menu.h"
#include "smooth/menu/menubar.h"
#include "smooth/menu/popupmenu.h"

#include "smooth/dialogs/colordlg.h"
#include "smooth/dialogs/dirdlg.h"
#include "smooth/dialogs/filedlg.h"
#include "smooth/dialogs/fontdlg.h"
#include "smooth/dialogs/messagebox.h"
#include "smooth/dialogs/splashscreen.h"
#include "smooth/dialogs/tipodaydlg.h"

#include "smooth/xml/attribute.h"
#include "smooth/xml/document.h"
#include "smooth/xml/node.h"

#endif
