#ifndef IMAGEFUNCIONS_H
#define IMAGEFUNCIONS_H

#include "../lib.hpp"

namespace img{
    wxImage GetImage(std::string);
    wxImage GetImageAndScale(std::string, float);
    std::string const IMGPATH = "../src/resources/img/";
}

#endif //IMAGEFUNCIONS_H