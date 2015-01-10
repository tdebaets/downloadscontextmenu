#ifndef __CONTEXTMENU_IMPL_H__
#define __CONTEXTMENU_IMPL_H__

#include "contextmenu.h"
#include "w32contextmenu.h"

class CCtxMenu : public ICtxMenu
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_ICTXMENU

  CCtxMenu(CW32ContextMenu *w32CtxMenu);
  virtual ~CCtxMenu();

protected:
  CW32ContextMenu *mW32CtxMenu;
  IMenu *mMenu;
};

#endif