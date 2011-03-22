
#include "contextmenu-impl.h"

#include "menu-impl.h"

NS_IMPL_ISUPPORTS1(CCtxMenu, ICtxMenu)

CCtxMenu::CCtxMenu(CW32ContextMenu *w32CtxMenu)
{
  mW32CtxMenu = w32CtxMenu;
  CMenu *menu = mW32CtxMenu->getMenu();
  if (menu)
    CallQueryInterface(menu, &mMenu);
  //OutputDebugString(L"CCtxMenu constructor");
}

CCtxMenu::~CCtxMenu()
{
  //OutputDebugString(L"CCtxMenu destructor");
  if (mW32CtxMenu)
    delete mW32CtxMenu;
  NS_IF_RELEASE(mMenu);
}

/* readonly attribute IMenu menu; */
NS_IMETHODIMP CCtxMenu::GetMenu(IMenu **aMenu)
{
  *aMenu = mMenu;
  NS_IF_ADDREF(*aMenu);
  return NS_OK;
}

/* boolean invokeCommand (in unsigned long idCmd); */
NS_IMETHODIMP CCtxMenu::InvokeCommand(PRUint32 idCmd, PRBool *_retval)
{
  *_retval = PR_FALSE;
  if (mW32CtxMenu) {
    if (mW32CtxMenu->invokeCommand(idCmd))
      *_retval = PR_TRUE;
  }
  return NS_OK;
}