//
// -*- tab-width: 4; indent-tabs-mode: nil;  -*-
// vim: tabstop=4:softtabstop=4:expandtab:shiftwidth=4
//
// $Id$
//
#include "fldialog.h"
#include "controller.h"
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl.H>

#include <iostream>
using namespace std;


/**
 * Allows the dialog to do custom event handling.
 */
class DialogWindow : public Fl_Double_Window
{
    FlDialog* dlg_;

    int handle(int e) {
        int result = dlg_->handle(e);
        if (result == 0) {
            result = Fl_Double_Window::handle(e);
        }
        return result;
    }

public:
    DialogWindow(FlDialog* dlg, int x, int y, int w, int h, const char* title)
        : Fl_Double_Window(x, y, w, h, title)
        , dlg_(dlg)
    { }
};


FlDialog::FlDialog(

    ui::Controller& controller,
    int             x,
    int             y,
    int             w,
    int             h,
    const char* title /* = nullptr */
    )

    : ui::Dialog(controller)
    , window_(new DialogWindow(this, x, y, w, h, title))
    , group_(new Fl_Group(0, 0, w, h))
{
    window_->callback(close_callback, static_cast<ui::Dialog*>(this));
}


FlDialog::~FlDialog()
{
}


void FlDialog::center()
{
    int x = (controller().w() - window_->w()) / 2;
    int y = (controller().h() - window_->h()) / 2;

    window_->position(controller().x() + x, controller().y() + y);
}


void FlDialog::add_button(
    int         x,
    int         y,
    int         w,
    int         h,
    const char* label,
    Fl_Callback cb )

{
    auto b = new Fl_Button(x, y, w, h, label);
    b->callback(cb, this);
}


void FlDialog::action_callback(Fl_Widget* w, void* data)
{
    reinterpret_cast<FlDialog*>(data)->exec_action( w->label() );
}


void FlDialog::close_callback(Fl_Widget* w, void* data)
{
    reinterpret_cast<ui::Dialog*>(data)->close();
}


void FlDialog::close_impl()
{
}


void FlDialog::set_resizable(
    int minWidth,
    int minHeight)
{
    window_->resizable(*window_);
    window_->size_range(minWidth, minHeight);
}


void FlDialog::show(bool doShow)
{
    if (doShow)
    {
        window_->set_modal();
        window_->show();
        assert(Fl::modal() == window_.get());
    }
    else
    {
        window_->hide();
        assert(Fl::modal() != window_.get());
    }
}


bool FlDialog::enable_button(
    const char* label,
    bool        enable )
{
    const int count = group_->children();
    for (int i = 0; i != count; ++i)
    {
        auto w = group_->child(i);
        if (!w || !w->label())
        {
            continue;
        }
        if (strcmp(w->label(), label) == 0)
        {
            if (enable)
            {
                w->activate();
            }
            else
            {
                w->deactivate();
            }
            return true;
        }
    }
    return false;
}


