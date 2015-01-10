
#include "contextmenuhelper-impl.h"

#include "contextmenu-impl.h"
#include "w32contextmenu.h"

NS_IMPL_ISUPPORTS1(CContextMenuHelper, IContextMenuHelper)

/* ICtxMenu getContextMenuForFile (in wstring filename); */
NS_IMETHODIMP CContextMenuHelper::GetContextMenuForFile(const PRUnichar *filename, ICtxMenu **_retval)
{
  *_retval = NULL;
  CW32ContextMenu *w32CtxMenu = new CW32ContextMenu(filename);
  ICtxMenu *ctxMenu;
  CallQueryInterface(new CCtxMenu(w32CtxMenu), &ctxMenu);
  *_retval = ctxMenu;
  return NS_OK;
}