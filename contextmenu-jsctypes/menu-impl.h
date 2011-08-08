#ifndef __MENU_IMPL_H__
#define __MENU_IMPL_H__

#include <string>
#include <vector>

class CMenu;

class CMenuItem
{
public:
    CMenuItem();
    CMenuItem(wchar_t* caption, wchar_t accessKey, unsigned int idCmd,
        bool isDefault, bool isDisabled);
    CMenuItem(wchar_t* caption, wchar_t accessKey, bool isDisabled,
        CMenu *subMenu);
    virtual ~CMenuItem();

    std::wstring mCaption;
    wchar_t mAccessKey;
    bool mIsSeparator;
    bool mIsDefault;
    bool mIsDisabled;
    CMenu *mSubMenu;
    unsigned int mIdCmd;
};

class CMenu
{
public:
    CMenu(std::vector<CMenuItem*> &items);
    virtual ~CMenu();

protected:
    std::vector<CMenuItem*> mItems;
};

#endif