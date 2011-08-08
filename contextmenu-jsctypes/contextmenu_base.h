#ifndef __CONTEXTMENU_IMPL_H__
#define __CONTEXTMENU_IMPL_H__

#include "contextmenu.h"

class CContextMenu
{
public:
    CContextMenu();
    virtual ~CContextMenu();
    virtual bool initialize(const wchar_t *filename) = 0;
    virtual bool invokeMenuItemCommand(const CMMenuItem* item) = 0;
    CMContextMenu* getContextMenu();

protected:
    void initMenuItem(CMMenuItem &item, const wchar_t* caption, wchar_t accessKey,
        unsigned int idCmd, bool isDefault, bool isDisabled);
    void initMenuItemSubMenu(CMMenuItem &item, const wchar_t* caption,
        wchar_t accessKey, bool isDisabled, CMMenu *subMenu);
    void initMenuItemSeparator(CMMenuItem &item);

    CMContextMenu *contextMenu;

private:
    void freeMenu(CMMenu* menu);
};

#endif