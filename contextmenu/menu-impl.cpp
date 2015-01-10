
#include "menu-impl.h"

NS_IMPL_ISUPPORTS1(CMenuItem, IMenuItem)
NS_IMPL_ISUPPORTS1(CMenu, IMenu)

CMenuItem::CMenuItem()
{
  //OutputDebugString(L"CMenuItem constructor");
  mIsSeparator = PR_TRUE;
  mCaption.Assign(L"");
  mSubMenu = NULL;
}

CMenuItem::CMenuItem(wchar_t* caption, wchar_t accessKey, PRUint32 idCmd,
                     PRBool isDefault, PRBool isDisabled)
{
  //OutputDebugString(L"CMenuItem constructor");
  mIsSeparator = PR_FALSE;
  mCaption.Assign(caption);
  mAccessKey = accessKey;
  mIdCmd = idCmd;
  mIsDefault = isDefault;
  mIsDisabled = isDisabled;
  mSubMenu = NULL;
}

CMenuItem::CMenuItem(wchar_t* caption, wchar_t accessKey, PRBool isDisabled,
    CMenu *subMenu)
{
  //OutputDebugString(L"CMenuItem constructor");
  mIsSeparator = PR_FALSE;
  mCaption.Assign(caption);
  mAccessKey = accessKey;
  mIdCmd = 0;
  mIsDefault = PR_FALSE;
  mIsDisabled = isDisabled;
  CallQueryInterface(subMenu, &mSubMenu);
}

CMenuItem::~CMenuItem()
{
  NS_IF_RELEASE(mSubMenu);
  //OutputDebugString(L"CMenuItem destructor");
}

/* readonly attribute AString caption; */
NS_IMETHODIMP CMenuItem::GetCaption(nsAString & aCaption)
{
  aCaption.Assign(mCaption);
  return NS_OK;
}

/* readonly attribute wchar accessKey; */
NS_IMETHODIMP CMenuItem::GetAccessKey(PRUnichar *aAccessKey)
{
  *aAccessKey = mAccessKey;
  return NS_OK;
}

/* readonly attribute boolean isSeparator; */
NS_IMETHODIMP CMenuItem::GetIsSeparator(PRBool *aIsSeparator)
{
  *aIsSeparator = mIsSeparator;
  return NS_OK;
}

/* readonly attribute boolean isDefault; */
NS_IMETHODIMP CMenuItem::GetIsDefault(PRBool *aIsDefault)
{
  *aIsDefault = mIsDefault;
  return NS_OK;
}

/* readonly attribute boolean isDisabled; */
NS_IMETHODIMP CMenuItem::GetIsDisabled(PRBool *aIsDisabled)
{
  *aIsDisabled = mIsDisabled;
  return NS_OK;
}

/* readonly attribute boolean hasSubMenu; */
NS_IMETHODIMP CMenuItem::GetHasSubMenu(PRBool *aHasSubMenu)
{
  *aHasSubMenu = mSubMenu != NULL ? PR_TRUE : PR_FALSE;
  return NS_OK;
}

/* readonly attribute IMenu subMenu; */
NS_IMETHODIMP CMenuItem::GetSubMenu(IMenu **aSubMenu)
{
  *aSubMenu = mSubMenu;
  NS_IF_ADDREF(*aSubMenu);
  return NS_OK;
}

/* readonly attribute unsigned long idCmd; */
NS_IMETHODIMP CMenuItem::GetIdCmd(PRUint32 *aIdCmd)
{
  *aIdCmd = mIdCmd;
  return NS_OK;
}

bool CMenuItem::isSeparator()
{
  return mIsSeparator == PR_TRUE;
}

CMenu::CMenu(vector<CMenuItem*> *items)
{
  IMenuItem *item;
  for (unsigned int i = 0; i < items->size(); i++)
  {
    CallQueryInterface(items->at(i), &item);
    if (item != NULL)
      mItems.push_back(item);
  }
  //OutputDebugString(L"CMenu constructor");
}

CMenu::~CMenu()
{
  //OutputDebugString(L"CMenu destructor");
  IMenuItem *item;
  while (!mItems.empty())
  {
    item = mItems.front();
    mItems.erase(mItems.begin());
    NS_RELEASE(item);
  }
}

/* readonly attribute unsigned long itemCount; */
NS_IMETHODIMP CMenu::GetItemCount(PRUint32 *aItemCount)
{
  *aItemCount = mItems.size();
  return NS_OK;
}

/* IMenuItem getMenuItem (in unsigned long idx); */
NS_IMETHODIMP CMenu::GetMenuItem(PRUint32 idx, IMenuItem **_retval)
{
  *_retval = NULL;
  if (idx < mItems.size()) 
  {
    *_retval = mItems.at(idx);
    NS_ADDREF(*_retval);
  }
  return NS_OK;
}