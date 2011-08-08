
#include "contextmenu.h"
#include "contextmenu_base.h"
#include "contextmenu_win32.h"

CMContextMenu* CMGetContextMenuForFile(const wchar_t *filename)
{
    //OutputDebugStringW(filename);
    CContextMenu* contextMenu = new CContextMenuWin32();
    if (contextMenu->initialize(filename))
        return contextMenu->getContextMenu();
    else
    {
        delete contextMenu;
        return NULL;
    }
}

bool CMContextMenuCommand(const CMContextMenu *menu, const CMMenuItem *item)
{
    if (!menu || !menu->__handle || !item)
        return false;
    CContextMenu* contextMenu = static_cast<CContextMenu*>(menu->__handle);
    return contextMenu->invokeMenuItemCommand(item);
}

void CMFreeContextMenu(const CMContextMenu *menu)
{
    if (!menu || !menu->__handle)
        return;
    CContextMenu* contextMenu = static_cast<CContextMenu*>(menu->__handle);
    delete contextMenu;
}
