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
    methods();
    meters();
    refresh();
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
  for (auto &channel : state->channels)
  {
    channel.send.resize(busnumber);
    channel.fx.resize(fxnumber);
  }
}


void Connection::methods()
{
  channelMethods();
  busMethods();
  fxMethods();
  retMethods();
}


void Connection::channelMethods()
{
  auto state = State::get();
  int id = 1;
  for (auto &channel : state->channels)
  {
    std::stringstream s;
    s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/fader";
    server.add_method(s.str(), "f", [&channel](lo_arg **argv, int argc) {
      channel.fader = argv[0]->f;
    });
    client->send_from(server, s.str(), "");
    s.str("");

    s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/on";
    server.add_method(s.str(), "i", [&channel](lo_arg **argv, int argc) {
      channel.mute = argv[0]->i;
    });
    client->send_from(server, s.str(), "");
    s.str("");

    s << "/headamp/" << std::setw(2) << std::setfill('0') << id << "/gain";
    server.add_method(s.str(), "f", [&channel, &id](lo_arg **argv, int argc) {
      channel.gain = argv[0]->f;
    });
    client->send_from(server, s.str(), "");
    s.str("");

    s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/preamp/invert";
    server.add_method(s.str(), "i", [&channel](lo_arg **argv, int argc) {
      channel.invert = argv[0]->i;
    });
    client->send_from(server, s.str(), "");
    s.str("");

    s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/gate/on";
    server.add_method(s.str(), "i", [&channel](lo_arg **argv, int argc) {
      channel.gate.on = argv[0]->i;
    });
    client->send_from(server, s.str(), "");
    s.str("");

    s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/gate/thr";
    server.add_method(s.str(), "f", [&channel, &id](lo_arg **argv, int argc) {
      channel.gate.fader = argv[0]->f;
    });
    client->send_from(server, s.str(), "");
    s.str("");

    s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/dyn/on";
    server.add_method(s.str(), "i", [&channel](lo_arg **argv, int argc) {
      channel.dyn.on = argv[0]->i;
    });
    client->send_from(server, s.str(), "");
    s.str("");

    s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/dyn/thr";
    client->send_from(server, s.str(), "");
    server.add_method(s.str(), "f", [&channel, &id](lo_arg **argv, int argc) {
      channel.dyn.fader = argv[0]->f;
    });
    s.str("");

    int busnumber = 1;
    for (auto &bus : channel.send)
    {
      s.str("");
      s << "/ch/" << std::setw(2) << std::setfill('0') << id;
      s << "/mix/" << std::setw(2) << std::setfill('0') << busnumber << "/level";
      server.add_method(s.str(), "f", [&bus](lo_arg **argv, int argc) {
        bus = argv[0]->f;
      });
      client->send_from(server, s.str(), "");
      ++busnumber;
    }

    int fxnumber = 1;
    for (auto &fx : channel.fx)
    {
      s.str("");
      s << "/ch/" << std::setw(2) << std::setfill('0') << id;
      s << "/mix/" << std::setw(2) << std::setfill('0') << fxnumber + busnumber - 1 << "/level";
      server.add_method(s.str(), "f", [&fx](lo_arg **argv, int argc) {
        fx = argv[0]->f;
      });
      client->send_from(server, s.str(), "");
      ++fxnumber;
    }

    ++id;
  }
}


void Connection::busMethods()
{
  auto state = State::get();
  int id = 1;
  for (auto &bus: state->busses)
  {
    std::stringstream s;
    s << "/bus/" << id << "/mix/fader";
    server.add_method(s.str(), "f", [&bus](lo_arg **argv, int argc) {
      bus.fader = argv[0]->f;
    });
    client->send_from(server, s.str(), "");

    s.str("");
    s << "/bus/" << id << "/mix/on";
    server.add_method(s.str(), "i", [&bus](lo_arg **argv, int argc) {
      bus.mute = argv[0]->i;
    });
    client->send_from(server, s.str(), "");

    ++id;
  }
}


void Connection::fxMethods()
{
  auto state = State::get();
  int id = 1;
  for (auto &fx: state->fx)
  {
    std::stringstream s;
    s << "/fxsend/" << id << "/mix/fader";
    server.add_method(s.str(), "f", [&fx](lo_arg **argv, int argc) {
      fx.fader = argv[0]->f;
    });
    client->send_from(server, s.str(), "");

    s.str("");
    s << "/fxsend/" << id << "/mix/on";
    server.add_method(s.str(), "i", [&fx](lo_arg **argv, int argc) {
      fx.mute = argv[0]->i;
    });
    client->send_from(server, s.str(), "");

    ++id;
  }
}


void Connection::retMethods()
{
  auto state = State::get();
  int id = 1;
  for (auto &ret: state->ret)
  {
    std::stringstream s;
    s << "/rtn/" << id << "/mix/fader";
    server.add_method(s.str(), "f", [&ret](lo_arg **argv, int argc) {
      ret.fader = argv[0]->f;
    });
    client->send_from(server, s.str(), "");

    s.str("");
    s << "/rtn/" << id << "/mix/on";
    server.add_method(s.str(), "i", [&ret](lo_arg **argv, int argc) {
      ret.mute = argv[0]->i;
    });
    client->send_from(server, s.str(), "");

    ++id;
  }
}


void Connection::refresh()
{
  client->send_from(server, "/xremotenfb", "");
}


void Connection::meters()
{
  client->send_from(server, "/meters", "s", "/meters/1");
}


Connection::~Connection() { if (client) { delete client; } }
