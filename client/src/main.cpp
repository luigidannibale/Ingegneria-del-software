#include "main.h"
#include <wx/wx.h>

// int main() {
//     // MyApp a = new MyApp();    
//     // istanzia View
//     // istanzia Model(View)
//     // istanzia Controller(View, Model)
// }


class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        wxFrame *frame = new wxFrame(nullptr, wxID_ANY, "Hello wxWidgets");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);