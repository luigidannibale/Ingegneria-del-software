#include "ViewGamePanel.hpp"

ViewGamePanel::ViewGamePanel(wxWindow *parent) : wxPanel(parent->GetParent())
{
    SetSize(parent->GetSize());
    SetBackgroundColour(wxColour(118, 150, 86));

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    // Create a static text widget
    wxStaticText *text = new wxStaticText(this, wxID_ANY, "Select a match", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    text->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    text->SetMinSize(text->GetTextExtent(text->GetLabel()));

    // Create a table using wxGrid
    table = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    int num_columns = 5;
    table->CreateGrid(0, num_columns); // 5 rows and 3 columns
    table->SetColLabelValue(0, "ID");
    table->SetColLabelValue(1, "White");
    table->SetColLabelValue(2, "Black");
    table->SetColLabelValue(3, "Esito");
    table->SetColLabelValue(4, "motivo");

    int height = this->GetSize().GetHeight() * 3 / 4;
    int width = this->GetSize().GetWidth() * 9 / 10;
    wxSize size = wxSize(width, height);
    table->SetMaxSize(size);
    table->EnableEditing(false);
    int columns_width = (width - table->GetRowLabelSize()) / num_columns;
    for (int i = 0; i < num_columns; i++)
    {
        table->SetColSize(i, columns_width);
    }
    SetTableHeight();

    backButton = new wxButton(this, wxID_ANY, "Back", wxDefaultPosition, wxDefaultSize, 0);

    // Add text and table to the sizer
    vbox->Add(text, wxSizerFlags(0).Align(wxALIGN_CENTER).Border(wxTOP | wxBOTTOM, 10));
    vbox->Add(table, wxSizerFlags(1).Expand().Center().Border(wxALL, 5));
    vbox->Add(backButton, wxSizerFlags(0).Align(wxALIGN_CENTER).Border(wxTOP | wxBOTTOM, 10));

    // Set the sizer for the panel
    SetSizer(vbox);
    Layout();
}

ViewGamePanel::~ViewGamePanel()
{
}

void ViewGamePanel::SetTableHeight()
{
    int tot = 0;
    for (int i = 0; i < table->GetNumberRows(); i++)
    {
        tot += table->GetRowSize(i);
    }
    tot += table->GetColLabelSize();
    tot = tot > table->GetMaxSize().GetHeight() ? table->GetMaxSize().GetHeight() : tot;
    table->SetSize(table->GetSize().GetWidth(), tot);
}

wxGrid *ViewGamePanel::GetTable()
{
    return table;
}

wxButton *ViewGamePanel::GetBackButton()
{
    return backButton;
}