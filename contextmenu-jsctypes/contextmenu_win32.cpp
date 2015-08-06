
#include "contextmenu_win32.h"

#include <locale>
#include <shlwapi.h>
#include <strsafe.h>

size_t StringLengthWithoutSpace(LPCTSTR lpString, size_t cchMax)
{
    size_t      len = 0;
    size_t      res = 0;
    std::locale locale; // the global locale

    if (!SUCCEEDED(StringCchLength(lpString, cchMax, &len)))
        return 0;

    for (unsigned int i = 0; i < len; i++)
    {
        // not using the C versions of isspace and iscntrl here because those don't
        // validate their input and can cause a read access violation on special
        // characters
        if (!std::isspace(lpString[i], locale) && !std::iscntrl(lpString[i], locale))
            res++;
    }

    return res;
}

bool pathToPidl(HWND hwnd, LPCWSTR pszPath, LPITEMIDLIST *ppidl)
{
    bool result = false;
    OSVERSIONINFO osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    if ( (osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion == 0) )
    {
        // no SHParseDisplayName on Win2k
        IShellFolder *shellFolder;
        if SUCCEEDED(SHGetDesktopFolder(&shellFolder))
        {
            result = SUCCEEDED(shellFolder->ParseDisplayName(hwnd, NULL,
                const_cast<LPWSTR>(pszPath), NULL, ppidl, NULL));
            shellFolder->Release();
        }
    }
    else
    {
        SFGAOF sfgao;
        result = SUCCEEDED(SHParseDisplayName(pszPath, NULL, ppidl, 0, &sfgao));
    }
    return result;
}

HRESULT getUIObjectOfFile(HWND hwnd, LPCWSTR pszPath, REFIID riid, void **ppv)
{
    *ppv = NULL;
    HRESULT hr;
    LPITEMIDLIST pidl;
    if (pathToPidl(hwnd, pszPath, &pidl))
    {
        IShellFolder *psf;
        LPCITEMIDLIST pidlChild;
        if (SUCCEEDED(hr = SHBindToParent(pidl, IID_IShellFolder,
            (void**)&psf, &pidlChild)))
        {
            hr = psf->GetUIObjectOf(hwnd, 1, &pidlChild, riid, NULL, ppv);
            psf->Release();
        }
        CoTaskMemFree(pidl);
    }
    return hr;
}

CContextMenuWin32::CContextMenuWin32()
{
    mICtxMenu = NULL;
    mICtxMenu2 = NULL;
    mICtxMenu3 = NULL;
    mhMenu = 0;
}

CContextMenuWin32::~CContextMenuWin32()
{
    //OutputDebugString(L"CW32ContextMenu destructor");
    if (mhMenu)
        DestroyMenu(mhMenu);
    if (mICtxMenu)
        mICtxMenu->Release();
}

bool CContextMenuWin32::initialize(const wchar_t *filename)
{
    IContextMenu *pcm;
    if (SUCCEEDED(getUIObjectOfFile(0, filename, IID_IContextMenu, (void**)&pcm)))
    {
        mICtxMenu = pcm;
        contextMenu->menu = getMenu();
    }
    return contextMenu->menu != NULL;
}

void CContextMenuWin32::checkSeparator(std::vector<CMMenuItem> &items,
                                       unsigned int pos)
{
    if (pos < items.size())
    {
        if (items.at(pos).isSeparator)
            items.erase(items.begin() + pos);
    }
}

#define CTXMENU_W32_MAX_RECURSION 10
#define CTXMENU_W32_MAX_TEXT_LENGTH 100

// TODO: support owner drawn items or build CopyTo-menu ourselves
CMMenu* CContextMenuWin32::walkMenu(HMENU hmenu, int level)
{
    if (level > CTXMENU_W32_MAX_RECURSION)
        return NULL;
    std::vector<CMMenuItem> items;
    TCHAR menuItemText[CTXMENU_W32_MAX_TEXT_LENGTH];
    MENUITEMINFO info;
    bool separatorAdded = false;
    CMMenu *subMenu;
    CMMenuItem item;
    for (int i = 0; i < GetMenuItemCount(hmenu); i++)
    {
        ZeroMemory(&info, sizeof(info));
        info.cbSize = sizeof(info);
        info.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU;
        info.dwTypeData = menuItemText;
        info.cch = CTXMENU_W32_MAX_TEXT_LENGTH;
        bool isDefault, isDisabled;
        TCHAR mnemonic;
        if (GetMenuItemInfo(hmenu, i, TRUE, &info))
        {
            if ((info.fType & MFT_SEPARATOR) == MFT_SEPARATOR)
            {
                if (!separatorAdded)
                {
                    initMenuItemSeparator(item);
                    items.push_back(item);
                }
                //OutputDebugString("-\n");
                separatorAdded = true;
            }
            else
            {
                //OutputDebugString(menuItemText);
                if ( ((info.fType & MFT_STRING) == MFT_STRING) &&
                    ((info.fType & MFT_OWNERDRAW) == 0))
                {
                    mnemonic = SHStripMneumonic(menuItemText);
                    // ignore items without any text
                    if (StringLengthWithoutSpace(menuItemText,
                            CTXMENU_W32_MAX_TEXT_LENGTH) == 0)
                        continue;
                    isDisabled = (info.fState & MFS_DISABLED) == MFS_DISABLED;
                    if (info.hSubMenu == 0)
                    {
                        isDefault = (info.fState & MFS_DEFAULT) == MFS_DEFAULT;
                        initMenuItem(item, menuItemText, mnemonic, info.wID,
                            isDefault, isDisabled);
                        items.push_back(item);
                        separatorAdded = false;
                    }
                    else
                    {
                        if (mICtxMenu3)
                        {
                            LRESULT lres;
                            mICtxMenu3->HandleMenuMsg2(WM_INITMENUPOPUP,
                                (WPARAM)info.hSubMenu, i, &lres);
                        }
                        else if (mICtxMenu2) {
                            mICtxMenu2->HandleMenuMsg(WM_INITMENUPOPUP,
                                (WPARAM)info.hSubMenu, i);
                        }
                        subMenu = walkMenu(info.hSubMenu, level + 1);
                        if (subMenu != NULL)
                        {
                            initMenuItemSubMenu(item, menuItemText, mnemonic,
                                isDisabled, subMenu);
                            items.push_back(item);
                            separatorAdded = false;
                        }
                    }
                    //OutputDebugString(menuItemText);
                    //OutputDebugStringW(pszText);

                }
                /*if ((info.fType & MFT_BITMAP) == MFT_BITMAP)
                text << "bitmap ";*/
                /*if (info.hSubMenu != 0)
                text << "submenu:" << info.hSubMenu << " ";*/
            }
        }
    }
    checkSeparator(items, 0);
    if (items.size() > 1)
        checkSeparator(items, items.size() - 1);
    if (items.empty())
        return NULL;
    else
    {
        CMMenu *menu = new CMMenu;
        menu->itemCount = items.size();
        menu->items = new CMMenuItem[menu->itemCount];
        for (unsigned int i = 0; i < items.size(); i++)
            menu->items[i] = items.at(i);
        return menu;
    }
}

