#include "Library.h"

class BorrowRcd { //借阅记录
	//每个记录为一次借书，每次只能借一本书
public:
	BorrowRcd(int id) :rid(id) {
		deadline = std::chrono::system_clock::now() + std::chrono::days(30); //借书期限为30天
	}
	void show() const {
		std::time_t time = std::chrono::system_clock::to_time_t(deadline); 
		std::println("图书证号：{} ，应归还时间：{}", rid ,time); //直接打印时间戳
	}
	bool isExpired() const { //是否过期
		return deadline < std::chrono::system_clock::now();
	}
	int getRid() const { return rid; }
private:
	int rid;             // 图书证号
	std::chrono::system_clock::time_point deadline; //应归还时间,根据当前时间计算出来
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
		std::cout << "书号：" << bid << " 书名：" << title << " 作者：" << author
			<< " 现存量：" << mount << std::endl;
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
		os << "书号：" << book.bid << " 书名：" << book.title << " 作者：" << book.author
			<< " 现存量：" << book.mount << std::endl;
		return os;
	}

private:
	int bid;            // 书号
	std::string title;  // 书名
	std::string author; // 作者
	int mount;          // 现存量
	// 不记录总库存量，因为总库存量可以通过借阅记录和现存量计算得到
	// 不记录借出量，因为借出量可以通过借阅记录计算得到
	std::list<BorrowRcd>  rcd;   // 借阅记录

public:
	void borrowBook(int rid) { //借书
		if (mount > 0) {
			mount--;
			rcd.push_back(BorrowRcd(rid));
			std::cout << "借书成功！" << std::endl;
			rcd.back().show();
		}
		else {
			std::cout << "该书已经全部借出！" << std::endl;
		}
	}
	void returnBook(int rid) { //还书
		if (rcd.empty()) {
			std::cout << "该书没有被借出！" << std::endl;
		}
		else {
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

//用std::tuple代替会更好吗?不，因为std::tuple计算借书期限比较麻烦


struct BTreeNode {  //2-3树的节点
	std::vector<Book> books; //书
	std::vector<BTreeNode*> children; //子节点

};