#include <iostream>
#include <glog/logging.h>
#include <thread>
#include "negascan.h"

// class AppRunnable
// {
// public:
//     void operator() {
//         std::cout << "in the thread\n";
//         // app.run();
//     };
// };


int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    Negascan app(1920, 1080, "app");
    app.run();
    return 0;
}
