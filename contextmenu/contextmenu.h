/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM contextmenu.idl
 */

#ifndef __gen_contextmenu_h__
#define __gen_contextmenu_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif
class IMenu; /* forward declaration */


/* starting interface:    IMenuItem */
#define IMENUITEM_IID_STR "be8df6fd-11ed-40a0-bc78-ada9e2caf6d7"

#define IMENUITEM_IID \
  {0xbe8df6fd, 0x11ed, 0x40a0, \
    { 0xbc, 0x78, 0xad, 0xa9, 0xe2, 0xca, 0xf6, 0xd7 }}

class NS_NO_VTABLE NS_SCRIPTABLE IMenuItem : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IMENUITEM_IID)

  /* readonly attribute AString caption; */
  NS_SCRIPTABLE NS_IMETHOD GetCaption(nsAString & aCaption) = 0;

  /* readonly attribute wchar accessKey; */
  NS_SCRIPTABLE NS_IMETHOD GetAccessKey(PRUnichar *aAccessKey) = 0;

  /* readonly attribute boolean isSeparator; */
  NS_SCRIPTABLE NS_IMETHOD GetIsSeparator(PRBool *aIsSeparator) = 0;

  /* readonly attribute boolean isDefault; */
  NS_SCRIPTABLE NS_IMETHOD GetIsDefault(PRBool *aIsDefault) = 0;

  /* readonly attribute boolean isDisabled; */
  NS_SCRIPTABLE NS_IMETHOD GetIsDisabled(PRBool *aIsDisabled) = 0;

  /* readonly attribute boolean hasSubMenu; */
  NS_SCRIPTABLE NS_IMETHOD GetHasSubMenu(PRBool *aHasSubMenu) = 0;

  /* readonly attribute IMenu subMenu; */
  NS_SCRIPTABLE NS_IMETHOD GetSubMenu(IMenu **aSubMenu) = 0;

  /* readonly attribute unsigned long idCmd; */
  NS_SCRIPTABLE NS_IMETHOD GetIdCmd(PRUint32 *aIdCmd) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(IMenuItem, IMENUITEM_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IMENUITEM \
  NS_SCRIPTABLE NS_IMETHOD GetCaption(nsAString & aCaption); \
  NS_SCRIPTABLE NS_IMETHOD GetAccessKey(PRUnichar *aAccessKey); \
  NS_SCRIPTABLE NS_IMETHOD GetIsSeparator(PRBool *aIsSeparator); \
  NS_SCRIPTABLE NS_IMETHOD GetIsDefault(PRBool *aIsDefault); \
  NS_SCRIPTABLE NS_IMETHOD GetIsDisabled(PRBool *aIsDisabled); \
  NS_SCRIPTABLE NS_IMETHOD GetHasSubMenu(PRBool *aHasSubMenu); \
  NS_SCRIPTABLE NS_IMETHOD GetSubMenu(IMenu **aSubMenu); \
  NS_SCRIPTABLE NS_IMETHOD GetIdCmd(PRUint32 *aIdCmd); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IMENUITEM(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetCaption(nsAString & aCaption) { return _to GetCaption(aCaption); } \
  NS_SCRIPTABLE NS_IMETHOD GetAccessKey(PRUnichar *aAccessKey) { return _to GetAccessKey(aAccessKey); } \
  NS_SCRIPTABLE NS_IMETHOD GetIsSeparator(PRBool *aIsSeparator) { return _to GetIsSeparator(aIsSeparator); } \
  NS_SCRIPTABLE NS_IMETHOD GetIsDefault(PRBool *aIsDefault) { return _to GetIsDefault(aIsDefault); } \
  NS_SCRIPTABLE NS_IMETHOD GetIsDisabled(PRBool *aIsDisabled) { return _to GetIsDisabled(aIsDisabled); } \
  NS_SCRIPTABLE NS_IMETHOD GetHasSubMenu(PRBool *aHasSubMenu) { return _to GetHasSubMenu(aHasSubMenu); } \
  NS_SCRIPTABLE NS_IMETHOD GetSubMenu(IMenu **aSubMenu) { return _to GetSubMenu(aSubMenu); } \
  NS_SCRIPTABLE NS_IMETHOD GetIdCmd(PRUint32 *aIdCmd) { return _to GetIdCmd(aIdCmd); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IMENUITEM(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetCaption(nsAString & aCaption) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetCaption(aCaption); } \
  NS_SCRIPTABLE NS_IMETHOD GetAccessKey(PRUnichar *aAccessKey) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetAccessKey(aAccessKey); } \
  NS_SCRIPTABLE NS_IMETHOD GetIsSeparator(PRBool *aIsSeparator) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsSeparator(aIsSeparator); } \
  NS_SCRIPTABLE NS_IMETHOD GetIsDefault(PRBool *aIsDefault) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsDefault(aIsDefault); } \
  NS_SCRIPTABLE NS_IMETHOD GetIsDisabled(PRBool *aIsDisabled) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIsDisabled(aIsDisabled); } \
  NS_SCRIPTABLE NS_IMETHOD GetHasSubMenu(PRBool *aHasSubMenu) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHasSubMenu(aHasSubMenu); } \
  NS_SCRIPTABLE NS_IMETHOD GetSubMenu(IMenu **aSubMenu) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetSubMenu(aSubMenu); } \
  NS_SCRIPTABLE NS_IMETHOD GetIdCmd(PRUint32 *aIdCmd) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetIdCmd(aIdCmd); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public IMenuItem
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IMENUITEM

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, IMenuItem)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* readonly attribute AString caption; */
NS_IMETHODIMP _MYCLASS_::GetCaption(nsAString & aCaption)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute wchar accessKey; */
NS_IMETHODIMP _MYCLASS_::GetAccessKey(PRUnichar *aAccessKey)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isSeparator; */
NS_IMETHODIMP _MYCLASS_::GetIsSeparator(PRBool *aIsSeparator)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isDefault; */
NS_IMETHODIMP _MYCLASS_::GetIsDefault(PRBool *aIsDefault)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isDisabled; */
NS_IMETHODIMP _MYCLASS_::GetIsDisabled(PRBool *aIsDisabled)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean hasSubMenu; */
NS_IMETHODIMP _MYCLASS_::GetHasSubMenu(PRBool *aHasSubMenu)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute IMenu subMenu; */
NS_IMETHODIMP _MYCLASS_::GetSubMenu(IMenu **aSubMenu)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long idCmd; */
NS_IMETHODIMP _MYCLASS_::GetIdCmd(PRUint32 *aIdCmd)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    IMenu */
#define IMENU_IID_STR "d3405a37-500f-4242-8bfd-e6833c17431b"

