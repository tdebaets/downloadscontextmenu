#include "mozilla/ModuleUtils.h"
#include "contextmenuhelper-impl.h"

NS_GENERIC_FACTORY_CONSTRUCTOR(CContextMenuHelper)

// old code for Gecko 1.9.2

//static nsModuleComponentInfo components[] =
//{
//    {
//       CONTEXTMENU_CLASSNAME, 
//       CONTEXTMENU_CID,
//       CONTEXTMENU_CONTRACTID,
//       CContextMenuHelperConstructor,
//    }
//};
//
//NS_IMPL_NSGETMODULE("ContextMenuModule", components) 

// new code for Gecko 2.0

NS_DEFINE_NAMED_CID(CONTEXTMENU_CID);

static const mozilla::Module::CIDEntry kContextMenuCIDs[] = {
    { &kCONTEXTMENU_CID, false, NULL, CContextMenuHelperConstructor },
    { NULL }
};

static const mozilla::Module::ContractIDEntry kContextMenuContracts[] = {
    { CONTEXTMENU_CONTRACTID, &kCONTEXTMENU_CID },
    { NULL }
};

static const mozilla::Module kContextMenuModule = {
    mozilla::Module::kVersion,
    kContextMenuCIDs,
    kContextMenuContracts
};

NSMODULE_DEFN(ContextMenuModule) = &kContextMenuModule;

NS_IMPL_MOZILLA192_NSGETMODULE(&kContextMenuModule)
