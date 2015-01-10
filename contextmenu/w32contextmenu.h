#ifndef __W32CONTEXTMENU_H__
#define __W32CONTEXTMENU_H__

#include <vector>

using namespace std;

#include <shlobj.h>

#include "menu-impl.h"

class CW32ContextMenu
{
public:
  CW32ContextMenu();
  CW32ContextMenu(const wchar_t *filename);
  ~CW32ContextMenu();
  CMenu* getMenu();
  bool invokeCommand(int idCmd);
private:
  void checkSeparator(vector<CMenuItem*> *items, unsigned int pos);
  CMenu* convertToCMenu();
  CMenu* walkMenu(HMENU hmenu, int level);
  IContextMenu *mICtxMenu;
  IContextMenu2 *mICtxMenu2;
  IContextMenu3 *mICtxMenu3;
  HMENU mhMenu;
};

#endif