#define IMENU_IID \
  {0xd3405a37, 0x500f, 0x4242, \
    { 0x8b, 0xfd, 0xe6, 0x83, 0x3c, 0x17, 0x43, 0x1b }}

class NS_NO_VTABLE NS_SCRIPTABLE IMenu : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IMENU_IID)

  /* readonly attribute unsigned long itemCount; */
  NS_SCRIPTABLE NS_IMETHOD GetItemCount(PRUint32 *aItemCount) = 0;

  /* IMenuItem getMenuItem (in unsigned long idx); */
  NS_SCRIPTABLE NS_IMETHOD GetMenuItem(PRUint32 idx, IMenuItem **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(IMenu, IMENU_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IMENU \
  NS_SCRIPTABLE NS_IMETHOD GetItemCount(PRUint32 *aItemCount); \
  NS_SCRIPTABLE NS_IMETHOD GetMenuItem(PRUint32 idx, IMenuItem **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IMENU(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetItemCount(PRUint32 *aItemCount) { return _to GetItemCount(aItemCount); } \
  NS_SCRIPTABLE NS_IMETHOD GetMenuItem(PRUint32 idx, IMenuItem **_retval NS_OUTPARAM) { return _to GetMenuItem(idx, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IMENU(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetItemCount(PRUint32 *aItemCount) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetItemCount(aItemCount); } \
  NS_SCRIPTABLE NS_IMETHOD GetMenuItem(PRUint32 idx, IMenuItem **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMenuItem(idx, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public IMenu
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IMENU

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, IMenu)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* readonly attribute unsigned long itemCount; */
NS_IMETHODIMP _MYCLASS_::GetItemCount(PRUint32 *aItemCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* IMenuItem getMenuItem (in unsigned long idx); */
NS_IMETHODIMP _MYCLASS_::GetMenuItem(PRUint32 idx, IMenuItem **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    ICtxMenu */
#define ICTXMENU_IID_STR "53e0031a-ec32-4994-96ae-f2d0ffaf612c"

#define ICTXMENU_IID \
  {0x53e0031a, 0xec32, 0x4994, \
    { 0x96, 0xae, 0xf2, 0xd0, 0xff, 0xaf, 0x61, 0x2c }}

class NS_NO_VTABLE NS_SCRIPTABLE ICtxMenu : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(ICTXMENU_IID)

  /* readonly attribute IMenu menu; */
  NS_SCRIPTABLE NS_IMETHOD GetMenu(IMenu **aMenu) = 0;

  /* boolean invokeCommand (in unsigned long idCmd); */
  NS_SCRIPTABLE NS_IMETHOD InvokeCommand(PRUint32 idCmd, PRBool *_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(ICtxMenu, ICTXMENU_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_ICTXMENU \
  NS_SCRIPTABLE NS_IMETHOD GetMenu(IMenu **aMenu); \
  NS_SCRIPTABLE NS_IMETHOD InvokeCommand(PRUint32 idCmd, PRBool *_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_ICTXMENU(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetMenu(IMenu **aMenu) { return _to GetMenu(aMenu); } \
  NS_SCRIPTABLE NS_IMETHOD InvokeCommand(PRUint32 idCmd, PRBool *_retval NS_OUTPARAM) { return _to InvokeCommand(idCmd, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_ICTXMENU(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetMenu(IMenu **aMenu) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetMenu(aMenu); } \
  NS_SCRIPTABLE NS_IMETHOD InvokeCommand(PRUint32 idCmd, PRBool *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->InvokeCommand(idCmd, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public ICtxMenu
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_ICTXMENU

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, ICtxMenu)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* readonly attribute IMenu menu; */
NS_IMETHODIMP _MYCLASS_::GetMenu(IMenu **aMenu)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean invokeCommand (in unsigned long idCmd); */
NS_IMETHODIMP _MYCLASS_::InvokeCommand(PRUint32 idCmd, PRBool *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


/* starting interface:    IContextMenuHelper */
#define ICONTEXTMENUHELPER_IID_STR "735ff095-e0b1-49b6-85bb-2d246e50cdca"

#define ICONTEXTMENUHELPER_IID \
  {0x735ff095, 0xe0b1, 0x49b6, \
    { 0x85, 0xbb, 0x2d, 0x24, 0x6e, 0x50, 0xcd, 0xca }}

class NS_NO_VTABLE NS_SCRIPTABLE IContextMenuHelper : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(ICONTEXTMENUHELPER_IID)

  /* ICtxMenu getContextMenuForFile (in wstring filename); */
  NS_SCRIPTABLE NS_IMETHOD GetContextMenuForFile(const PRUnichar *filename, ICtxMenu **_retval NS_OUTPARAM) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(IContextMenuHelper, ICONTEXTMENUHELPER_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_ICONTEXTMENUHELPER \
  NS_SCRIPTABLE NS_IMETHOD GetContextMenuForFile(const PRUnichar *filename, ICtxMenu **_retval NS_OUTPARAM); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_ICONTEXTMENUHELPER(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetContextMenuForFile(const PRUnichar *filename, ICtxMenu **_retval NS_OUTPARAM) { return _to GetContextMenuForFile(filename, _retval); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_ICONTEXTMENUHELPER(_to) \
  NS_SCRIPTABLE NS_IMETHOD GetContextMenuForFile(const PRUnichar *filename, ICtxMenu **_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetContextMenuForFile(filename, _retval); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public IContextMenuHelper
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_ICONTEXTMENUHELPER

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, IContextMenuHelper)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* ICtxMenu getContextMenuForFile (in wstring filename); */
NS_IMETHODIMP _MYCLASS_::GetContextMenuForFile(const PRUnichar *filename, ICtxMenu **_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_contextmenu_h__ */
