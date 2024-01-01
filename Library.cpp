#include "Library.h"

void Book::show() const {
  std::cout << "��ţ�" << bid << " ������" << title << " ���ߣ�" << author
            << " �ִ�����" << mount << std::endl;
  // ��ӡ��������ڵļ�¼
  if (rcd.empty()) return;
  auto Ercd = rcd.begin();
  for (auto it = rcd.begin(); it != rcd.end(); it++) {
    if (it->getDL() < Ercd->getDL()) {
      Ercd = it;
    }
  }
  Ercd->show();
}
bool Book::borrowBook(int rid) {  // ����
  if (mount > 0) {
    mount--;
    rcd.push_back(BorrowRcd(rid));
    std::cout << "����ɹ���" << std::endl;
    rcd.back().show();
    return true;

  } else {
    std::cout << "�����Ѿ�ȫ�������" << std::endl;
    return false;
  }
}
bool Book::returnBook(int rid) {  // ����
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
           std::string author)  // Ĭ��mountΪ0,��Ҫ��setMount���
    : bid(bid), title(title), author(author), mount(mount) {
  std::cout << "�����������,�Ƿ񲹳��鼮��Ϣ����y���ǣ�n:��" << std::endl;

  char answer = 'n';

  /* DEBUGING
  std::cin >> answer;
  while (answer != 'y' && answer != 'n') {
    std::cout << "��������Чѡ�";
    std::cin >> answer;
  }
  */

  if (answer == 'y') {
    std::string title, author;
    std::cout << "�벹���鼮��Ϣ��" << std::endl << "������";
    std::cin >> title;
    std::cout << std::endl << "���ߣ�";
    std::cin >> author;
    std::cout << std::endl << "�鼮��Ϣ��д��ɡ�" << std::endl;
    this->title = title;
    this->author = author;
  }
  std::cout << "���鴴����ɣ�";
  show();
}

result Library::find(int bid) const {  // ������
  if (root == nullptr) {
    return result(nullptr, -1, false);  // ����
  } else {
    std::shared_ptr<BTreeNode> curr = root;
    std::shared_ptr<BTreeNode> parent = root;

    INDEX_SIZE i = 0;
    while (curr != nullptr) {
      bool flag = false;
      for (i = 0; i < curr->books.size(); i++) {
        if (curr->books[i] == bid) {
          return result(curr, i, true);  // �ҵ�
        } else if (
            curr->books[i] >
            bid) {  // ֻҪ�ҵ���bid����飬�Ϳ���ȷ��bid��������ڵ㣬����ֻ������curr->children[i]��
          parent = curr;
          curr = curr->children[i];
          flag = true;
          break;
        }
      }
      // ��ʱcurr�����Ѿ������curr->children[i]��
      // ��������Ѿ�������ȫ��books����˵��bid��
      // curr->books�е������鶼������ֻ���������һ��������
      if (flag == false) {
        parent = curr;
        curr = curr->children.back();
      }
    }
    return result(parent, i, false);  // û�ҵ�
  }
}
void Library::AddBook(const int bid, const int number) {
  result r = find(bid);
  /*std::cout << r.node << std::endl
              << r.index << std::endl
              << r.tag << std::endl;
  */
  if (bid == 18) {
    std::cout << "DEBUGING!";
  }
  if (r.tag) {
    std::cout << "���ӿ��" << number << "��,"
              << "���ܿ��Ϊ" << (r.node->books[r.index] += number) << "����";
  } else {
    Book b(bid, number);
    if (root == nullptr) {                    // ���ڵ�Ϊ�գ���Ϊ����
      root = std::make_shared<BTreeNode>(b);  // �½����ڵ�
    } else {                                  // �ڷǿ�����������
      insert(std::move(b), r);
    }
  }
}

