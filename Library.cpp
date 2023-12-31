#include "Library.h"

class BorrowRcd {  // ���ļ�¼
                   // ÿ����¼Ϊһ�ν��飬ÿ��ֻ�ܽ�һ����
 public:
  BorrowRcd(int id) : rid(id) {
    deadline = std::chrono::system_clock::now() +
               std::chrono::days(30);  // ��������Ϊ30��
  }
  void show() const {
    std::time_t time = std::chrono::system_clock::to_time_t(deadline);
    std::println("ͼ��֤�ţ�{} ��Ӧ�黹ʱ�䣺{}", rid, time);  // ֱ�Ӵ�ӡʱ���
  }
  bool isExpired() const {  // �Ƿ����
    return deadline < std::chrono::system_clock::now();
  }
  int getRid() const { return rid; }

 private:
  int rid;  // ͼ��֤��
  std::chrono::system_clock::time_point
      deadline;  // Ӧ�黹ʱ��,���ݵ�ǰʱ��������
};

class Book {
 public:
  Book() = default;
  Book(int bid, std::string title, std::string author,
       int mount = 0)  // Ĭ��mountΪ0,��Ҫ��setMount���
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
    std::cout << "��ţ�" << bid << " ������" << title << " ���ߣ�" << author
              << " �ִ�����" << mount << std::endl;
  }
  bool Invalid() const {  // �Ƿ���Ч
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

  // �������������
  friend std::istream& operator>>(std::istream& is, Book& book) {
    std::cout << "��������ţ�";
    is >> book.bid;
    std::cout << "������������";
    is >> book.title;
    std::cout << "���������ߣ�";
    is >> book.author;
    std::cout << "�������ִ�����";
    is >> book.mount;
    return is;
  }
  // ������������
  friend std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << "��ţ�" << book.bid << " ������" << book.title << " ���ߣ�"
       << book.author << " �ִ�����" << book.mount << std::endl;
    return os;
  }

 private:
  int bid;             // ���
  std::string title;   // ����
  std::string author;  // ����
  int mount;           // �ִ���
  // ����¼�ܿ��������Ϊ�ܿ��������ͨ�����ļ�¼���ִ�������õ�
  // ����¼���������Ϊ���������ͨ�����ļ�¼����õ�
  std::list<BorrowRcd> rcd;  // ���ļ�¼

 public:
  void borrowBook(int rid) {  // ����
    if (mount > 0) {
      mount--;
      rcd.push_back(BorrowRcd(rid));
      std::cout << "����ɹ���" << std::endl;
      rcd.back().show();
    } else {
      std::cout << "�����Ѿ�ȫ�������" << std::endl;
    }
  }
  void returnBook(int rid) {  // ����
    if (rcd.empty()) {
      std::cout << "����û�б������" << std::endl;
    } else {
      for (auto it = rcd.begin(); it != rcd.end(); it++) {
        if (it->getRid() == rid) {
          rcd.erase(it);
          mount++;
          return;
        }
      }
      std::cout << "����û�б������" << std::endl;
    }
  }
};

// ��std::tuple����������?������Ϊstd::tuple����������ޱȽ��鷳

using INDEX_SIZE = std::vector<std::shared_ptr<BTreeNode>>::
    size_type;  // ���ĺ�����������,���������������,ʵ�ʾ���size_t
