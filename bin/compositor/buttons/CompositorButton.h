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

#ifndef COMPOSITORBUTTON_H_
#define COMPOSITORBUTTON_H_

#include "ui/Widget.h"
#include "types/Image.h"
#include "lib/TweenAnimation.h"

namespace ilixi
{

  class CompositorButton : public Widget
  {
  public:
    CompositorButton(const std::string& imagePath, Widget* parent = 0);

    virtual
    ~CompositorButton();

    virtual Size
    preferredSize() const;

    void
    show();

    void
    hide();

    sigc::signal<void> sigPressed;

  protected:
    bool _click;
    TweenAnimation _anim;
    Tween* _tween;
    Image* _image;

    virtual void
    pointerButtonDownEvent(const PointerEvent& pointerEvent);

    virtual void
    pointerButtonUpEvent(const PointerEvent& pointerEvent);

  private:
    void
    tweenSlot();

    void
    tweenEndSlot();
  };

} /* namespace ilixi */
#endif /* COMPOSITORBUTTON_H_ */