void Library::insert(Book&& b, result& r) {
  r.node->books.insert(r.node->books.begin() + r.index, b);  // ������
  r.node->children.push_back(nullptr);

  // ������󣬿��ܵ��½ڵ�����������ﵽ3������B���Ľṹ
  if (r.node->books.size() == 3) {
    DisMore(r.node);
  }
}
bool Library::GiveBack(int bid, int rid) {  // ����
  result r = find(bid);
  if (r.tag) {  // �ҵ�
    if (r.node->books[r.index].returnBook(rid)) {
      r.node->books[r.index] += 1;
      return true;
    } else {
      std::cout << "��û�н�����顣" << std::endl;
    }
  }
  // ����Ŷ�Ӧ���鲻����
  std::cout << "���鲻���ڣ���������ȷ����š�" << std::endl;
  return false;
}
void Library::Display() {
  auto printNode = [](std::shared_ptr<BTreeNode> node, int depth) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    for (int i = 0; i < node->books.size(); i++) {
      std::cout << node->books[i].getBid() << ",";
    }
    std::cout << std::endl;
  };
  traverse(root, printNode, 0);
}
bool Library::DeleteBook(const int bid) {
  result r = find(bid);
  if (!r.tag) return false;  // û�ҵ�
  DeleteBTree(r.node, r.index);
  return true;
}
bool Library::Borrow(int bid, int rid) {  // ���ﷵ�����Ϳ���Ϊvoid
  result r = find(bid);
  if (r.tag) {
    std::cout << "���鲻���ڡ�";
    return false;
  }
  if (r.node->books[r.index].borrowBook(rid)) return true;
  return false;
}
void Library::DeleteBTree(std::shared_ptr<BTreeNode> node,
                          const INDEX_SIZE& index) {
  if (node->children.empty()) {
    node->books.erase(node->books.begin() + index);            // ɾ����
    node->children.erase(node->children.begin() + index + 1);  // ɾ������
    if (node->books.empty())
      DisLess(node, index);  // ����B���Ľṹ��ʹ������B���Ķ���
  } else {
    Successor(node, index + 1);
    DeleteBTree(node, 0);
  }
}
void Library::Successor(std::shared_ptr<BTreeNode>& node,
                        const INDEX_SIZE& index) {
  node = node->children[index + 1];
  if (!node->children.empty()) Successor(node, 1);
}
void Library::DisMore(
    std::shared_ptr<BTreeNode> ErrorNode) {  // ����B���Ľṹ��ʹ������B���Ķ���
  // ���ѽڵ���Ҫ���ѹؼ��ֺ��ƽ�����
  // ���ѹؼ���
  auto newNode = std::make_shared<BTreeNode>(
      ErrorNode->books.back());  // �����һ������Ϊ�½ڵ����
  ErrorNode->books.pop_back();
  // �ƽ���������
  if (ErrorNode->children[2] != nullptr) {
    newNode->children[0] = ErrorNode->children[2];
    newNode->children[0]->parent = newNode;
    newNode->children[1] = ErrorNode->children[3];
    newNode->children[1]->parent = newNode;
  }
  ErrorNode->children.pop_back();
  ErrorNode->children.pop_back();

  // �����м䣬��Ҫ��ȡ���ڵ�ָ��
  auto parent = ErrorNode->parent.lock();

  if (parent == nullptr) {                                          // ���ڵ�
    parent = std::make_shared<BTreeNode>(ErrorNode->books.back());  // ������ڵ�
    ErrorNode->books.pop_back();  // ɾ�����һ����

    parent->children[0] = ErrorNode;
    parent->children[1] = newNode;
    ErrorNode->parent = parent;
    newNode->parent = parent;
    root = parent;

  } else {               // �Ǹ��ڵ�
    INDEX_SIZE pos = 0;  // OverweightNode�ڸ��ڵ��е�λ��
    for (; pos < parent->children.size(); pos++) {
      if (parent->children[pos] == ErrorNode) break;
    }

    parent->books.insert(parent->books.begin() + pos,
                         ErrorNode->books.back());  // �����һ������븸�ڵ�
    ErrorNode->books.pop_back();                    // ɾ�����һ����
    parent->children.insert(parent->children.begin() + pos + 1,
                            newNode);  // ���½ڵ��ƽ����ڵ�
    newNode->parent = parent;
  }

  if (parent->books.size() == 3) DisMore(parent);
}
void Library::DisLess(
    std::shared_ptr<BTreeNode> ErrorNode,
    const INDEX_SIZE& index) {  // ����B���Ľṹ��ʹ������B���Ķ���
  std::shared_ptr<BTreeNode> BorrowNode(ErrorNode);  // ��Ҫ����Ľڵ�
  do {
    auto const parent = ErrorNode->parent.lock();  // ��ȡ���ڵ�
    if (parent == nullptr) {  // ���ڵ��Ѿ�û������
      root = BorrowNode;
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
      // �ֲ�����
      auto Adjust = [parent, merge](INDEX_SIZE to, INDEX_SIZE from) {
        bool toLeft = to < from;
        INDEX_SIZE PBookPos =
            toLeft ? to : from;  // ���ڵ�����Ҫ�����õ����index
        parent->children[to]->books.push_back(parent->books[PBookPos]);
        // 1.ֱ�ӽ�
        if (parent->children[from]->books.size() == 2) {
          INDEX_SIZE upside = toLeft ? 0 : 1;  // �������Ƶ�book��λ��
          parent->books[PBookPos] = parent->children[from]->books[upside];
          parent->children[from]->books.erase(
              parent->children[from]->books.begin() + upside);
          // �ƽ�����
          if (toLeft) {
            parent->children[to]->children.insert(
                parent->children[to]->children.end(),
                parent->children[from]->children.front());
            parent->children[from]->children.erase(
                parent->children[from]->children.begin());
          } else {
            parent->children[to]->children.insert(
                parent->children[to]->children.begin(),
                parent->children[from]->children.back());
            parent->children[from]->children.erase(
                parent->children[from]->children.end());
          }
        }
        // 2.���ڵ�裬��Ҫmerge
        parent->books.erase(parent->books.begin() + PBookPos);
        merge(PBookPos);  // ʼ������ϲ����Ա�֤�ؼ�������
        parent->children.erase(parent->children.begin() + PBookPos + 1);
      };
      switch (index) {
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
    BorrowNode = parent;  // �������ϼ��
  } while (BorrowNode->books.empty());
}
