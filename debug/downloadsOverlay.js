
var Cc = Components.classes;
var Ci = Components.interfaces;

window.addEventListener("load", function(e) { downloadsctxmenu.onLoad(e); },
  false);

var downloadsctxmenu = {

  _contextMenuComp: null,
  _downloadsView: null,
  
  onLoad: function() {
    // initialization code
    try {
      const cid = "@bmproductions.fixnum.org/contextmenu;1";
      var obj = Cc[cid].createInstance();
      this._contextMenuComp = obj.QueryInterface(Ci.IContextMenuHelper);
    }
    catch (err) {
      alert(err);
    }
    this._downloadsView = document.getElementById("downloadView");
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
      if (file && this._contextMenuComp)
        this.insertMenuItem(popup, !file.exists());
    }
  },
  
  insertMenuItem: function(popup, disabled) {
    var menuItemOpen;
    var childNodes = popup.childNodes;
    for (var i = 0; i < childNodes.length; i++) {
      var child = childNodes[i];
      var id = child.getAttribute("id");
      if (id == "menuitem_open")
        menuItemOpen = child;
      else if (id == "menuitem_show" && menuItemOpen) {
        var menu = this.createMenu(disabled);
        popup.insertBefore(menu, child.nextSibling);
        break;
      }
    }
  },

  createMenu: function(disabled) {
    /*var menu = document.createElement("menu"); 
    menu.setAttribute("label", _menuItemLabel);*/
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
      while (popup.hasChildNodes())
        popup.removeChild(popup.firstChild);
      var file = this.getSelectedFile();
      if (!file || !file.exists())
        return false;
      var ctxMenu =
        this._contextMenuComp.getContextMenuForFile(file.path);
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
    popup.downloadsCtxMenu = null;
    var childNodes = popup.childNodes;
    for (var i = 0; i < childNodes.length; i++) {
      var child = childNodes[i];
      if (child.nodeName == "menu" && child.childNodes.length > 0)
        this.removePopupObjects(child.childNodes[0]);
    }
  },
  
  onContextMenuHide: function(event) {
    var popup = event.target;
    // allow the garbage collector to free the Menu and MenuItem XPCOM objects
    if (popup)
      this.removePopupObjects(popup);
  },
  
  onContextMenuItemCommand: function(event) {
    var target = event.target;
    if (target.parentNode.downloadsCtxMenu && target.ctxMenuIdCmd) {
      target.parentNode.downloadsCtxMenu.invokeCommand(target.ctxMenuIdCmd);
    }
  },
  
  setMenuItemAttributes: function(item, ctxMenuItem) {
    item.setAttribute("label", ctxMenuItem.caption);
    var accesskey = ctxMenuItem.accessKey;
    if (accesskey != '\u0000')
      item.setAttribute("accesskey", accesskey);
    if (ctxMenuItem.isDisabled)
      item.setAttribute("disabled", true);
  },
  
  fillPopupWithIMenu: function(popup, menu, ctxMenu) {
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
  
  getSelectedFile: function() {
    if (this._downloadsView.selectedItem) {
      var dl = this._downloadsView.selectedItem;
      if (dl)
        return getLocalFileFromNativePathOrUrl(dl.getAttribute("file"));
    }
  },
  
  onGarbageCollectClick: function() {
    var req = window.QueryInterface(Components.interfaces.nsIInterfaceRequestor); 
    var utils = req.getInterface(Components.interfaces.nsIDOMWindowUtils); 
    utils.garbageCollect();
    alert("ok");
  },
  
};

function dump(message) {
  var consoleService = Cc["@mozilla.org/consoleservice;1"]
                       .getService(Ci.nsIConsoleService);
  consoleService.logStringMessage(message);
}