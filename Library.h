#pragma once
#include <time.h>

#include <chrono>
#include <format>
#include <iostream>
#include <list>
#include <print>
#include <string>
#include <vector>
class BorrowRcd {  // ���ļ�¼
                   // ÿ����¼Ϊһ�ν��飬ÿ��ֻ�ܽ�һ����
 public:
  BorrowRcd(int id) : rid(id) {
    deadline = std::chrono::system_clock::now() +
               std::chrono::days(30);  // ��������Ϊ30��
  }
  void show() const {
    std::time_t time = std::chrono::system_clock::to_time_t(deadline);
    std::print("ͼ��֤�ţ�{} ��Ӧ�黹ʱ�䣺{}", rid, time);  // ֱ�Ӵ�ӡʱ���
    if(isExpired()) std::print(" �ѹ���");
    std::cout << std::endl;
  }
  bool isExpired() const {  // �Ƿ����
    return deadline < std::chrono::system_clock::now();
  }
  int getRid() const { return rid; }
  auto getDL() const { return deadline; }

 private:
  int rid;  // ͼ��֤��
  std::chrono::system_clock::time_point
      deadline;  // Ӧ�黹ʱ��,���ݵ�ǰʱ��������
};
class Book {
 public:
  Book(int bid, int mount = 0, std::string title = "������",
       std::string author = "������");  // Ĭ��mountΪ0,��Ҫ��setMount���
  int getBid() const { return bid; }
  std::string getTitle() const { return title; }
  std::string getAuthor() const { return author; }
  int getMount() const { return mount; }

  void setBid(int bid) { this->bid = bid; }
  void setTitle(std::string title) { this->title = title; }
  void setAuthor(std::string author) { this->author = author; }
  void setMount(int mount) { this->mount = mount; }
  void show() const;

  bool operator==(const Book& book) const { return this->bid == book.bid; }
  bool operator!=(const Book& book) const { return this->bid != book.bid; }
  bool operator<(const Book& book) const { return this->bid < book.bid; }
  bool operator>(const Book& book) const { return this->bid > book.bid; }

  bool operator==(const int bid) const { return this->bid == bid; }
  bool operator!=(const int bid) const { return this->bid != bid; }
  bool operator<(const int bid) const { return this->bid < bid; }
  bool operator>(const int bid) const { return this->bid > bid; }

  int operator+=(const int number) { return mount += number; }

 private:
  int bid;             // ���
  std::string title;   // ����
  std::string author;  // ����
  int mount;           // �ִ���
  // ����¼�ܿ��������Ϊ�ܿ��������ͨ�����ļ�¼���ִ�������õ�
  // ����¼���������Ϊ���������ͨ�����ļ�¼����õ�
  std::list<BorrowRcd> rcd;  // ���ļ�¼

 public:
  bool borrowBook(int rid);
  bool returnBook(int rid);
};
struct BTreeNode {          // 2-3����3��B�����Ľڵ�
  std::vector<Book> books;  // ��
  std::vector<std::shared_ptr<BTreeNode>> children;  // �ӽڵ�
  std::weak_ptr<BTreeNode>
      parent;  // ���ڵ�,weak_ptr�����������ü�������ֹѭ������
  BTreeNode(Book b) : books(1, b), children(2, nullptr) {}
};
using INDEX_SIZE = std::vector<std::shared_ptr<BTreeNode>>::
    size_type;  // ���ĺ�����������,���������������,ʵ�ʾ���size_t
class result {  // ���ҽ��
  std::shared_ptr<BTreeNode>
      node;  // �ڵ㣬����ҵ���nodeΪ�����ڵĽڵ㣬����ΪӦ�ò����λ�ã����Ϊnullptr��˵����Ϊ��
  INDEX_SIZE index;  // ����,����ҵ���indexΪ�������������ΪӦ�ò����λ�ã�
  bool tag;          // �Ƿ��ҵ�
  result(std::shared_ptr<BTreeNode> n, INDEX_SIZE i, bool t)
      : node(n), index(i), tag(t) {}
  friend class Library;  // Library���Է���result�Ĺ��캯��
};
class Library {  // Singleton ����ģʽ

 public:
  static Library& getInstance() {  // ��ȡʵ��
    static Library instance;
    return instance;
  }
  void AddBook(const int bid, const int number = 10 , std::string title = "������",
       std::string author = "������");  // ������������ӿ��
  void AddBook(std::tuple<int, std::string, std::string>);  // ������������ӿ��
  bool DeleteBook(const int bid);                      // ��ո���
  bool Borrow(int bid, int rid = 1);                   // ����
  bool GiveBack(int bid, int rid = 1);                 // ����
  void Display();  // �԰�������ʽ��ʾ
  void showBook(int bid) const;
  
  //  TODO:�г�ĳ����ȫ��������,������Ҫ�ö�������¼
  // void ListbyAuthor(std::string);
 private:
  std::shared_ptr<BTreeNode> root;  // ���ڵ�

  Library() : root(nullptr) {}                  // ���캯��˽�л�
  Library(const Library&) = delete;             // ��ֹ��������
  Library& operator=(const Library&) = delete;  // ��ֹ��ֵ
  ~Library() {                                  // ��������
  }
  result find(int bid) const;
  void traverse(std::shared_ptr<BTreeNode> node,
                void (*visit)(std::shared_ptr<BTreeNode>, int),
                int depth = 0) const {
    visit(node, depth);
    for (auto& it : node->children) {
      if (it == nullptr) return;
      traverse(it, visit, depth + 1);
    }
  }
  // ���ҽ��������ͬ�����������Ϣ��ͬʱ����ʧ�ܣ�����false
  void insert(Book&& b, result& r);
  void DeleteBTree(std::shared_ptr<BTreeNode> node, const INDEX_SIZE& index);
  void Successor(std::shared_ptr<BTreeNode>& node, const INDEX_SIZE& index);
  void DisMore(std::shared_ptr<BTreeNode> ErrorNode);
  void DisLess(std::shared_ptr<BTreeNode> ErrorNode);
};