// TODO: base this on the SleepMsg function at http://blogs.msdn.com/b/oldnewthing/archive/2006/01/26/517849.aspx
DWORD MsgSleep(DWORD Time)
{
    const DWORD TimerGranularity = 100;
    const DWORD MaxWmUserInterval = 150;
    if (Time < 0)
        return -1;
    DWORD StartTickCount = GetTickCount();
    DWORD EndTickCount = StartTickCount + Time;
    DWORD CurTickCount = StartTickCount;
    DWORD LastWmUserCount = StartTickCount;
    // only subtract tickcounts, to account for a rollback!
    while ((DWORD)(CurTickCount - StartTickCount) < Time)
    {
        MSG msg;
        if ((DWORD)(CurTickCount - LastWmUserCount) > MaxWmUserInterval)
            break;
        DWORD CurWaitTime = (DWORD)(EndTickCount - CurTickCount);
        if (CurWaitTime > TimerGranularity)
            CurWaitTime = TimerGranularity;
        MsgWaitForMultipleObjects(0, NULL, TRUE, CurWaitTime,
            QS_ALLINPUT | QS_ALLPOSTMESSAGE);
        CurTickCount = GetTickCount();
        while (0 != PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_USER)
                LastWmUserCount = CurTickCount;
            // important: dispatch *all* messages, weird bugs if we don't
            DispatchMessage(&msg);
        }
    }
    return CurTickCount - StartTickCount;
}

// never wait for more than 1 sec
#define MAX_WAIT_TIME 1000

void waitForAsyncItems()
{
    OSVERSIONINFO osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    // only on Win7 and higher
    if ( (osvi.dwMajorVersion >= 6) && (osvi.dwMinorVersion >= 1) )
        MsgSleep(MAX_WAIT_TIME);
}

#define ID_CMD_FIRST 1
#define ID_CMD_LAST  0x7FFF

CMMenu* CContextMenuWin32::getMenu()
{
    mhMenu = CreatePopupMenu();
    CMMenu *menu = NULL;
    if (mhMenu && mICtxMenu)
    {
        if (SUCCEEDED(mICtxMenu->QueryContextMenu(mhMenu, 0,
            ID_CMD_FIRST, ID_CMD_LAST,
            CMF_NORMAL | CMF_EXTENDEDVERBS)))
        {
            mICtxMenu->QueryInterface(IID_IContextMenu2, (void**)&mICtxMenu2);
            mICtxMenu->QueryInterface(IID_IContextMenu3, (void**)&mICtxMenu3);
            // Windows 7 builds the SendTo menu asynchronously, so we need to process
            // messages for a while
            waitForAsyncItems();
            menu = walkMenu(mhMenu, 1);
            if (mICtxMenu2) {
                mICtxMenu2->Release();
                mICtxMenu2 = NULL;
            }
            if (mICtxMenu3) {
                mICtxMenu3->Release();
                mICtxMenu3 = NULL;
            }
        }
        // Don't destroy the menu before a command has been invoked!!!
        // some handlers will fail, such as Vista's SendTo
        //DestroyMenu(hmenu);
    }
    return menu;
}

bool CContextMenuWin32::invokeMenuItemCommand(const CMMenuItem *item)
{
    if (item->idCmd > 0 && mICtxMenu != NULL) {
        CMINVOKECOMMANDINFOEX info = { 0 };
        info.cbSize = sizeof(info);
        info.fMask = CMIC_MASK_UNICODE;
        info.hwnd = 0;
        info.lpVerb  = MAKEINTRESOURCEA(item->idCmd - ID_CMD_FIRST);
        info.lpVerbW = MAKEINTRESOURCEW(item->idCmd - ID_CMD_FIRST);
        info.nShow = SW_SHOWNORMAL;
        return SUCCEEDED(mICtxMenu->InvokeCommand((LPCMINVOKECOMMANDINFO)&info));
    }
    return false;
}