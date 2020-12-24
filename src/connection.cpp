#include "maomix/connection.hpp"


using namespace maomix;


Connection::Connection(const lo::string_type &host, const lo::num_string_type &port)
{
  server = new lo::ServerThread(port);
  server->start();
  client = new lo::Address(host, port);
}


Connection::~Connection()
{
  if (client) { delete client; }
  if (server) { delete server; }
}
