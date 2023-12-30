export module Library;
export import std;
export class Book {
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
};



export class BorrowRcd { //���ļ�¼
	//ÿ����¼Ϊһ�ν��飬ÿ��ֻ�ܽ�һ����
public:

private:
	int rid;             // ͼ��֤��
	std::chrono::year_month_day deadline; //�黹����
};