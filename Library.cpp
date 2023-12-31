#include "Library.h"

class BorrowRcd {  // 借阅记录
                   // 每个记录为一次借书，每次只能借一本书
 public:
  BorrowRcd(int id) : rid(id) {
    deadline = std::chrono::system_clock::now() +
               std::chrono::days(30);  // 借书期限为30天
  }
  void show() const {
    std::time_t time = std::chrono::system_clock::to_time_t(deadline);
    std::println("图书证号：{} ，应归还时间：{}", rid, time);  // 直接打印时间戳
  }
  bool isExpired() const {  // 是否过期
    return deadline < std::chrono::system_clock::now();
  }
  int getRid() const { return rid; }

 private:
  int rid;  // 图书证号
  std::chrono::system_clock::time_point
      deadline;  // 应归还时间,根据当前时间计算出来
};

class Book {
 public:
  Book() = default;
  Book(int bid, std::string title, std::string author,
       int mount = 0)  // 默认mount为0,需要用setMount添加
      : bid(bid), title(title), author(author), mount(mount) {}

  int getBid() const { return bid; }

  std::string getTitle() const { return title; }
  std::string getAuthor() const { return author; }
  int getMount() const { return mount; }

  void setBid(int bid) { this->bid = bid; }
  void setTitle(std::string title) { this->title = title; }
  void setAuthor(std::string author) { this->author = author; }
  void setMount(int mount) { this->mount = mount; }
  void show() const {
    std::cout << "书号：" << bid << " 书名：" << title << " 作者：" << author
              << " 现存量：" << mount << std::endl;
  }
  bool Invalid() const {  // 是否无效
    return (mount == 0 && rcd.empty());
  }
  bool operator==(const Book& book) const { return this->bid == book.bid; }
  bool operator!=(const Book& book) const { return this->bid != book.bid; }
  bool operator<(const Book& book) const { return this->bid < book.bid; }
  bool operator>(const Book& book) const { return this->bid > book.bid; }

  bool operator==(const int bid) const { return this->bid == bid; }
  bool operator!=(const int bid) const { return this->bid != bid; }
  bool operator<(const int bid) const { return this->bid < bid; }
  bool operator>(const int bid) const { return this->bid > bid; }

  int operator+=(const int number) { mount += number; }
  int operator-=(const int number) { mount -= number; }

  bool beConsistentwith(const Book& book) const {
    return title == book.title && author == book.author;
  }

  // 重载输入运算符
  friend std::istream& operator>>(std::istream& is, Book& book) {
    std::cout << "请输入书号：";
    is >> book.bid;
    std::cout << "请输入书名：";
    is >> book.title;
    std::cout << "请输入作者：";
    is >> book.author;
    std::cout << "请输入现存量：";
    is >> book.mount;
    return is;
  }
  // 重载输出运算符
  friend std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << "书号：" << book.bid << " 书名：" << book.title << " 作者："
       << book.author << " 现存量：" << book.mount << std::endl;
    return os;
  }

 private:
  int bid;             // 书号
  std::string title;   // 书名
  std::string author;  // 作者
  int mount;           // 现存量
  // 不记录总库存量，因为总库存量可以通过借阅记录和现存量计算得到
  // 不记录借出量，因为借出量可以通过借阅记录计算得到
  std::list<BorrowRcd> rcd;  // 借阅记录

 public:
  void borrowBook(int rid) {  // 借书
    if (mount > 0) {
      mount--;
      rcd.push_back(BorrowRcd(rid));
      std::cout << "借书成功！" << std::endl;
      rcd.back().show();
    } else {
      std::cout << "该书已经全部借出！" << std::endl;
    }
  }
  void returnBook(int rid) {  // 还书
    if (rcd.empty()) {
      std::cout << "该书没有被借出！" << std::endl;
    } else {
      for (auto it = rcd.begin(); it != rcd.end(); it++) {
        if (it->getRid() == rid) {
          rcd.erase(it);
          mount++;
          return;
        }
      }
      std::cout << "该书没有被借出！" << std::endl;
    }
  }
};

// 用std::tuple代替会更好吗?不，因为std::tuple计算借书期限比较麻烦

using INDEX_SIZE = std::vector<std::shared_ptr<BTreeNode>>::
    size_type;  // 结点的孩子数量类型,兼容书的数量类型,实际就是size_t
