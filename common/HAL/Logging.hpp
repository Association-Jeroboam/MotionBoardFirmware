#pragma once

class Logging {
  public:
    static void init();

    static void print(const char* fmt, ...);

    static void println(const char* fmt, ...);
};
