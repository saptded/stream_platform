#include "streamer.hpp"
#include "client.hpp"

#include <QApplication>

int main(int argc, char ** argv) {
//    QApplication app{argc, argv};

    sp::Client client;

    client.get_link();

    client.connect_to_server();

    client.start_watch();

    return 0;

//    return QApplication::exec();
}
