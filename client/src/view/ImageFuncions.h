#ifndef IMAGEFUNCIONS_H
#define IMAGEFUNCIONS_H

#include <wx-3.0/wx/wx.h>

namespace img{

    wxImage GetImage(std::string);
    wxImage GetImageAndScale(std::string, float);
}

#endif //IMAGEFUNCIONS_H