struct BTreeNode {          // 2-3树（3阶B树）的节点
  std::vector<Book> books;  // 书
  std::vector<std::shared_ptr<BTreeNode>> children;  // 子节点
  std::weak_ptr<BTreeNode>
      parent;  // 父节点,weak_ptr不会增加引用计数，防止循环引用
  BTreeNode(Book& b) : books(1, b), children(2, nullptr) {}
};
class result {  // 查找结果
  std::shared_ptr<BTreeNode>
      node;  // 节点，如果找到，node为书所在的节点，否则为应该插入的位置；如果为nullptr，说明树为空
  INDEX_SIZE index;  // 索引,如果找到，index为书的索引，否则为应该插入的位置；
  bool tag;          // 是否找到
  result(std::shared_ptr<BTreeNode> n, INDEX_SIZE i, bool t)
      : node(n), index(i), tag(t) {}
  friend class Library;  // Library可以访问result的构造函数
};

class Library {  // Singleton 单例模式

 public:
  static Library& getInstance() {  // 获取实例
    static Library instance;
    return instance;
  }

 private:
  std::shared_ptr<BTreeNode> root;  // 根节点

  Library() : root(nullptr) {}                  // 构造函数私有化
  Library(const Library&) = delete;             // 禁止拷贝构造
  Library& operator=(const Library&) = delete;  // 禁止赋值
  ~Library() {                                  // 析构函数
  }
  result find(int bid) const {  // 查找书
    if (root == nullptr) {
      return result(nullptr, -1, false);  // 空树
    } else {
      std::shared_ptr<BTreeNode> curr = root;
      while (curr != nullptr) {
        INDEX_SIZE i = 0;
        for (; i < curr->books.size(); i++) {
          if (curr->books[i] == bid) {
            return result(curr, i, true);  // 找到
          } else if (
              curr->books[i] >
              bid) {  // 只要找到比bid大的书，就可以确定bid不在这个节点，而且只可能在curr->children[i]中
            curr = curr->children[i];
            break;
          }
        }
        // 此时curr可能已经变成了curr->children[i]。
        // 但是如果i==curr->books.size()，则说明bid比
        // curr->books中的所有书都大，所以只可能在curr->children[i+1]中
        if (curr->children.empty()) {     // 叶子节点
          return result(curr, i, false);  // 没找到
        }
        if (i == curr->books.size()) {
          curr = curr->children[i + 1];
        }
      }
    }
  }

  bool insert(
      Book b,
      int number) {  // 当且仅当书号相同但书的其他信息不同时插入失败，返回false
    // 插入书
    result r = find(b.getBid());
    if (r.node == nullptr) {                  // 空树
      root = std::make_shared<BTreeNode>(b);  // 根节点为空，新建根节点
      root->books[0].setMount(number);        // 设置数量
      return true;
    } else if (r.tag) {  // 找到
      if (b.beConsistentwith(
              r.node->books[r.index])) {  // 书号相同，书的其他信息也相同
        r.node->books[r.index] += number;  // 增加数量
        return true;
      } else {  // 书号相同，书的其他信息不同
        return false;
      }
    } else {                                                     // 没找到
      r.node->books.insert(r.node->books.begin() + r.index, b);  // 插入书
      // 插入书后，可能导致节点中书的数量超过3，需要分裂节点

      if (r.node->books.size() == 3) {
        DisMore(r.node, r.index);  // 调整B树的结构，使其满足B树的定义
      }
    }
  }

  bool GiveBack(int bid) {  // 还书
    result r = find(bid);
    if (r.tag) {  // 找到
      r.node->books[r.index] += 1;
      return true;
    }
    // 该书号对应的书不存在
    return false;
  }
  bool DeleteBook(int bid) {
    result r = find(bid);
    if (!r.tag) return false;                                // 没找到
    if (r.node->children.empty()) {                          // 终端节点
      r.node->books.erase(r.node->books.begin() + r.index);  // 删除书
      r.node->children.erase(r.node->children.begin() + r.index +
                             1);  // 删除孩子
      if (r.node->books.empty())
        DisLess(r.node, r.index);  // 调整B树的结构，使其满足B树的定义
    } else {                       // 非终端节点
    }
  }

