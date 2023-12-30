#include "Library.h"

class BorrowRcd { //���ļ�¼
	//ÿ����¼Ϊһ�ν��飬ÿ��ֻ�ܽ�һ����
public:
	BorrowRcd(int id) :rid(id) {
		deadline = std::chrono::system_clock::now() + std::chrono::days(30); //��������Ϊ30��
	}
	void show() const {
		std::time_t time = std::chrono::system_clock::to_time_t(deadline); 
		std::println("ͼ��֤�ţ�{} ��Ӧ�黹ʱ�䣺{}", rid ,time); //ֱ�Ӵ�ӡʱ���
	}
	bool isExpired() const { //�Ƿ����
		return deadline < std::chrono::system_clock::now();
	}
	int getRid() const { return rid; }
private:
	int rid;             // ͼ��֤��
	std::chrono::system_clock::time_point deadline; //Ӧ�黹ʱ��,���ݵ�ǰʱ��������
};

class Book {
public:
	Book() = default;
	Book(int bid, std::string title, std::string author, int mount)
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
		os << "��ţ�" << book.bid << " ������" << book.title << " ���ߣ�" << book.author
			<< " �ִ�����" << book.mount << std::endl;
		return os;
	}

private:
	int bid;            // ���
	std::string title;  // ����
	std::string author; // ����
	int mount;          // �ִ���
	// ����¼�ܿ��������Ϊ�ܿ��������ͨ�����ļ�¼���ִ�������õ�
	// ����¼���������Ϊ���������ͨ�����ļ�¼����õ�
	std::list<BorrowRcd>  rcd;   // ���ļ�¼

public:
	void borrowBook(int rid) { //����
		if (mount > 0) {
			mount--;
			rcd.push_back(BorrowRcd(rid));
			std::cout << "����ɹ���" << std::endl;
			rcd.back().show();
		}
		else {
			std::cout << "�����Ѿ�ȫ�������" << std::endl;
		}
	}
	void returnBook(int rid) { //����
		if (rcd.empty()) {
			std::cout << "����û�б������" << std::endl;
		}
		else {
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

//��std::tuple����������?������Ϊstd::tuple����������ޱȽ��鷳


struct BTreeNode {  //2-3���Ľڵ�
	std::vector<Book> books; //��
	std::vector<BTreeNode*> children; //�ӽڵ�

};