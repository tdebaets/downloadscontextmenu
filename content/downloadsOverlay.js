

var Cc = Components.classes;
var Ci = Components.interfaces;

window.addEventListener("load", function(e) { downloadsctxmenu.onLoad(e); },
  false);

var downloadsctxmenu = {
                         
  // contextmenu binary ctypes library
  _contextMenuLib: null,
  
  // contextmenu binary library structs
  _CMMenuItem: null,
  _CMMenu: null,
  _CMContextMenu: null,
  
  // contextmenu binary library functions
  _CMGetContextMenuForFile: null,
  _CMContextMenuCommand: null,
  _CMFreeContextMenu: null,
  
  declareContextMenuStructs: function() {
  
    // forward declaration of CMMenu
    this._CMMenu = new ctypes.StructType("CMMenu");
    
    this._CMMenuItem = new ctypes.StructType("CMMenuItem",
                            [ { "caption": ctypes.jschar.ptr },
                              { "accessKey": ctypes.jschar },
                              { "isSeparator": ctypes.bool },
                              { "isDefault": ctypes.bool },
                              { "isDisabled": ctypes.bool },
                              { "subMenu": this._CMMenu.ptr },
                              { "idCmd": ctypes.unsigned_int } ]);
                              
    this._CMMenu.define(    [ { "itemCount": ctypes.unsigned_int },
                              { "items": this._CMMenuItem.array().ptr } ]);
                              
    this._CMContextMenu = new ctypes.StructType("CMContextMenu",
                            [ { "__handle": ctypes.voidptr_t /* internal use only */ },
                              { "menu": this._CMMenu.ptr } ]);
                              
  },
  
  onLoad: function() {
    // initialization code
    window.addEventListener("unload", function(e) { downloadsctxmenu.onUnload(e); },
      false);
    // load contextmenu binary component
    Components.utils.import("resource://gre/modules/ctypes.jsm");
    Components.utils.import("resource://gre/modules/AddonManager.jsm");
    AddonManager.getAddonByID("downloadscontextmenu@bmproductions",
        function(addon) { downloadsctxmenu.addonCallback(addon); } );
    // get our contextmenu popup (see downloadsOverlay.xul)
    var contextMenu = document.getElementById("downloadsContextMenu");
    if (contextMenu) {
      contextMenu.addEventListener("popupshowing",
          function(e) { downloadsctxmenu.onDownloadContextMenuPopup(e); },
          false);
    }
  },
  
  addonCallback: function(addon) {
    var uri = addon.getResourceURI("components/contextmenu.dll");
    if (uri instanceof Components.interfaces.nsIFileURL) {
      this._contextMenuLib = ctypes.open(uri.file.path);
      if (this._contextMenuLib) {
        this.declareContextMenuStructs();
        this._CMGetContextMenuForFile =
            this._contextMenuLib.declare("CMGetContextMenuForFile",
                                         ctypes.winapi_abi, // no name mangling
                                         this._CMContextMenu.ptr,
                                         ctypes.jschar.ptr);
        this._CMContextMenuCommand =
            this._contextMenuLib.declare("CMContextMenuCommand",
                                         ctypes.winapi_abi, // no name mangling
                                         ctypes.bool,
                                         this._CMContextMenu.ptr,
                                         this._CMMenuItem.ptr);
        this._CMFreeContextMenu =
            this._contextMenuLib.declare("CMFreeContextMenu",
                                         ctypes.winapi_abi, // no name mangling
                                         ctypes.void_t,
                                         this._CMContextMenu.ptr);
      }
    }
  },
  
  onUnload: function() {
    if (this._contextMenuLib)
      this._contextMenuLib.close();
  },

  onDownloadContextMenuPopup: function(event) {
    var popup = document.getElementById("downloadsContextMenu");
    // without this *simple* guard here, life can be very very frustrating...
    if (event.target == popup) {
      var file = this.getSelectedFile();
      if (file && this._contextMenuLib) {
        this.insertMenuItem(popup, !file.exists());
      }
    }
  },
  
  insertMenuItem: function(popup, disabled) {
    var menuItemOpen;
    var childNodes = popup.childNodes;
    // search for the Open and Show menu items
    for (var i = 0; i < childNodes.length; i++) {
      var child = childNodes[i];
      var id = child.getAttribute("id");
      if (id == "menuitem_open")
        menuItemOpen = child;
      // insert our menu after Show, and only if Open has been found too
      else if (id == "menuitem_show" && menuItemOpen) {
        var menu = this.createMenu(disabled);
        popup.insertBefore(menu, child.nextSibling);
        break;
      }
    }
  },

  createMenu: function(disabled) {
    var menu =
      document.getElementById("downloadscontextmenuItem").cloneNode(true);
    menu.setAttribute("disabled", disabled);
    var popup = document.createElement("menupopup");
    popup.id = "downloadscontextmenupopup";
    popup.setAttribute("onpopupshowing",
      "return downloadsctxmenu.onContextMenuPopup(event);");
    popup.setAttribute("onpopuphiding",
      "downloadsctxmenu.onContextMenuHide(event);");
    menu.appendChild(popup);
    return menu;
  },
  
  onContextMenuPopup: function(event) {
    var popup = document.getElementById("downloadscontextmenupopup");
    // without this *simple* guard here, life can be very very frustrating...
    if (event.target == popup) {
      // remove all previous items
      while (popup.hasChildNodes())
        popup.removeChild(popup.firstChild);
      var file = this.getSelectedFile();
      if (!file || !file.exists())
        return false;
      // use contextmenu library to get the context menu
      var cmCtxMenu = this._CMGetContextMenuForFile(file.path);
      if (cmCtxMenu.isNull() || cmCtxMenu.contents.menu.isNull())
        return false;
      else {
        // fill popup with the items of the returned CMContextMenu struct
        this.fillPopupWithIMenu(popup, cmCtxMenu.contents.menu.contents, cmCtxMenu);
      }
    }
    // we need to stop propagation, because buildContextMenu in downloads.js
    // returns false for any id other than "downloadContextMenu".
    event.stopPropagation();
    return true;
  },
  
  removePopupObjects: function(popup) {
    // remove all references to CMContextMenu in this popup and any submenus
    popup.downloadsCtxMenu = null;
    // iterate submenus
    var childNodes = popup.childNodes;
    for (var i = 0; i < childNodes.length; i++) {
      var child = childNodes[i];
      child.cmMenuItem = null;
      if (child.nodeName == "menu" && child.childNodes.length > 0)
        this.removePopupObjects(child.childNodes[0]);
    }
  },
  
  onContextMenuHide: function(event) {
    var popup = event.target;
    // only free if the main popup is hidden (this listener also gets called for submenus)
    if ( (popup.id == "downloadscontextmenupopup") && popup.downloadsCtxMenu)
    {
      // free the CMContextMenu struct returned by CMGetContextMenuForFile
      this._CMFreeContextMenu(popup.downloadsCtxMenu);
      // to be safe, remove all the CMContextMenu references in the popup
      this.removePopupObjects(popup);
    }
  },
  
  onContextMenuItemCommand: function(event) {
    var target = event.target;
    if (target.parentNode.downloadsCtxMenu && target.cmMenuItem) {
      // invoke the command by calling CMContextMenuCommand and providing the
      // CMContextMenu struct of the item's parent menu together with the
      // CMMenuItem struct of the item
      this._CMContextMenuCommand(target.parentNode.downloadsCtxMenu,
          target.cmMenuItem.address());
    }
  },
  
  setMenuItemAttributes: function(item, cmMenuItem) {
    // set common attributes of a menuitem based on a CMMenuItem struct
    item.cmMenuItem = cmMenuItem;
    item.setAttribute("label", cmMenuItem.caption.readString());
    var accesskey = cmMenuItem.accessKey;
    if (accesskey != '\u0000')
      item.setAttribute("accesskey", accesskey);
    if (cmMenuItem.isDisabled)
      item.setAttribute("disabled", true);
  },
  
  fillPopupWithIMenu: function(popup, cmMenu, cmCtxMenu) {
    // fills popup with the menu items of a CMMenu struct
    // save reference to the CMContextMenu struct for invoking commands (and releasing) later
    popup.downloadsCtxMenu = cmCtxMenu;
    var CMMenuItemArray = new ctypes.ArrayType(this._CMMenuItem, cmMenu.itemCount);
    var items = ctypes.cast(cmMenu.items, CMMenuItemArray.ptr);
    for (var i = 0; i < cmMenu.itemCount; i++) {
      var item = items.contents[i];
      if (item) {
        if (item.isSeparator)
          // separator
          popup.appendChild(document.createElement("menuseparator"));
        else if (item.subMenu.isNull()) {
          // menu item
          var menuitem = document.createElement("menuitem");
          this.setMenuItemAttributes(menuitem, item);
          menuitem.setAttribute("oncommand",
            "downloadsctxmenu.onContextMenuItemCommand(event);");
          popup.appendChild(menuitem);
        }
        else {
          // submenu
          var subMenu = document.createElement("menu");
          this.setMenuItemAttributes(subMenu, item);
          var subPopup = document.createElement("menupopup");
          // recursive call for submenus
          this.fillPopupWithIMenu(subPopup, item.subMenu.contents, cmCtxMenu);
          subMenu.appendChild(subPopup);
          popup.appendChild(subMenu);
        }
      }
    }
  },
  
  getDownloadsView: function() {
    // get the downloads view, don't save in a variable because it changes
    // when the download list is cleared
    return document.getElementById("downloadView");
  },
  
  getSelectedFile: function() {
    // get the currently selected file in the Downloads view
    var dl = this.getDownloadsView().selectedItem;
    if (dl)
      return getLocalFileFromNativePathOrUrl(dl.getAttribute("file"));
  },
  
};

/*function dump(message) {
  var consoleService = Cc["@mozilla.org/consoleservice;1"]
                       .getService(Ci.nsIConsoleService);
  consoleService.logStringMessage(message);
}*/
