#include "Library.h"

int main() {
  Library& myLibrary = Library::getInstance();
  using BookInfo = std::tuple<int, std::string, std::string>;
  std::vector<BookInfo> bookInfos = {
      BookInfo(35, "���ݽṹ", "��ε��"),
      BookInfo(16, "������ѧ����ѧ", "��˶��"),
      BookInfo(18, "ƽ��������", "·ң"),
      BookInfo(70, "���ģʽ", "Erich Gamma"),
      BookInfo(5, "��ķ����", "������ɯʿ����"),
      BookInfo(50, "����������ʿ", "ëķ"),
      BookInfo(22, "C���Գ������", "̷��ǿ"),
      BookInfo(60, "�г�Ӫ��ѧ", "�����ա�������"),
      BookInfo(13, "�����߼���EDA���", "��鲨"),
      BookInfo(17, "JAVA���ļ�������һ��", "Cay S. Horstmann"),
      BookInfo(12, "����", "��"),
      BookInfo(45, "��ɢ��ѧ����Ӧ��", "Ф����"),
      BookInfo(25, "�ߵ���ѧ���ϲᣩ", "�ߵȽ���������"),
      BookInfo(42, "�ߵ���ѧ���²ᣩ", "�ߵȽ���������"),
      BookInfo(15, "�������ѧ����", "Gregory V. Wilson"),
      BookInfo(90, "���Դ���", "������"),
      BookInfo(30, "�繤ѧ���ϲᣩ", "�ߵȽ���������"),
      BookInfo(7, "�繤ѧ���²ᣩ", "�ߵȽ���������")};

  for (auto& it : bookInfos) {
    myLibrary.AddBook(it);
    myLibrary.Display();
  }
  std::vector<int> bookIDs = {45, 90, 50, 22, 42};
  for (auto& it : bookIDs) {
    myLibrary.DeleteBook(it);
    myLibrary.Display();
  }
  std::cout << "ͼ�����ϵͳ�ɱ���⡢�����������ϡ�" << std::endl
            << std::endl
            << "1���ɱ����" << std::endl
            << "2��������" << std::endl
            << "3������ͼ��" << std::endl
            << "4���黹ͼ��" << std::endl
            << "5����ѯͼ����Ϣ" << std::endl
            << "6�����B����״̬" << std::endl
            << "0���˳�" << std::endl;
  int option = 0;
  while (1) {
    std::cout << "����������Ķ�Ӧ���:";
    std::cin >> option;
    switch (option) {
      case 1: {
        std::cout << "������Ҫ����ͼ���ţ�";
        int bookID;
        std::cin >> bookID;
        myLibrary.AddBook(bookID);
        myLibrary.Display();
        break;
      }
      case 2: {
        std::cout << "������Ҫ�������ͼ���ţ�";
        int bookID;
        std::cin >> bookID;
        myLibrary.DeleteBook(bookID);
        myLibrary.Display();
        break;
      }
      case 3: {
        std::cout << "������ͼ��֤�ţ�";
        int cardID;
        std::cin >> cardID;
        std::cout << "������Ҫ���ĵ�ͼ���ţ�";
        int bookID;
        std::cin >> bookID;
        myLibrary.Borrow(bookID, cardID);
        break;
      }
      case 4: {
        std::cout << "������ͼ��֤�ţ�";
        int cardID;
        std::cin >> cardID;
        std::cout << "������Ҫ�黹��ͼ���ţ�";
        int bookID;
        std::cin >> bookID;
        myLibrary.GiveBack(bookID, cardID);
        break;
      }
      case 5: {
        std::cout << "������Ҫ��ѯ��ͼ���ţ�";
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
        std::cout << "�˳�ϵͳ" << std::endl;
        return 0;
        break;
      }
      default: {
        std::cout << "�������";
        break;
      }
    }
  };
  return 0;
}