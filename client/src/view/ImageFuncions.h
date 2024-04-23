#ifndef IMAGEFUNCIONS_H
#define IMAGEFUNCIONS_H

#include "../lib.h"

namespace img{
    wxImage GetImage(std::string);
    wxImage GetImageAndScale(std::string, float);
}

#endif //IMAGEFUNCIONS_H