//
// Created by luigi on 04/03/24.
//

#include "ImageFuncions.h"

wxImage img::GetImage(std::string path){
    wxImage image(path, wxBITMAP_TYPE_PNG);
    if (image.IsOk()){
        return image;
    }
}

wxImage img::GetImageAndScale(std::string path, float scalePercentage){
    wxImage image = GetImage(path);
    image = image.Scale(image.GetWidth()*scalePercentage, image.GetHeight()*scalePercentage);
    if (image.IsOk()) {
        return image;
    }
}