// BTreeBook.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include <fstream>

#include "Library.h"
#include "LogStream.h"

int main() {
  std::ofstream logFile("log.txt");  // 创建日志文件

  // 创建自定义输出流并重定向到日志文件和 std::cout
  LogStream logStream(logFile, std::cout.rdbuf());
  std::cout.rdbuf(&logStream);

  // 输出内容，会同时写入到日志文件和终端
  std::cout << "Hello, Log!" << std::endl;

  Library& myLibrary = Library::getInstance();
  std::vector<int> bookIDs = {35, 16, 18, 70, 5,  50, 22, 60, 13,
                              17, 12, 45, 25, 42, 15, 90, 30, 7};
  for(auto it:bookIDs){
      myLibrary.AddBook(it);
      myLibrary.Display();
  }

  // 恢复原始的 std::cout 流
  std::cout.rdbuf(logStream.originalStreamBuf);

  return 0;
}