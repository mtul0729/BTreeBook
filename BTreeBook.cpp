// BTreeBook.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//

#include <fstream>

#include "Library.h"
#include "LogStream.h"

int main() {
  std::ofstream logFile("log.txt");  // ������־�ļ�

  // �����Զ�����������ض�����־�ļ��� std::cout
  LogStream logStream(logFile, std::cout.rdbuf());
  std::cout.rdbuf(&logStream);

  // ������ݣ���ͬʱд�뵽��־�ļ����ն�

  std::cout << "ͼ�����ϵͳ" << std::endl << std::endl;
  int option = 0;
  do {
    std::cout << "1���ɱ����" << std::endl
              << "2��������" << std::endl
              << "3������ͼ��" << std::endl
              << "4���黹ͼ��" << std::endl
              << "5����ѯͼ����Ϣ" << std::endl
              << "6�����B����״̬" << std::endl
        <<"0���˳�"<< std::endl
        <<"����������Ķ�Ӧ��ţ�(Ĭ��Ϊ0)";
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
  std::cout.rdbuf(logStream.originalStreamBuf);

  return 0;
}