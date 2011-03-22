
var Cc = Components.classes;
var Ci = Components.interfaces;

window.addEventListener("load", function(e) { downloadsctxmenu.onLoad(e); },
  false);

var downloadsctxmenu = {

  // IContextMenuHelper XPCOM component for getting the context menu of a file
  _contextMenuComp: null, 
  
  onLoad: function() {
    // initialization code
    // create IContextMenuHelper XPCOM component
    try {
      const cid = "@bmproductions.fixnum.org/contextmenu;1";
      var obj = Cc[cid].createInstance();
      this._contextMenuComp = obj.QueryInterface(Ci.IContextMenuHelper);
    }
    catch (err) {
      alert(err);
    }
    var contextMenu = document.getElementById("downloadContextMenu");
    if (contextMenu)
      contextMenu.addEventListener("popupshowing",
        function(e) { downloadsctxmenu.onDownloadContextMenuPopup(e); },
				false)
  },

  onDownloadContextMenuPopup: function(event) {
    var popup = document.getElementById("downloadContextMenu");
    //without this *simple* guard here, life can be very very frustrating...
    if (event.target == popup){
      var file = this.getSelectedFile();
      if (file && this._contextMenuComp) {
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
    //without this *simple* guard here, life can be very very frustrating...
    if (event.target == popup) {
      // remove all previous items
      while (popup.hasChildNodes())
        popup.removeChild(popup.firstChild);
      var file = this.getSelectedFile();
      if (!file || !file.exists())
        return false;
      // use IContextMenuHelper to get the context menu
      var ctxMenu =
        this._contextMenuComp.getContextMenuForFile(file.path);
      // fill popup with the items of the returned ICtxMenu XPCOM object
      if (ctxMenu)
        this.fillPopupWithIMenu(popup, ctxMenu.menu, ctxMenu);
      else
        return false;
    }
    // we need to stop propagation, because buildContextMenu in downloads.js
    // returns false for any id other than "downloadContextMenu".
    event.stopPropagation();
    return true;
  },
  
  removePopupObjects: function(popup) {
    // remove all references to ICtxMenu in this popup and any submenus
    popup.downloadsCtxMenu = null;
    // iterate submenus
    var childNodes = popup.childNodes;
    for (var i = 0; i < childNodes.length; i++) {
      var child = childNodes[i];
      if (child.nodeName == "menu" && child.childNodes.length > 0)
        this.removePopupObjects(child.childNodes[0]);
    }
  },
  
  onContextMenuHide: function(event) {
    var popup = event.target;
    // allow the garbage collector to free the ICtxMenu XPCOM object
    if (popup)
      this.removePopupObjects(popup);
  },
  
  onContextMenuItemCommand: function(event) {
    var target = event.target;
    if (target.parentNode.downloadsCtxMenu && target.ctxMenuIdCmd) {
      // invoke the command by calling ICtxMenu of the item's parent menu and
      // providing the idCmd of the item
      target.parentNode.downloadsCtxMenu.invokeCommand(target.ctxMenuIdCmd);
    }
  },
  
  setMenuItemAttributes: function(item, ctxMenuItem) {
    // set common attributes of a menu or menuitem based on an IMenuItem object
    item.setAttribute("label", ctxMenuItem.caption);
    var accesskey = ctxMenuItem.accessKey;
    if (accesskey != '\u0000')
      item.setAttribute("accesskey", accesskey);
    if (ctxMenuItem.isDisabled)
      item.setAttribute("disabled", true);
  },
  
  fillPopupWithIMenu: function(popup, menu, ctxMenu) {
    // fills popup with the menu items of an IMenu object
    // save reference to the ICtxMenu object for invoking commands
    popup.downloadsCtxMenu = ctxMenu;
    for (var i = 0; i < menu.itemCount; i++) {
      var item = menu.getMenuItem(i);
      if (item) {
        if (item.isSeparator)
          popup.appendChild(document.createElement("menuseparator"));
        else if (item.hasSubMenu) {
          var subMenu = document.createElement("menu");
          this.setMenuItemAttributes(subMenu, item);
          var subPopup = document.createElement("menupopup");
          // recursive call for submenus
          this.fillPopupWithIMenu(subPopup, item.subMenu, ctxMenu);
          subMenu.appendChild(subPopup);
          popup.appendChild(subMenu);
        }
        else {
          var menuitem = document.createElement("menuitem");
          this.setMenuItemAttributes(menuitem, item);
          menuitem.ctxMenuIdCmd = item.idCmd;
          menuitem.setAttribute("oncommand",
            "downloadsctxmenu.onContextMenuItemCommand(event);");
          popup.appendChild(menuitem);
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