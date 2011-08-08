
#include "menu-impl.h"

CMenuItem::CMenuItem()
{
    mIsSeparator = true;
    mCaption.assign(L"");
    mSubMenu = NULL;
}

CMenuItem::CMenuItem(wchar_t *caption, wchar_t accessKey, unsigned int idCmd,
                     bool isDefault, bool isDisabled)
{
    mIsSeparator = false;
    mCaption.assign(caption);
    mAccessKey = accessKey;
    mIdCmd = idCmd;
    mIsDefault = isDefault;
    mIsDisabled = isDisabled;
    mSubMenu = NULL;
}

CMenuItem::CMenuItem(wchar_t *caption, wchar_t accessKey, bool isDisabled,
                     CMenu *subMenu)
{
    mIsSeparator = false;
    mCaption.assign(caption);
    mAccessKey = accessKey;
    mIdCmd = 0;
    mIsDefault = false;
    mIsDisabled = false;
    mSubMenu = subMenu;
}

CMenuItem::~CMenuItem()
{
    if (mSubMenu)
        delete mSubMenu;
}

CMenu::CMenu(std::vector<CMenuItem*> &items)
{
    mItems = items;
}

CMenu::~CMenu()
{
    for (unsigned int i = 0; i < mItems.size(); i++)
        delete mItems[i];
}