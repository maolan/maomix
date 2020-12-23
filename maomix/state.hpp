#pragma once
#include <memory>
#include <vector>
#include <lo/lo_cpp.h>


class ImVec4;


namespace maomix
{
  class State
  {
    public:
      ~State();
      static State * get();

      void init(const std::string &host, const unsigned &port);

      ImVec4 *clear_color;
      lo::Address *address;
      char ip[16];
      int port;
      std::vector<float> busses;
      std::vector<float> channels;
      std::vector<float> fx;
      std::vector<float> gain;
      std::vector<float> ret;
      std::vector<int> mute;
      std::vector<int> solo;
      std::vector<int> invert;
      std::vector<int> phantom;
      std::vector<int> busmute;
      std::vector<int> bussolo;
      std::vector<int> fxmute;
      std::vector<int> fxsolo;
      std::vector<int> retmute;
      std::vector<int> retsolo;
      float output;
      int muted;
      float line;
      int linemuted;
      int linesolo;

    protected:
      State();
      static State * state;
  };
}
