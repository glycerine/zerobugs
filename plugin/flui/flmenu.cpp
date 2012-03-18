//
// -*- tab-width: 4; indent-tabs-mode: nil;  -*-
// vim: tabstop=4:softtabstop=4:expandtab:shiftwidth=4
//
// $Id$
//
#include "controller.h"
#include "flmenu.h"

using namespace std;


////////////////////////////////////////////////////////////////
FlCompositeMenu::FlCompositeMenu(ui::Controller& c)

  : ui::CompositeMenu()
  , controller_(c)
{
}


FlCompositeMenu::~FlCompositeMenu() throw()
{
}


void FlCompositeMenu::exec_command(const char* path)
{
    // find the menu item that issued the command

    for (auto mi = begin(children_); mi != end(children_); ++mi)
    {
        if (strcmp((*mi)->name().c_str(), path) == 0)
        {
            controller_.call_main_thread_async((*mi)->emit_command());
            break;
        }
    }
}


////////////////////////////////////////////////////////////////
FlMenuBar::FlMenuBar(

    ui::Controller& controller,
    int             width,
    int             height )

: FlCompositeMenu(controller)
, menu_(new Fl_Menu_Bar(0, 0, width, height))

{
}


FlMenuBar::~FlMenuBar() throw()
{
}


void FlMenuBar::add(

    const string&       name,
    int                 shortcut,
    ui::EnableCondition enable,
    RefPtr<ui::Command> command)

{
    int i = menu_->add(name.c_str(), shortcut, exec_command, this);
    RefPtr<ui::MenuElem> item(
        new FlMenuItem(name, shortcut, enable, command, menu_, i));

    ui::CompositeMenu::add(item);
}


/* static */
void FlMenuBar::exec_command(Fl_Widget* /* w */, void* p)
{
    auto* menu = reinterpret_cast<FlMenuBar*>(p);

    char path[100] = { 0 };
    assert(menu->menu_);

    menu->menu_->item_pathname(path, sizeof(path) - 1);
    menu->exec_command(path);
}


////////////////////////////////////////////////////////////////
FlPopupMenu::FlPopupMenu( ui::Controller& controller )
    : FlCompositeMenu(controller)
{
}


FlPopupMenu::~FlPopupMenu() throw()
{
}


void FlPopupMenu::show(int x, int y)
{
    if (!items_.empty())
    {
        items_.push_back(Fl_Menu_Item { 0 });
        if (auto* mi = items_.front().popup(x, y))
        {
            exec_command(mi->label());
        }
    }
}

void FlPopupMenu::add(

    const string&       name,
    int                 shortcut,
    ui::EnableCondition enable,
    RefPtr<ui::Command> command)

{
    RefPtr<ui::MenuElem> item =
        new ui::MenuItem(name, shortcut, enable, command);

    auto i = Fl_Menu_Item { item->name().c_str(), shortcut };
    items_.push_back(i);

    ui::CompositeMenu::add(item);
}


////////////////////////////////////////////////////////////////
FlMenuItem::FlMenuItem(

    const string&       name,
    int                 shortcut,
    ui::EnableCondition cond,
    RefPtr<ui::Command> cmd,
    Fl_Menu_*           menu,
    int                 index )

    : ui::MenuItem(name, shortcut, cond, cmd)
    , menu_(menu)
    , index_(index)

{
}


void FlMenuItem::enable(bool activate)
{
    const int flags = menu_->mode(index_);

    if (activate)
    {
        menu_->mode(index_, flags & ~FL_MENU_INACTIVE);
    }
    else
    {
        menu_->mode(index_, flags | FL_MENU_INACTIVE);
    }
}

