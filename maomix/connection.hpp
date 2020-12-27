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

      void info(const char *path, const lo::Message &msg);
      void meters();
      void refresh();

      lo::ServerThread server;
      lo::Address *client;
  };
}
