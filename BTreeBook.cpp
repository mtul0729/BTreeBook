// BTreeBook.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include <fstream>

#include "Library.h"
#include "LogStream.h"

int main() {
  std::ofstream logFile("log.txt");  // ������־�ļ�

  // �����Զ�����������ض�����־�ļ��� std::cout
  //LogStream logOStream(logFile, std::cout.rdbuf());
  //std::cout.rdbuf(&logOStream);
  //LogStream logIStream(logFile, std::cin.rdbuf());
  //std::cin.rdbuf(&logIStream);
  // ������ݣ���ͬʱд�뵽��־�ļ����ն�
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
  }
  std::cout << "ͼ�����ϵͳ" << std::endl
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
    std::cout<< "����������Ķ�Ӧ��ţ�(Ĭ��Ϊ0)";
    std::cin >> option;
    switch (option) {
      case 1: {
        std::cout << "������Ҫ����ͼ���ţ�";
        int bookID;
        std::cin >> bookID;
        myLibrary.AddBook(bookID);
        break;
      }
      case 2: {
        std::cout << "������Ҫ�������ͼ���ţ�";
        int bookID;
        std::cin >> bookID;
        myLibrary.DeleteBook(bookID);
        break;
      }
      case 3: {
        std::cout << "������Ҫ���ĵ�ͼ���ţ�";
        int bookID;
        std::cin >> bookID;
        myLibrary.Borrow(bookID);
        break;
      }
      case 4: {
        std::cout << "������Ҫ�黹��ͼ���ţ�";
        int bookID;
        std::cin >> bookID;
        myLibrary.GiveBack(bookID);
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

  myLibrary.Borrow(16);
  myLibrary.Borrow(23);  // ������
  myLibrary.Borrow(18);
  for (int i = 0; i < 10; i++) myLibrary.Borrow(16, i);
  myLibrary.GiveBack(16, 2);
  myLibrary.GiveBack(16, 2);

  /*������
  //myLibrary.Display();
  bookIDs = {45, 90, 50, 22, 42};
  for(auto it:bookIDs){
      myLibrary.DeleteBook(it);
      myLibrary.Display();
  }*/

  // �ָ�ԭʼ�� std::cout ��
  //std::cout.rdbuf(logOStream.originalStreamBuf);
  //std::cin.rdbuf(logIStream.originalStreamBuf);

  return 0;
}