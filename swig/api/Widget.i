%module ilixi_Widget

%import(module="ilixi_Enums") <types/Enums.h>
%import(module="ilixi_Size") <types/Size.h>

%{
#include <ilixiGUI.h>
using namespace ilixi;
%}

%include <ui/Widget.h>