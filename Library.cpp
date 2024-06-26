#include "Library.h"

void Book::show() const {
  std::cout << "书号：" << bid << "\t书名：" << title << "\t作者：" << author
            << "\t现存量：" << mount << std::endl;
  // 打印出最早过期的记录
  if (rcd.empty()) return;
  auto Ercd = rcd.begin();
  for (auto it = rcd.begin(); it != rcd.end(); it++) {
    if (it->getDL() < Ercd->getDL()) {
      Ercd = it;
    }
  }
  Ercd->show();
}
bool Book::borrowBook(int rid) {  // 借书
  if (mount > 0) {
    mount--;
    rcd.push_back(BorrowRcd(rid));
    std::cout << "借书成功！" << std::endl;
    rcd.back().show();
    return true;

  } else {
    std::cout << "该书已经全部借出！" << std::endl;
    return false;
  }
}
bool Book::returnBook(int rid) {  // 还书
  for (auto it = rcd.begin(); it != rcd.end(); it++) {
    if (it->getRid() == rid) {
      rcd.erase(it);
      mount++;
      return true;
    }
  }
  return false;
}

Book::Book(int bid, int mount, std::string title,
           std::string author)  
    : bid(bid), title(title), author(author), mount(mount) {
  std::cout << "新书创建完成：";
  show();
}

result Library::find(int bid) const {  // 查找书
  if (root == nullptr) {
    return result(nullptr, -1, false);  // 空树
  } else {
    std::shared_ptr<BTreeNode> curr = root;
    std::shared_ptr<BTreeNode> parent = root;

    INDEX_SIZE i = 0;
    while (curr != nullptr) {
      bool flag = false;
      for (i = 0; i < curr->books.size(); i++) {
        if (curr->books[i] == bid) {
          return result(curr, i, true);  // 找到
        } else if (curr->books[i] >
                   bid) {  // 只要找到比bid大的书，就可以确定bid不
          // 在这个节点，而且只可能在curr->children[i]中
          parent = curr;
          curr = curr->children[i];
          flag = true;
          break;
        }
      }
      // 此时curr可能已经变成了curr->children[i]。
      // 但是如果已经遍历了全部books，则说明bid比
      // curr->books中的所有书都大，所以只可能在最后一个子树中
      if (flag == false) {
        parent = curr;
        curr = curr->children.back();
      }
    }
    return result(parent, i, false);  // 没找到
  }
}
void Library::AddBook(const int bid, const int number, std::string title,
                      std::string author) {
  result r = find(bid);
  if (r.tag) {
    std::cout << "增加库存" << number << "本,"
              << "现总库存为" << (r.node->books[r.index] += number) << "本。";
    if(bid==16){
    std::cout<<"调试"<<std::endl;
    }
  } else {
    Book b(bid, number, title, author);
    if (root == nullptr) {                    // 根节点为空，则为空树
      root = std::make_shared<BTreeNode>(b);  // 新建根节点
    } else {                                  // 在非空树插入新书
      insert(std::move(b), r);
    }
  }
}

void Library::AddBook(std::tuple<int, std::string, std::string> b) {
  AddBook(std::get<0>(b), 10, std::get<1>(b), std::get<2>(b));
}

void Library::insert(Book&& b, result& r) {  // 当且仅当书号相同
  // 但书的其他信息不同时插入失败，返回false
  r.node->books.insert(r.node->books.begin() + r.index, b);  // 插入书
  r.node->children.push_back(nullptr);

  // 插入书后，可能导致节点中书的数量达到3，调整B树的结构
  if (r.node->books.size() == 3) {
    DisMore(r.node);
  }
}
bool Library::GiveBack(int bid, int rid) {  // 还书
  result r = find(bid);
  if (r.tag) {  // 找到
    if (r.node->books[r.index].returnBook(rid)) {
      std::cout << "还书成功。" << std::endl;
      return true;
    } else {
      std::cout << "您没有借过此书。" << std::endl;
      return false;
    }
  }
  // 该书号对应的书不存在
  std::cout << "该书不存在，请输入正确的书号。" << std::endl;
  return false;
}
void Library::Display() {  // 以凹入表的形式显示
    std::cout<< "当前B树结构：" << std::endl;
  auto printNode = [](std::shared_ptr<BTreeNode> node, int depth) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    for (int i = 0; i < node->books.size() - 1; i++) {
      std::cout << node->books[i].getBid() << ",";
    }
    std::cout << node->books.back().getBid();
    /*DEBUG ，打印父节点
    auto parent = node->parent.lock();
    if (parent != nullptr) {
      std::cout << "\tParent:";
      for (int i = 0; i < parent->books.size(); i++) {
        std::cout << parent->books[i].getBid() << ",";
      }
    }*/
    std::cout << std::endl;
  };
  traverse(root, printNode, 0);
}
void Library::showBook(int bid) const {
  result r = find(bid);
  if (r.tag) {
    r.node->books[r.index].show();
  } else {
    std::cout << "该书不存在。" << std::endl;
  }
}
void Library::Borrow(int bid, int rid) {  // 这里返回类型可以为void
  result r = find(bid);
  if (!r.tag) {
    std::cout << "该书不存在。";
  } else if (r.node->books[r.index].borrowBook(rid)) {
    std::cout << "借书成功。";
  } else {
	std::cout << "该书已经全部借出。";
  }
}

bool Library::DeleteBook(const int bid) {
  result r = find(bid);
  std::cout << "删除" << bid << std::endl;
  if (!r.tag) return false;  // 没找到
  DeleteBTree(r.node, r.index);
  std::cout << "删除成功" << std::endl << std::endl;
  return true;
}

