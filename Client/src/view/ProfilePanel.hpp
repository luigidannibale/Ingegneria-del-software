#ifndef PROFILEPANEL_H
#define  PROFILEPANEL_H

#include "../lib.hpp"
#include "../model/User.hpp"

class ProfilePanel : public wxPanel{
public:
    ProfilePanel(wxWindow*, User*);
    wxButton* GetBtnBack();
    wxButton* GetBtnCancel();
    wxButton* GetBtnUpdate();
    wxButton* GetBtnSave();
    wxTextCtrl* GetTxtUsername();
    wxTextCtrl* GetTxtName();
    wxTextCtrl* GetTxtSurname();
    void UpdateUserDatas(User*);
    void ShowUpdate(wxCommandEvent&);
    void ShowDefault(wxCommandEvent&);

private:
    wxPanel* defaultPanel;
    wxStaticText* lblUsername;
    wxStaticText* lblName;
    wxStaticText* lblSurname;
    wxButton* btnUpdateMode;
    wxStaticText* lblEloPointsD;
    wxButton* btnBack;

    wxPanel* updatePanel;
    wxTextCtrl* txtUsername;
    wxTextCtrl* txtName;
    wxTextCtrl* txtSurname;
    wxButton* btnSave;
    wxStaticText* lblEloPointsU;
    wxButton* btnCancel;

    //passwd wxTextCtrl* txtPassword;

};

#endif //PROFILEPANEL_H
