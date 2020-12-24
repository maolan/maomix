#pragma once
#include <string>
#include <lo/lo_cpp.h>


namespace maomix
{
  class Connection
  {
    public:
      Connection(const lo::string_type &host, const lo::num_string_type &port);
      ~Connection();

      lo::Address *client;
      lo::ServerThread *server;
  };
}
