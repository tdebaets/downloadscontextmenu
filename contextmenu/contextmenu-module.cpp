#include "nsIGenericFactory.h"
#include "contextmenuhelper-impl.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(CContextMenuHelper)

static nsModuleComponentInfo components[] =
{
    {
       CONTEXTMENU_CLASSNAME, 
       CONTEXTMENU_CID,
       CONTEXTMENU_CONTRACTID,
       CContextMenuHelperConstructor,
    }
};

NS_IMPL_NSGETMODULE("ContextMenuModule", components) 
