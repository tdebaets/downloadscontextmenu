
#include <shlwapi.h>
#include <strsafe.h>

#include "w32contextmenu.h"

size_t StringLengthWithoutSpace(LPCTSTR lpString, size_t cchMax)
{
    size_t len;
    if (!SUCCEEDED(StringCchLength(lpString, cchMax, &len)))
        return 0;
    size_t res = 0;
    for (unsigned int i = 0; i <= len; i++)
    {
        if (!isspace(lpString[i]) && !iscntrl(lpString[i]))
            res++;
    }
    return res;
}

bool pathToPidl(HWND hwnd, LPWSTR pszPath, LPITEMIDLIST *ppidl)
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
            result = SUCCEEDED(shellFolder->ParseDisplayName(hwnd, NULL, pszPath,
                NULL, ppidl, NULL));
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

HRESULT getUIObjectOfFile(HWND hwnd, LPWSTR pszPath, REFIID riid, void **ppv)
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

CW32ContextMenu::CW32ContextMenu()
{
    mICtxMenu = NULL;
    mICtxMenu2 = NULL;
    mICtxMenu3 = NULL;
    mhMenu = 0;
}

CW32ContextMenu::CW32ContextMenu(const wchar_t *filename)
{
    //OutputDebugString(L"CW32ContextMenu constructor");
    mICtxMenu = NULL;
    mICtxMenu2 = NULL;
    mICtxMenu3 = NULL;
    mhMenu = 0;
    IContextMenu *pcm;
    int len = wcslen(filename);
    wchar_t *fname = new wchar_t[len + 1];
    wcscpy_s(fname, len + 1, filename);
    if (SUCCEEDED(getUIObjectOfFile(0, fname,
        IID_IContextMenu, (void**)&pcm)))
        mICtxMenu = pcm;
    delete [] fname;
}

CW32ContextMenu::~CW32ContextMenu()
{
    //OutputDebugString(L"CW32ContextMenu destructor");
    if (mhMenu)
        DestroyMenu(mhMenu);
    if (mICtxMenu)
        mICtxMenu->Release();
}

void CW32ContextMenu::checkSeparator(vector<CMenuItem*> *items, unsigned int pos)
{
    if (pos < items->size())
    {
        CMenuItem *item = items->at(pos);
        if (item->isSeparator())
        {
            items->erase(items->begin() + pos);
            delete item;
        }
    }
}

#define W32CTXMENU_MAX_RECURSION 10

// TODO: support owner drawn items or build CopyTo-menu ourselves
CMenu* CW32ContextMenu::walkMenu(HMENU hmenu, int level) {
    if (level > W32CTXMENU_MAX_RECURSION)
        return NULL;
    vector<CMenuItem*> items;
    const int MAX_TEXT_LENGTH = 100;
    TCHAR menuItemText[MAX_TEXT_LENGTH];
    MENUITEMINFO info;
    bool separatorAdded = false;
    CMenu *subMenu;
    for (int i = 0; i < GetMenuItemCount(hmenu); i++)
    {
        ZeroMemory(&info, sizeof(info));
        info.cbSize = sizeof(info);
        info.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU;
        info.dwTypeData = menuItemText;
        info.cch = MAX_TEXT_LENGTH;
        bool isDefault, isDisabled;
        TCHAR mnemonic;
        if (GetMenuItemInfo(hmenu, i, TRUE, &info)) {
            if ((info.fType & MFT_SEPARATOR) == MFT_SEPARATOR)
            {
                if (!separatorAdded)
                    items.push_back(new CMenuItem());
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
                    if (StringLengthWithoutSpace(menuItemText, MAX_TEXT_LENGTH) == 0)
                        continue;
                    isDisabled = (info.fState & MFS_DISABLED) == MFS_DISABLED;
                    if (info.hSubMenu == 0)
                    {
                        isDefault = (info.fState & MFS_DEFAULT) == MFS_DEFAULT;
                        items.push_back(new CMenuItem(menuItemText, mnemonic, info.wID,
                            isDefault, isDisabled));
                        separatorAdded = false;
                    }
                    else
                    {
                        if (mICtxMenu3)
                        {
                            LRESULT lres;
                            mICtxMenu3->HandleMenuMsg2(WM_INITMENUPOPUP, (WPARAM)info.hSubMenu, i, &lres);
                        }
                        else if (mICtxMenu2) {
                            mICtxMenu2->HandleMenuMsg(WM_INITMENUPOPUP, (WPARAM)info.hSubMenu, i);
                        }
                        subMenu = walkMenu(info.hSubMenu, level + 1);
                        if (subMenu != NULL)
                        {
                            items.push_back(new CMenuItem(menuItemText, mnemonic,
                                isDisabled, subMenu));
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
    checkSeparator(&items, 0);
    if (items.size() > 1)
        checkSeparator(&items, items.size() - 1);
    if (items.empty())
        return NULL;
    else
    {
        return new CMenu(&items);
    }
}

CMenu* CW32ContextMenu::convertToCMenu()
{
    return walkMenu(mhMenu, 1);
}

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

// never wait for than 1 sec
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

CMenu* CW32ContextMenu::getMenu()
{
    mhMenu = CreatePopupMenu();
    CMenu *menu = NULL;
    if (mhMenu && mICtxMenu) {
        if (SUCCEEDED(mICtxMenu->QueryContextMenu(mhMenu, 0,
            ID_CMD_FIRST, ID_CMD_LAST,
            CMF_NORMAL | CMF_EXTENDEDVERBS))) {
                mICtxMenu->QueryInterface(IID_IContextMenu2, (void**)&mICtxMenu2);
                mICtxMenu->QueryInterface(IID_IContextMenu3, (void**)&mICtxMenu3);
                // Windows 7 builds the SendTo menu asynchronously, so we need to process
                // messages for a while
                waitForAsyncItems();
                menu = convertToCMenu();
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

bool CW32ContextMenu::invokeCommand(int idCmd)
{
    if (idCmd > 0 && mICtxMenu != NULL) {
        CMINVOKECOMMANDINFOEX info = { 0 };
        info.cbSize = sizeof(info);
        info.fMask = CMIC_MASK_UNICODE;
        info.hwnd = 0;
        info.lpVerb  = MAKEINTRESOURCEA(idCmd - ID_CMD_FIRST);
        info.lpVerbW = MAKEINTRESOURCEW(idCmd - ID_CMD_FIRST);
        info.nShow = SW_SHOWNORMAL;
        return SUCCEEDED(mICtxMenu->InvokeCommand((LPCMINVOKECOMMANDINFO)&info));
    }
    return false;
}