#include "Library.h"

int main() {
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
    myLibrary.Display();
  }
  std::vector<int> bookIDs = {45, 90, 50, 22, 42};
  for (auto& it : bookIDs) {
    myLibrary.DeleteBook(it);
    myLibrary.Display();
  }
  std::cout << "图书管理系统采编入库、清除库存测试完毕。" << std::endl
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
    std::cout << "请输入操作的对应编号:";
    std::cin >> option;
    switch (option) {
      case 1: {
        std::cout << "请输入要入库的图书编号：";
        int bookID;
        std::cin >> bookID;
        myLibrary.AddBook(bookID);
        myLibrary.Display();
        break;
      }
      case 2: {
        std::cout << "请输入要清除库存的图书编号：";
        int bookID;
        std::cin >> bookID;
        myLibrary.DeleteBook(bookID);
        myLibrary.Display();
        break;
      }
      case 3: {
        std::cout << "请输入图书证号：";
        int cardID;
        std::cin >> cardID;
        std::cout << "请输入要借阅的图书编号：";
        int bookID;
        std::cin >> bookID;
        myLibrary.Borrow(bookID, cardID);
        break;
      }
      case 4: {
        std::cout << "请输入图书证号：";
        int cardID;
        std::cin >> cardID;
        std::cout << "请输入要归还的图书编号：";
        int bookID;
        std::cin >> bookID;
        myLibrary.GiveBack(bookID, cardID);
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
  return 0;
}