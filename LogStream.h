#pragma once
#include <fstream>
#include <iostream>
#include <streambuf>

class LogStream : public std::streambuf {
 public:
  LogStream(std::ostream& logStream, std::streambuf* originalStreamBuf)
      : logStream(logStream), originalStreamBuf(originalStreamBuf) {}
  std::streambuf* originalStreamBuf;  // ԭʼ���Ļ�����ָ��
 protected:
  virtual int_type overflow(int_type c) override {
    if (c != EOF) {
      char ch = static_cast<char>(c);
      logStream.put(ch);             // ���ַ��������־�ļ�
      originalStreamBuf->sputc(ch);  // ���ַ������ԭʼ����std::cout��
    }
    return c;
  }

 private:
  std::ostream& logStream;  // ��־�ļ���
};
