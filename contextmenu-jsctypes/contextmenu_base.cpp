
#include "contextmenu_base.h"

CContextMenu::CContextMenu()
{
    contextMenu = new CMContextMenu;
    memset(contextMenu, 0, sizeof(CMContextMenu));
    contextMenu->__handle = this;
}

CContextMenu::~CContextMenu()
{
    //OutputDebugStringA("destructor");
    if (contextMenu->menu)
        freeMenu(contextMenu->menu);
    delete contextMenu;
}

CMContextMenu* CContextMenu::getContextMenu()
{
    return contextMenu;
}

void CContextMenu::freeMenu(CMMenu *menu)
{
    for (unsigned int i = 0; i < menu->itemCount; i++)
    {
        if (menu->items[i].subMenu)
            freeMenu(menu->items[i].subMenu);
        if (menu->items[i].caption)
            delete[] menu->items[i].caption;
    }
    delete[] menu->items;
    delete menu;
}

wchar_t* copyWideString(const wchar_t *str)
{
    unsigned int len = wcslen(str);
    wchar_t *result = new wchar_t[len + 1];
    wcscpy_s(result, len + 1, str);
    return result;
}

void CContextMenu::initMenuItem(CMMenuItem &item, const wchar_t *caption,
                                  wchar_t accessKey, unsigned int idCmd,
                                  bool isDefault, bool isDisabled)
{
    memset(&item, 0, sizeof(CMMenuItem));
    item.isSeparator = false;
    item.caption = copyWideString(caption);
    item.accessKey = accessKey;
    item.idCmd = idCmd;
    item.isDefault = isDefault;
    item.isDisabled = isDisabled;
}

void CContextMenu::initMenuItemSubMenu(CMMenuItem &item, const wchar_t *caption,
                                       wchar_t accessKey, bool isDisabled,
                                       CMMenu *subMenu)
{
    memset(&item, 0, sizeof(CMMenuItem));
    item.isSeparator = false;
    item.caption = copyWideString(caption);
    item.accessKey = accessKey;
    item.idCmd = 0;
    item.isDefault = false;
    item.isDisabled = isDisabled;
    item.subMenu = subMenu;
}

void CContextMenu::initMenuItemSeparator(CMMenuItem &item)
{
    memset(&item, 0, sizeof(CMMenuItem));
    item.isSeparator = true;
}