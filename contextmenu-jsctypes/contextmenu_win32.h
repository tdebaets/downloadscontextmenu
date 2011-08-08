#ifndef __CONTEXTMENU_WIN32_H__
#define __CONTEXTMENU_WIN32_H__

#include <vector>

#include <shlobj.h>

#include "contextmenu.h"
#include "contextmenu_base.h"

class CContextMenuWin32 : public CContextMenu
{
public:
    CContextMenuWin32();
    virtual ~CContextMenuWin32();
    virtual bool initialize(const wchar_t *filename);
    virtual bool invokeMenuItemCommand(const CMMenuItem* item);
private:
    void checkSeparator(std::vector<CMMenuItem> &items, unsigned int pos);
    CMMenu* getMenu();
    CMMenu* walkMenu(HMENU hmenu, int level);
    IContextMenu *mICtxMenu;
    IContextMenu2 *mICtxMenu2;
    IContextMenu3 *mICtxMenu3;
    HMENU mhMenu;
};

#endif