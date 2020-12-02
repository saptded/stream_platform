#include "streamer.hpp"
#include "client.hpp"

int main() {
    sp::Client client;

    client.connect_to_server();

    client.start_watching();


}
