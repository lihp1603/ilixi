/*
 Copyright 2010-2012 Tarik Sekmen.

 All Rights Reserved.

 Written by Tarik Sekmen <tarik@ilixi.org>.

 This file is part of ilixi.

 ilixi is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ilixi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with ilixi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Demo2.h"
#include "ScrollItem.h"
#include "ui/ScrollArea.h"
#include "ui/HBoxLayout.h"
#include "ui/VBoxLayout.h"
#include "ui/PushButton.h"
#include <sigc++/bind.h>

using namespace ilixi;

Demo2::Demo2(int argc, char* argv[]) :
    Application(&argc, &argv)
{
  setTitle("Demo 2");
  setBackgroundFilled(true);
  setLayout(new HBoxLayout());

  ScrollArea* area = new ScrollArea();
  addWidget(area);
//  area->setGeometry(0, 0, 350, 350);
  area->setMaximumSize(350, 900);

  _icon = new Icon();
//  _icon->setGeometry(355, 0, 400, 350);
  _icon->setMinimumSize(400, 350);
  addWidget(_icon);

  // Add scrollitems
  VBoxLayout* box = new VBoxLayout();
  box->setSpacing(0);
  area->setContent(box);

  ScrollItem* item;
  char file[128];
  for (int i = 0; i < 40; ++i)
    {
      sprintf(file, "%sgallery/%d.jpg", ILIXI_DATADIR, i % 5);
      item = new ScrollItem(file);
      item->setImage(new Image(file, 50, 50));
      box->addWidget(item);
      item->sigPressed.connect(sigc::mem_fun(this, &Demo2::showImage));
    }
}

Demo2::~Demo2()
{
}

void
Demo2::showImage(const std::string& file)
{
  _icon->setDefaultImage(file);
  _icon->update();
}

int
main(int argc, char* argv[])
{
  Demo2 app(argc, argv);
  app.exec();
  return 0;
}
