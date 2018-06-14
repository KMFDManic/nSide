CheatDatabase::CheatDatabase() {
  cheatDatabase = this;
  refreshLocale();

  layout.setMargin(5);
  selectAllButton.onActivate([&] {
    for(auto& item : cheatList.items()) item.setChecked(true);
  });
  unselectAllButton.onActivate([&] {
    for(auto& item : cheatList.items()) item.setChecked(false);
  });
  addCodesButton.onActivate([&] { addCodes(); });

  setSize({800, 400});
  setAlignment({0.5, 1.0});
  setDismissable();
}

auto CheatDatabase::refreshLocale() -> void {
  selectAllButton.setText(locale["Tools/CheatDatabase/SelectAll"]);
  unselectAllButton.setText(locale["Tools/CheatDatabase/UnselectAll"]);
  addCodesButton.setText(locale["Tools/CheatDatabase/AddCodes"]);

  setSize(geometry().size());
}

auto CheatDatabase::findCodes() -> void {
  if(!emulator) return;
  auto sha256 = emulator->sha256();

  auto contents = string::read(locate("cheats.bml"));
  auto document = BML::unserialize(contents);

  for(auto cartridge : document.find("cartridge")) {
    if(cartridge["sha256"].text() != sha256) continue;

    codes.reset();
    cheatList.reset();
    for(auto cheat : cartridge.find("cheat")) {
      codes.append(cheat["code"].text());
      cheatList.append(ListViewItem().setCheckable().setText(cheat["description"].text()));
    }

    setTitle(cartridge["name"].text());
    setVisible();
    return;
  }

  MessageDialog().setParent(*toolsManager).setText("Sorry, no cheats were found for this game.").information();
}

auto CheatDatabase::addCodes() -> void {
  for(auto& item : cheatList.items()) {
    if(!item.checked()) continue;

    string code = codes(item.offset(), "");
    string description = item.text();
    if(toolsManager->cheatEditor.addCode(code, description) == false) {
      MessageDialog().setParent(*this).setText("Free slots exhausted. Not all codes could be added.").warning();
      break;
    }
  }
  setVisible(false);
  toolsManager->cheatEditor.doRefresh();
}