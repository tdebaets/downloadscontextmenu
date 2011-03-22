#ifndef __MENU_IMPL_H__
#define __MENU_IMPL_H__

#include <vector>

using namespace std;

#include "nsStringApi.h"
#include "contextmenu.h"

class CMenu;

class CMenuItem : public IMenuItem
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IMENUITEM

  CMenuItem();
  CMenuItem(wchar_t* caption, wchar_t accessKey, PRUint32 idCmd,
    PRBool isDefault, PRBool isDisabled);
  CMenuItem(wchar_t* caption, wchar_t accessKey, PRBool isDisabled,
    CMenu *subMenu);
  virtual ~CMenuItem();

  bool isSeparator();

protected:
  nsString mCaption;
  wchar_t mAccessKey;
  PRBool mIsSeparator;
  PRBool mIsDefault;
  PRBool mIsDisabled;
  IMenu *mSubMenu;
  PRUint32 mIdCmd;
};

class CMenu : public IMenu
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IMENU

  CMenu(vector<CMenuItem*> *items);
  virtual ~CMenu();

protected:
  vector<IMenuItem*> mItems;
};

#endif