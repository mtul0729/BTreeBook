#pragma once
#include <fstream>
#include <iostream>
#include <streambuf>

class LogStream : public std::streambuf {
 public:
  LogStream(std::ostream& logStream, std::streambuf* originalStreamBuf)
      : logStream(logStream), originalStreamBuf(originalStreamBuf) {}
  std::streambuf* originalStreamBuf;  // 原始流的缓冲区指针
 protected:
  virtual int_type overflow(int_type c) override {
    if (c != EOF) {
      char ch = static_cast<char>(c);
      logStream.put(ch);             // 将字符输出到日志文件
      originalStreamBuf->sputc(ch);  // 将字符输出到原始流（std::cout）
    }
    return c;
  }

 private:
  std::ostream& logStream;  // 日志文件流
};
