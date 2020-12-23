#pragma once


namespace maomix
{
  class UI
  {
    public:
      virtual ~UI();

      virtual void prepare() = 0;
      virtual void render() = 0;
      virtual void run() = 0;
  };
}
