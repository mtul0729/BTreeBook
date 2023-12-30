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
};



export class BorrowRcd { //借阅记录
	//每个记录为一次借书，每次只能借一本书
public:

private:
	int rid;             // 图书证号
	std::chrono::year_month_day deadline; //归还期限
};