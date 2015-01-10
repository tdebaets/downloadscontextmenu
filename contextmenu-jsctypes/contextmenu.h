#ifndef __CONTEXTMENU_H__
#define __CONTEXTMENU_H__

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

typedef struct CMMenu_s CMMenu;

typedef struct CMMenuItem_s {
    wchar_t         *caption;
    wchar_t         accessKey;
    bool            isSeparator;
    bool            isDefault;
    bool            isDisabled;
    CMMenu          *subMenu;
    unsigned int    idCmd;
} CMMenuItem;

typedef struct CMMenu_s {
    unsigned int    itemCount;
    CMMenuItem      *items;
} CMMenu;

typedef struct CMContextMenu_s {
    void            *__handle; /* internal use only */
    CMMenu          *menu;
} CMContextMenu;

CMContextMenu*  CMGetContextMenuForFile (const wchar_t *filename);
bool            CMContextMenuCommand    (const CMContextMenu *menu,
                                         const CMMenuItem* item);
void            CMFreeContextMenu       (const CMContextMenu *menu);

#endif
