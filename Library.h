#pragma once
#include <time.h>

#include <chrono>
#include <iostream>
#include <list>
#include <string>
#include <vector>
class BorrowRcd {  // 借阅记录
                   // 每个记录为一次借书，每次只能借一本书
 public:
  BorrowRcd(int id) : rid(id) {
    deadline = std::chrono::system_clock::now() +
               std::chrono::days(30);  // 借书期限为30天
  }
  int getRid() const { return rid; }
  auto getDL() const { return deadline; }
  void show() const {
    std::time_t time = std::chrono::system_clock::to_time_t(deadline);
    std::cout << "图书证号：" << rid << " ，应归还时间：" << time - 1706747083;//
    if (isExpired()) std::cout << " 已过期";  // 如果过期，打印已过期
    std::cout << std::endl;
  }

 private:
  int rid;  // 图书证号
  std::chrono::system_clock::time_point
      deadline;  // 应归还时间,根据当前时间计算出来

  bool isExpired() const {  // 是否过期
    return deadline < std::chrono::system_clock::now();
  }
};
class Book {
 public:
  Book(int bid, int mount, std::string title, std::string author);
  int getBid() const { return bid; }
  void show() const;
  bool operator==(int bid) const { return this->bid == bid; }
  bool operator>(const int bid) const { return this->bid > bid; }
  int operator+=(const int number) { return mount += number; }
  bool borrowBook(int rid);
  bool returnBook(int rid);

 private:
  int bid;             // 书号
  std::string title;   // 书名
  std::string author;  // 作者
  int mount;           // 现存量
  // 不记录总库存量，因为总库存量可以通过借阅记录和现存量计算得到
  std::list<BorrowRcd> rcd;  // 借阅记录
};
struct BTreeNode {          // 2-3树（3阶B树）的节点
  std::vector<Book> books;  // 书
  std::vector<std::shared_ptr<BTreeNode>> children;  // 子节点
  std::weak_ptr<BTreeNode>
      parent;  // 父节点,weak_ptr不会增加引用计数，防止循环引用
  BTreeNode(Book b) : books(1, b), children(2, nullptr) {}
};
using INDEX_SIZE = std::vector<std::shared_ptr<BTreeNode>>::
    size_type;  // 结点的孩子数量类型,兼容书的数量类型,实际就是size_t
class result {                      // 查找结果
  std::shared_ptr<BTreeNode> node;  // 节点，如果找到，node为书所在的节点，
  // 否则为应该插入的位置；如果为nullptr，说明树为空
  INDEX_SIZE index;  // 索引,如果找到，index为书的索引，否则为应该插入的位置；
  bool tag;          // 是否找到
  result(std::shared_ptr<BTreeNode> n, INDEX_SIZE i, bool t)
      : node(n), index(i), tag(t) {}
  friend class Library;  // Library可以访问result的构造函数
};
class Library {  // Singleton 单例模式,只允许构造一个实例
 public:
  static Library& getInstance() {  // 获取实例
    static Library instance;
    return instance;
  }
  void AddBook(const int bid, const int number = 10,
               std::string title = "待补充",
               std::string author = "待补充");  // 加入新书或增加库存
  void AddBook(std::tuple<int, std::string, std::string>);
  bool DeleteBook(const int bid);       // 清空该书
  void Borrow(int bid, int rid = 1);    // 借书
  bool GiveBack(int bid, int rid = 1);  // 还书
  void Display();                       // 以凹入表的形式显示
  void showBook(int bid) const;

 private:
  std::shared_ptr<BTreeNode> root;              // 根节点
  Library() : root(nullptr) {}                  // 构造函数私有化
  Library(const Library&) = delete;             // 禁止拷贝构造
  Library& operator=(const Library&) = delete;  // 禁止赋值
  ~Library() {}
  result find(int bid) const;
  void traverse(std::shared_ptr<BTreeNode> node, //记录深度的遍历
                void (*visit)(std::shared_ptr<BTreeNode>, int),
                int depth = 0) const {
    visit(node, depth);
    for (auto& it : node->children) {
      if (it == nullptr) return;
      traverse(it, visit, depth + 1);
    }
  }
  // 当且仅当书号相同但书的其他信息不同时插入失败，返回false
  void insert(Book&& b, result& r);  //
  void DeleteBTree(std::shared_ptr<BTreeNode> node, const INDEX_SIZE& index);
  void Successor(std::shared_ptr<BTreeNode>& node, const INDEX_SIZE& index);
  void DisMore(std::shared_ptr<BTreeNode> ErrorNode);
  void DisLess(std::shared_ptr<BTreeNode> ErrorNode);
};