struct BTreeNode {          // 2-3����3��B�����Ľڵ�
  std::vector<Book> books;  // ��
  std::vector<std::shared_ptr<BTreeNode>> children;  // �ӽڵ�
  std::weak_ptr<BTreeNode>
      parent;  // ���ڵ�,weak_ptr�����������ü�������ֹѭ������
  BTreeNode(Book& b) : books(1, b), children(2, nullptr) {}
};
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

 private:
  std::shared_ptr<BTreeNode> root;  // ���ڵ�

  Library() : root(nullptr) {}                  // ���캯��˽�л�
  Library(const Library&) = delete;             // ��ֹ��������
  Library& operator=(const Library&) = delete;  // ��ֹ��ֵ
  ~Library() {                                  // ��������
  }
  result find(int bid) const {  // ������
    if (root == nullptr) {
      return result(nullptr, -1, false);  // ����
    } else {
      std::shared_ptr<BTreeNode> curr = root;
      while (curr != nullptr) {
        INDEX_SIZE i = 0;
        for (; i < curr->books.size(); i++) {
          if (curr->books[i] == bid) {
            return result(curr, i, true);  // �ҵ�
          } else if (
              curr->books[i] >
              bid) {  // ֻҪ�ҵ���bid����飬�Ϳ���ȷ��bid��������ڵ㣬����ֻ������curr->children[i]��
            curr = curr->children[i];
            break;
          }
        }
        // ��ʱcurr�����Ѿ������curr->children[i]��
        // �������i==curr->books.size()����˵��bid��
        // curr->books�е������鶼������ֻ������curr->children[i+1]��
        if (curr->children.empty()) {     // Ҷ�ӽڵ�
          return result(curr, i, false);  // û�ҵ�
        }
        if (i == curr->books.size()) {
          curr = curr->children[i + 1];
        }
      }
    }
  }

  bool insert(
      Book b,
      int number) {  // ���ҽ��������ͬ�����������Ϣ��ͬʱ����ʧ�ܣ�����false
    // ������
    result r = find(b.getBid());
    if (r.node == nullptr) {                  // ����
      root = std::make_shared<BTreeNode>(b);  // ���ڵ�Ϊ�գ��½����ڵ�
      root->books[0].setMount(number);        // ��������
      return true;
    } else if (r.tag) {  // �ҵ�
      if (b.beConsistentwith(
              r.node->books[r.index])) {  // �����ͬ�����������ϢҲ��ͬ
        r.node->books[r.index] += number;  // ��������
        return true;
      } else {  // �����ͬ�����������Ϣ��ͬ
        return false;
      }
    } else {                                                     // û�ҵ�
      r.node->books.insert(r.node->books.begin() + r.index, b);  // ������
      // ������󣬿��ܵ��½ڵ��������������3����Ҫ���ѽڵ�

      if (r.node->books.size() == 3) {
        DisMore(r.node, r.index);  // ����B���Ľṹ��ʹ������B���Ķ���
      }
    }
  }

  bool GiveBack(int bid) {  // ����
    result r = find(bid);
    if (r.tag) {  // �ҵ�
      r.node->books[r.index] += 1;
      return true;
    }
    // ����Ŷ�Ӧ���鲻����
    return false;
  }
  bool DeleteBook(int bid) {
    result r = find(bid);
    if (!r.tag) return false;                                // û�ҵ�
    if (r.node->children.empty()) {                          // �ն˽ڵ�
      r.node->books.erase(r.node->books.begin() + r.index);  // ɾ����
      r.node->children.erase(r.node->children.begin() + r.index +
                             1);  // ɾ������
      if (r.node->books.empty())
        DisLess(r.node, r.index);  // ����B���Ľṹ��ʹ������B���Ķ���
    } else {                       // ���ն˽ڵ�
    }
  }

  void DisMore(std::shared_ptr<BTreeNode> ErrorNode,
               INDEX_SIZE index) {  // ����B���Ľṹ��ʹ������B���Ķ���
    std::shared_ptr<BTreeNode> OverweightNode(ErrorNode);
    auto newNode = std::make_shared<BTreeNode>(
        OverweightNode->books.back());  // �����һ������Ϊ�½ڵ����
    OverweightNode->books.pop_back();             // ɾ�����һ����
    auto parent = OverweightNode->parent.lock();  // ��ȡ���ڵ�
    do {
      if (parent == nullptr) {  // ���ڵ�
        parent = std::make_shared<BTreeNode>(
            OverweightNode->books.back());  // �����һ������Ϊ�½ڵ����
        OverweightNode->books.pop_back();  // ɾ�����һ����
        parent->children[0] = OverweightNode;
        parent->children[1] = newNode;
        OverweightNode->parent = parent;
        newNode->parent = parent;
        root = parent;
      } else {  // �Ǹ��ڵ�
        parent->books.insert(
            parent->books.begin() + index,
            OverweightNode->books.back());  // �����һ������븸�ڵ�
        OverweightNode->books.pop_back();   // ɾ�����һ����
        parent->children.insert(parent->children.begin() + index + 1,
                                newNode);  // ���½ڵ���븸�ڵ�
      }
      OverweightNode = parent;  // �������Ϸ���
    } while (OverweightNode->books.size() == 3);
  }
  void DisLess(std::shared_ptr<BTreeNode> ErrorNode,
               INDEX_SIZE index) {  // ����B���Ľṹ��ʹ������B���Ķ���
    std::shared_ptr<BTreeNode> BorrowNode(ErrorNode);  // ��Ҫ����Ľڵ�
    do {
      auto const parent = ErrorNode->parent.lock();  // ��ȡ���ڵ�
      if (parent ==
          nullptr) {  // ���ڵ㣬�Ҹ��ڵ��Ѿ�û�����ˣ�˵���������Ѿ�û������
        root = nullptr;
      } else {  // �Ǹ��ڵ�,�ҵ��ֵܽڵ�
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
        // TODO:�Ѿֲ�������װ�ɺ���
        auto MergeTO = [parent](INDEX_SIZE base, INDEX_SIZE inc) {
            //1.ֱ�ӽ�
            //2.�ظ��ڵ�裬��Ҫmerge
        };
        switch (index) {
          case (0):

            BorrowNode->books.push_back(parent->books[0]);
            if (parent->children[1]->books.size() ==
                2) {  // �ֵܽڵ���2���飬ֱ�ӽ���

              parent->books[0] = parent->children[1]->books[0];
              parent->children[1]->books.erase(
                  parent->children[1]->books.begin());

              // �ƽ�����
              BorrowNode->children.push_back(parent->children[0]);
              parent->children.erase(parent->children.begin());
            } else {  // �ֵܽڵ���1���飬��Ҫ��˫�׽ڵ����
              parent->books.erase(parent->books.begin());

              // �ϲ�
              merge(0);
              parent->children.erase(parent->children.begin() + 1);
            }
            break;
          case (2):
            BorrowNode->books.push_back(parent->books.back());
            if (parent->children[1]->books.size() ==
                2) {  // �ֵܽڵ���2���飬ֱ�ӽ���
              parent->books[0] = parent->children[1]->books[1];
              parent->children[1]->books.erase(
                  parent->children[1]->books.end());

              // �ƽ�����
              BorrowNode->children.insert(BorrowNode->children.begin(),
                                          parent->children[1]->children.back());
              parent->children[1]->children.pop_back();
            } else {
              parent->books.pop_back();

              // �ϲ�
              merge(1);
              parent->children.pop_back();
            }
            break;
          case (1):
            if (parent->children[0]->books.size() ==
                2) {  // ���ֵܽڵ���2���飬ֱ�ӽ���
              BorrowNode->books.insert(BorrowNode->books.begin(),
                                       parent->books[0]);

              parent->books[0] = parent->children[0]->books[1];
              parent->children[0]->books.pop_back();

              // �ƽ�����

            } else if (parent->books.size() == 2 &&
                       parent->children[2]->books.size() ==
                           2) {  // ���ֵܽڵ���2���飬ֱ�ӽ���
              BorrowNode->books.push_back(parent->books[2]);
              parent->books[1] = parent->children[1]->books[0];
              parent->children[1]->books.erase(
                  parent->children[1]->books.begin());

              // �ƽ�����
              BorrowNode->children.push_back(parent->children[0]);
              parent->children.erase(parent->children.begin());
            }
        }
      }
      BorrowNode = parent;  // �������ϼ��
    } while (BorrowNode->books.empty());
  }
};
