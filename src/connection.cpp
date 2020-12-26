#include <iostream>
#include <thread>
#include "imgui.h"
#include "maomix/connection.hpp"
#include "maomix/state.hpp"


using namespace maomix;


Connection::Connection(const lo::string_type &host, const lo::num_string_type &port)
  : server{nullptr}
{
  if (!server.is_valid()) {
    std::cerr << "Could not create a valid server thread." << std::endl;
  }
  std::cout << "Listening on " << server.url() << std::endl;
  server.start();

  server.add_method("/info", "ssss", [](const char* path, const lo::Message &msg) {
    const std::string model = &msg.argv()[2]->s;
    auto state = State::get();
    int ch;
    int busnumber;
    int fxnumber;
    int retnumber;
    if (model == "XR18")
    {
      ch = 16;
      busnumber = 6;
      fxnumber = 4;
      retnumber = 4;
    }
    state->busses.resize(busnumber);
    state->fx.resize(fxnumber);
    state->ret.resize(retnumber);
    state->channels.resize(ch);
    for (auto &channel : state->channels)
    {
      channel.send.resize(busnumber);
      channel.fx.resize(fxnumber);
    }
  });

  client = new lo::Address(host, port);
  client->send_from(server, "/info", "N", nullptr);
}


Connection::~Connection()
{
  if (client) { delete client; }
}