  void DisMore(std::shared_ptr<BTreeNode> ErrorNode,
               INDEX_SIZE index) {  // 调整B树的结构，使其满足B树的定义
    std::shared_ptr<BTreeNode> OverweightNode(ErrorNode);
    auto newNode = std::make_shared<BTreeNode>(
        OverweightNode->books.back());  // 将最后一个书作为新节点的书
    OverweightNode->books.pop_back();             // 删除最后一个书
    auto parent = OverweightNode->parent.lock();  // 获取父节点
    do {
      if (parent == nullptr) {  // 根节点
        parent = std::make_shared<BTreeNode>(
            OverweightNode->books.back());  // 将最后一个书作为新节点的书
        OverweightNode->books.pop_back();  // 删除最后一个书
        parent->children[0] = OverweightNode;
        parent->children[1] = newNode;
        OverweightNode->parent = parent;
        newNode->parent = parent;
        root = parent;
      } else {  // 非根节点
        parent->books.insert(
            parent->books.begin() + index,
            OverweightNode->books.back());  // 将最后一个书插入父节点
        OverweightNode->books.pop_back();   // 删除最后一个书
        parent->children.insert(parent->children.begin() + index + 1,
                                newNode);  // 将新节点插入父节点
      }
      OverweightNode = parent;  // 继续向上分裂
    } while (OverweightNode->books.size() == 3);
  }
  void DisLess(std::shared_ptr<BTreeNode> ErrorNode,
               INDEX_SIZE index) {  // 调整B树的结构，使其满足B树的定义
    std::shared_ptr<BTreeNode> BorrowNode(ErrorNode);  // 需要借书的节点
    do {
      auto const parent = ErrorNode->parent.lock();  // 获取父节点
      if (parent ==
          nullptr) {  // 根节点，且根节点已经没有书了，说明整棵树已经没有书了
        root = nullptr;
      } else {  // 非根节点,找到兄弟节点
        auto merge = [parent](INDEX_SIZE index) {
          parent->children[index]->books.insert(
              parent->children[index]->books.begin(),
              parent->children[index + 1]->books.begin(),
              parent->children[index + 1]->books.end());
          parent->children[index]->children.insert(
              parent->children[index]->children.begin(),
              parent->children[index + 1]->children.begin(),
              parent->children[index + 1]->children.end());
        };
        // TODO:把局部调整封装成函数
        auto MergeTO = [parent](INDEX_SIZE base, INDEX_SIZE inc) {
            //1.直接借
            //2.重父节点借，需要merge
        };
        switch (index) {
          case (0):

            BorrowNode->books.push_back(parent->books[0]);
            if (parent->children[1]->books.size() ==
                2) {  // 兄弟节点有2本书，直接借书

              parent->books[0] = parent->children[1]->books[0];
              parent->children[1]->books.erase(
                  parent->children[1]->books.begin());

              // 移交子树
              BorrowNode->children.push_back(parent->children[0]);
              parent->children.erase(parent->children.begin());
            } else {  // 兄弟节点有1本书，需要从双亲节点借书
              parent->books.erase(parent->books.begin());

              // 合并
              merge(0);
              parent->children.erase(parent->children.begin() + 1);
            }
            break;
          case (2):
            BorrowNode->books.push_back(parent->books.back());
            if (parent->children[1]->books.size() ==
                2) {  // 兄弟节点有2本书，直接借书
              parent->books[0] = parent->children[1]->books[1];
              parent->children[1]->books.erase(
                  parent->children[1]->books.end());

              // 移交子树
              BorrowNode->children.insert(BorrowNode->children.begin(),
                                          parent->children[1]->children.back());
              parent->children[1]->children.pop_back();
            } else {
              parent->books.pop_back();

              // 合并
              merge(1);
              parent->children.pop_back();
            }
            break;
          case (1):
            if (parent->children[0]->books.size() ==
                2) {  // 左兄弟节点有2本书，直接借书
              BorrowNode->books.insert(BorrowNode->books.begin(),
                                       parent->books[0]);

              parent->books[0] = parent->children[0]->books[1];
              parent->children[0]->books.pop_back();

              // 移交子树

            } else if (parent->books.size() == 2 &&
                       parent->children[2]->books.size() ==
                           2) {  // 右兄弟节点有2本书，直接借书
              BorrowNode->books.push_back(parent->books[2]);
              parent->books[1] = parent->children[1]->books[0];
              parent->children[1]->books.erase(
                  parent->children[1]->books.begin());

              // 移交子树
              BorrowNode->children.push_back(parent->children[0]);
              parent->children.erase(parent->children.begin());
            }
        }
      }
      BorrowNode = parent;  // 继续向上检查
    } while (BorrowNode->books.empty());
  }
};