void Library::DeleteBTree(std::shared_ptr<BTreeNode> node,
                          const INDEX_SIZE& index) {
  if (node->children[0] == nullptr) {
    node->books.erase(node->books.begin() + index);  // 删除书
    node->children.pop_back();  // 最下层节点的children都为空，删除任意一个
    if (node->books.empty()) DisLess(node);  // 调整B树的结构，使其满足B树的定义
  } else {
    Successor(node, index);
    DeleteBTree(node, 0);
  }
}
void Library::Successor(std::shared_ptr<BTreeNode>& node,
                        const INDEX_SIZE& index) {
  node->books[index] = node->children[index + 1]->books[0];
  node = node->children[index + 1];
  if (node->children[0] != nullptr) Successor(node, 0);
}
void Library::DisMore(
    std::shared_ptr<BTreeNode> ErrorNode) {  // 调整B树的结构，使其满足B树的定义
  // 分裂节点需要分裂关键字和移交子树
  // 分裂关键字
  auto newNode = std::make_shared<BTreeNode>(
      ErrorNode->books.back());  // 将最后一个书作为新节点的书
  ErrorNode->books.pop_back();
  // 移交两棵子树
  if (ErrorNode->children[2] != nullptr) {
    newNode->children[0] = ErrorNode->children[2];
    newNode->children[0]->parent = newNode;
    newNode->children[1] = ErrorNode->children[3];
    newNode->children[1]->parent = newNode;
  }
  ErrorNode->children.pop_back();
  ErrorNode->children.pop_back();

  // 上移中间，需要获取父节点指针
  auto parent = ErrorNode->parent.lock();

  if (parent == nullptr) {  // 根节点
    parent =
        std::make_shared<BTreeNode>(ErrorNode->books.back());  // 构造根节点
    ErrorNode->books.pop_back();  // 删除最后一个书

    parent->children[0] = ErrorNode;
    parent->children[1] = newNode;
    ErrorNode->parent = parent;
    newNode->parent = parent;
    root = parent;

  } else {               // 非根节点
    INDEX_SIZE pos = 0;  // ErrorNode在父节点中的位置
    for (; pos < parent->children.size(); pos++) {
      if (parent->children[pos] == ErrorNode) break;
    }

    parent->books.insert(parent->books.begin() + pos,
                         ErrorNode->books.back());  // 将最后一个书插入父节点
    ErrorNode->books.pop_back();                    // 删除最后一个书
    parent->children.insert(parent->children.begin() + pos + 1,
                            newNode);  // 将新节点移交父节点
    newNode->parent = parent;
  }

  if (parent->books.size() == 3) DisMore(parent);
}
void Library::DisLess(
    std::shared_ptr<BTreeNode> ErrorNode) {  // 调整B树的结构，使其满足B树的定义
  if (ErrorNode->parent.lock() == nullptr) {  // 调整根
    root = ErrorNode->children[0];
    return;
  }
  auto const parent = ErrorNode->parent.lock();
  INDEX_SIZE pos = 0;  // ErrorNode在父节点中的位置
  for (; pos < parent->children.size(); pos++) {
    if (parent->children[pos] == ErrorNode) break;
  }
  if (parent == nullptr) {  // 根节点已经没有书了
    root = ErrorNode;
  } else {  // 非根节点,找到兄弟节点
    auto merge = [parent](INDEX_SIZE index) {
      parent->children[index]->books.insert(
          parent->children[index]->books.end(),
          parent->children[index + 1]->books.begin(),
          parent->children[index + 1]->books.end());
      parent->children[index]->children.insert(
          parent->children[index]->children.end(),
          parent->children[index + 1]->children.begin(),
          parent->children[index + 1]->children.end());
      if (parent->children[index]->children[0] != nullptr)
        for (int i = 0; i < parent->children[index]->children.size(); i++)
          parent->children[index]->children[i]->parent =
              parent->children[index];
    };
    // 局部调整
    auto Adjust = [parent, merge](INDEX_SIZE to, INDEX_SIZE from) {
      bool toLeft = to < from;
      INDEX_SIZE PBookPos =
          toLeft ? to : from;  // 父节点中需要被借用的书的index
      parent->children[to]->books.push_back(parent->books[PBookPos]);
      // 1.直接借
      if (parent->children[from]->books.size() == 2) {
        INDEX_SIZE upside = toLeft ? 0 : 1;  // 将被上移的book的位置
        parent->books[PBookPos] = parent->children[from]->books[upside];
        parent->children[from]->books.erase(
            parent->children[from]->books.begin() + upside);
        // 移交子树
        if (toLeft) {
          parent->children[to]->children.push_back(
              parent->children[from]->children.front());
          parent->children[from]->children.erase(
              parent->children[from]->children.begin());
          if (parent->children[to]->children.back() != nullptr)
            parent->children[to]->children.back()->parent =
                parent->children[to];
        } else {
          parent->children[to]->children.insert(
              parent->children[to]->children.begin(),
              parent->children[from]->children.back());
          parent->children[from]->children.pop_back();
          if (parent->children[to]->children.front() != nullptr)
            parent->children[to]->children.front()->parent =
                parent->children[to];
        }
      } else {  // 2.从父节点借，需要merge
        parent->books.erase(parent->books.begin() + PBookPos);
        merge(PBookPos);  // 始终向左合并，以保证关键字升序
        parent->children.erase(parent->children.begin() + PBookPos + 1);
      }
    };
    switch (pos) {
      case (0):
        Adjust(0, 1);
        break;
      case (1):
        if (parent->children.size() == 3 &&
            parent->children[2]->books.size() == 2)
          Adjust(1, 2);
        else {
          Adjust(1, 0);
        }
        break;
      case (2):
        Adjust(2, 1);
    }
  }
  if (parent->books.empty()) DisLess(parent);
}
