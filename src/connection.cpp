#include <iostream>
#include <iomanip>
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

  server.add_method("/info", "ssss", [this](const char* path, const lo::Message &msg) {
    info(path, msg);
    meters();
    refresh();
  });

  server.add_method("/meters/1", "b", [](lo_arg **argv, int argc) {
    int size = argv[0]->blob.size;
    int *data = (int *)&(argv[0]->blob.data);
    int count = data[0];
    std::cout << size << ' ' << count << std::endl;
  });

  server.add_method("/meters/6", "b", [](lo_arg **argv, int argc) {
    int size = argv[0]->blob.size;
    int *data = (int *)&(argv[0]->blob.data);
    int count = data[0];
    std::cout << size << ' ' << count << std::endl;
  });

  client = new lo::Address(host, port);
  client->send_from(server, "/info", "N", nullptr);
}


void Connection::info(const char *path, const lo::Message &msg)
{
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
  int id = 1;
  for (auto &channel : state->channels)
  {
    channel.send.resize(busnumber);
    channel.fx.resize(fxnumber);
    std::stringstream s;
    s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/fader";
    server.add_method(s.str(), "f", [&channel](lo_arg **argv, int argc) {
      channel.fader = argv[0]->f;
    });
    client->send_from(server, s.str(), "");
    ++id;
  }
}


void Connection::meters()
{
  // client->send_from(server, "/meters", "s", "/meters/1");
  // client->send_from(server, "/meters", "si", "/meters/6", 1);
}


void Connection::refresh()
{
  client->send_from(server, "/xremotenfb", "");
}


Connection::~Connection() { if (client) { delete client; } }
