#include <iostream>
#include <glog/logging.h>
#include "negascan.h"

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    Negascan app(1280, 720, "app");

    std::string test_image_path = "/Users/mgharbi/Pictures/film_tests/robi_guadalupe.CR2";

    app.load_image(test_image_path);

    app.run();
    return 0;
}
