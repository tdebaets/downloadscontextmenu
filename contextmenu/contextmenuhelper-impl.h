#ifndef __CONTEXTMENUHELPER_IMPL_H__
#define __CONTEXTMENUHELPER_IMPL_H__

#include "contextmenu.h"

#define CONTEXTMENU_CONTRACTID "@bmproductions.fixnum.org/contextmenu;1"
#define CONTEXTMENU_CLASSNAME "ContextMenu"
// {85179E9E-99DF-4081-8AE7-930A95945B63}
#define CONTEXTMENU_CID { 0x85179e9e, 0x99df, 0x4081, { 0x8a, 0xe7, 0x93, 0xa, 0x95, 0x94, 0x5b, 0x63 } }

class CContextMenuHelper : public IContextMenuHelper
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_ICONTEXTMENUHELPER
};

#endif