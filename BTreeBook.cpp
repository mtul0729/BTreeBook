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
  std::cout << "Hello, Log!" << std::endl;

  Library& myLibrary = Library::getInstance();
  std::vector<int> bookIDs = {35, 16, 18, 70, 5,  50, 22, 60, 13,
                              17, 12, 45, 25, 42, 15, 90, 30, 7};
  for(auto it:bookIDs){
      myLibrary.AddBook(it);
      myLibrary.Display();
  }

  // �ָ�ԭʼ�� std::cout ��
  std::cout.rdbuf(logStream.originalStreamBuf);

  return 0;
}