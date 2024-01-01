// BTreeBook.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include <fstream>

#include "Library.h"
#include "LogStream.h"

int main() {
  std::ofstream logFile("log.txt");  // 创建日志文件

  // 创建自定义输出流并重定向到日志文件和 std::cout
  //LogStream logOStream(logFile, std::cout.rdbuf());
  //std::cout.rdbuf(&logOStream);
  //LogStream logIStream(logFile, std::cin.rdbuf());
  //std::cin.rdbuf(&logIStream);
  // 输出内容，会同时写入到日志文件和终端
  Library& myLibrary = Library::getInstance();
  using BookInfo = std::tuple<int, std::string, std::string>;
  std::vector<BookInfo> bookInfos = {
  	  BookInfo(35, "数据结构", "严蔚敏"),
  	  BookInfo(16, "简明大学物理学", "郭硕鸿"),
  	  BookInfo(18, "平凡的世界", "路遥"),
  	  BookInfo(70, "设计模式", "Erich Gamma"),
  	  BookInfo(5, "哈姆雷特", "威廉·莎士比亚"),
  	  BookInfo(50, "月亮与六便士", "毛姆"),
  	  BookInfo(22, "C语言程序设计", "谭浩强"),
  	  BookInfo(60, "市场营销学", "菲利普·科特勒"),
  	  BookInfo(13, "数字逻辑与EDA设计", "朱洪波"),
  	  BookInfo(17, "JAVA核心技术（卷一）", "Cay S. Horstmann"),
  	  BookInfo(12, "看见", "柴静"),
  	  BookInfo(45, "离散数学及其应用", "肖明宇"),
  	  BookInfo(25, "高等数学（上册）", "高等教育出版社"),
  	  BookInfo(42, "高等数学（下册）", "高等教育出版社"),
  	  BookInfo(15, "计算机科学概论", "Gregory V. Wilson"),
  	  BookInfo(90, "线性代数", "李永乐"),
  	  BookInfo(30, "电工学（上册）", "高等教育出版社"),
  	  BookInfo(7, "电工学（下册）", "高等教育出版社")};

  for (auto& it : bookInfos) {
    myLibrary.AddBook(it);
  }
  std::cout << "图书管理系统" << std::endl
            << std::endl
            << "1：采编入库" << std::endl
            << "2：清除库存" << std::endl
            << "3：借阅图书" << std::endl
            << "4：归还图书" << std::endl
            << "5：查询图书信息" << std::endl
            << "6：输出B树的状态" << std::endl
            << "0：退出" << std::endl;
  int option = 0;
  while (1) {
    std::cout<< "请输入操作的对应编号：(默认为0)";
    std::cin >> option;
    switch (option) {
      case 1: {
        std::cout << "请输入要入库的图书编号：";
        int bookID;
        std::cin >> bookID;
        myLibrary.AddBook(bookID);
        break;
      }
      case 2: {
        std::cout << "请输入要清除库存的图书编号：";
        int bookID;
        std::cin >> bookID;
        myLibrary.DeleteBook(bookID);
        break;
      }
      case 3: {
        std::cout << "请输入要借阅的图书编号：";
        int bookID;
        std::cin >> bookID;
        myLibrary.Borrow(bookID);
        break;
      }
      case 4: {
        std::cout << "请输入要归还的图书编号：";
        int bookID;
        std::cin >> bookID;
        myLibrary.GiveBack(bookID);
        break;
      }
      case 5: {
        std::cout << "请输入要查询的图书编号：";
        int bookID;
        std::cin >> bookID;
        myLibrary.showBook(bookID);
        break;
      }
      case 6: {
        myLibrary.Display();
        break;
      }
      case 0: {
        std::cout << "退出系统" << std::endl;
        return 0;
        break;
      }
      default: {
        std::cout << "输入错误，";
        break;
      }
    }
  };

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
  //std::cout.rdbuf(logOStream.originalStreamBuf);
  //std::cin.rdbuf(logIStream.originalStreamBuf);

  return 0;
}