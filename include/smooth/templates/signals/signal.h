 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "../../object.h"

namespace smooth
{
	template <class returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_LIST> class SIGNALS_SIGNAL_CLASS_NAME;
};

#include "signal_base.h"

#include "../slots/slot_base.h"

#include "../slots/slot_class.h"
#include "../slots/slot_global.h"
#include "../slots/slot_signal.h"

namespace smooth
{
	template <class returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_LIST> class SIGNALS_SIGNAL_CLASS_NAME : public Signal
	{
		protected:
			Array<SIGNALS_SLOT_BASE_CLASS_NAME<Void SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *>	 slotsN;
			Array<SlotBase0<Void> *>									 slots0;
		public:
			SIGNALS_SIGNAL_CLASS_NAME()
			{
			}

			SIGNALS_SIGNAL_CLASS_NAME(const SIGNALS_SIGNAL_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> &oSignal)
			{
				for (int i = 0; i < oSignal.slotsN.GetNOfEntries(); i++)
				{
					slotsN.AddEntry(Array<SIGNALS_SLOT_BASE_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *>::GetNthEntry(&oSignal.slotsN, i)->Copy());
				}

				for (int i = 0; i < oSignal.slots0.GetNOfEntries(); i++)
				{
					slots0.AddEntry(Array<SlotBase0<returnTYPE> *>::GetNthEntry(&oSignal.slots0, i)->Copy());
				}

				parent = oSignal.parent;
			}

			virtual ~SIGNALS_SIGNAL_CLASS_NAME()
			{
				DisconnectAll();
			}

			template <class ct, class ot, class rrt> Int Connect(rrt (ct::*proc)(SIGNALS_ARGUMENT_TYPES), ot *inst)
			{
				slotsN.AddEntry(new SIGNALS_SLOT_CLASS_CLASS_NAME<ot, rrt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(proc, inst));

				return Success;
			}

			template <class rrt> Int Connect(rrt (*proc)(SIGNALS_ARGUMENT_TYPES))
			{
				slotsN.AddEntry(new SIGNALS_SLOT_GLOBAL_CLASS_NAME<rrt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(proc));

				return Success;
			}

			Int Connect(SIGNALS_SIGNAL_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES> *sig)
			{
				if (sig == this) return Error;

				slotsN.AddEntry(new SIGNALS_SLOT_SIGNAL_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(sig));
		
				return Success;
			}

#ifndef SIGNALS_SIGNAL_ZERO
			template <class ct, class ot, class rrt> Int Connect(rrt (ct::*proc)(), ot *inst)
			{
				slots0.AddEntry(new SlotClass0<ot, rrt>(proc, inst));

				return Success;
			}

			template <class rrt> Int Connect(rrt (*proc)())
			{
				slots0.AddEntry(new SlotGlobal0<rrt>(proc));

				return Success;
			}

			template <class rrt> Int Connect(Signal0<rrt> *sig)
			{
				if ((Signal *) sig == (Signal *) this) return Error;

				slots0.AddEntry(new SlotSignal0<rrt>(sig));

				return Success;
			}
#endif

			template <class ct, class ot, class rrt> Int Disconnect(rrt (ct::*proc)(SIGNALS_ARGUMENT_TYPES), ot *inst)
			{
				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)
				{
					if ((*slotsN.GetNthEntry(i)) == SIGNALS_SLOT_CLASS_CLASS_NAME<ot, rrt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(proc, inst))
					{
						delete slotsN.GetNthEntry(i);

						slotsN.RemoveEntry(slotsN.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}

			template <class rrt> Int Disconnect(rrt (*proc)(SIGNALS_ARGUMENT_TYPES))
			{
				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)
				{
					if ((*slotsN.GetNthEntry(i)) == SIGNALS_SLOT_GLOBAL_CLASS_NAME<rrt SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(proc))
					{
						delete slotsN.GetNthEntry(i);

						slotsN.RemoveEntry(slotsN.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}

			Int Disconnect(SIGNALS_SIGNAL_CLASS_NAME *sig)
			{
				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)
				{
					if ((*slotsN.GetNthEntry(i)) == SIGNALS_SLOT_SIGNAL_CLASS_NAME<returnTYPE SIGNALS_CONDITIONAL_COMMA SIGNALS_ARGUMENT_TYPES>(sig))
					{
						delete slotsN.GetNthEntry(i);

						slotsN.RemoveEntry(slotsN.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}

#ifndef SIGNALS_SIGNAL_ZERO
			template <class ct, class ot, class rrt> Int Disconnect(rrt (ct::*proc)(), ot *inst)
			{
				for (Int i = 0; i < slots0.GetNOfEntries(); i++)
				{
					if ((*slots0.GetNthEntry(i)) == SlotClass0<ot, rrt>(proc, inst))
					{
						delete slots0.GetNthEntry(i);

						slots0.RemoveEntry(slots0.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}

			template <class rrt> Int Disconnect(rrt (*proc)())
			{
				for (Int i = 0; i < slots0.GetNOfEntries(); i++)
				{
					if ((*slots0.GetNthEntry(i)) == SlotGlobal0<rrt>(proc))
					{
						delete slots0.GetNthEntry(i);

						slots0.RemoveEntry(slots0.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}

			template <class rrt> Int Disconnect(Signal0<rrt> *sig)
			{
				for (Int i = 0; i < slots0.GetNOfEntries(); i++)
				{
					if ((*slots0.GetNthEntry(i)) == SlotSignal0<rrt>(sig))
					{
						delete slots0.GetNthEntry(i);

						slots0.RemoveEntry(slots0.GetNthEntryIndex(i));

						break;
					}
				}

				return Success;
			}
#endif

			Int DisconnectAll()
			{
				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)	delete slotsN.GetNthEntry(i);
				for (Int j = 0; j < slots0.GetNOfEntries(); j++)	delete slots0.GetNthEntry(j);

				slotsN.RemoveAll();
				slots0.RemoveAll();

				return Success;
			}

			Void Emit(SIGNALS_ARGUMENT_PARAMETER_LIST)
			{
				ProtectParent();

				for (Int i = 0; i < slotsN.GetNOfEntries(); i++)	slotsN.GetNthEntry(i)->Call(SIGNALS_ARGUMENT_PARAMETERS);
				for (Int j = 0; j < slots0.GetNOfEntries(); j++)	slots0.GetNthEntry(j)->Call();

				UnprotectParent();
			}

			Int GetNOfConnectedSlots()
			{
				return slotsN.GetNOfEntries() + slots0.GetNOfEntries();
			}
	};
};
