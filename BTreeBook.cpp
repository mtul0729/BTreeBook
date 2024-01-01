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

  std::cout << "图书管理系统" << std::endl << std::endl;
  int option = 0;
  do {
    std::cout << "1：采编入库" << std::endl
              << "2：清除库存" << std::endl
              << "3：借阅图书" << std::endl
              << "4：归还图书" << std::endl
              << "5：查询图书信息" << std::endl
              << "6：输出B树的状态" << std::endl
        <<"0：退出"<< std::endl
        <<"请输入操作的对应编号：(默认为0)";
    std::cout << "" << std::endl;
  }while(option<0||option>6);

  Library& myLibrary = Library::getInstance();
  std::vector<int> bookIDs = {35, 16, 18, 70, 5,  50, 22, 60, 13,
                              17, 12, 45, 25, 42, 15, 90, 30, 7};
  for (auto it : bookIDs) {
    myLibrary.AddBook(it);
    // myLibrary.Display();
  }

  myLibrary.Borrow(16);
  myLibrary.Borrow(23);  // 不存在
  myLibrary.Borrow(18);
  for (int i = 0; i < 10; i++) myLibrary.Borrow(16, i);
  myLibrary.GiveBack(16, 2);
  myLibrary.GiveBack(16, 2);

  /*清除库存
  //myLibrary.Display();
  bookIDs = {45, 90, 50, 22, 42};
  for(auto it:bookIDs){
      myLibrary.DeleteBook(it);
      myLibrary.Display();
  }*/

  // 恢复原始的 std::cout 流
  std::cout.rdbuf(logStream.originalStreamBuf);

  return 0;
}