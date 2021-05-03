#include<iostream>
#include<string>
#include<windows.h>
#include<ctime>
using namespace std;

class Exception {
protected:
	string text;
	int line;
	string source;
	string createdTime;
	Exception(const string& text, const int line, const string& source, const string& createdTime)
	{
		setText(text);
		setLine(line);
		setSource(source);
		setTime(createdTime);
	}
public:
	string getText()const {
		return text;
	}
	int getLine()const {
		return line;
	}
	string getSource()const {
		return source;
	}
	string getTime()const {
		return createdTime;
	}

	void setText(const string& text) {
		if (text.empty()) throw "Text is empty";
		this->text = text;
	}
	void setLine(const int line) {
		if (line < 0) throw "Line cannot be negative";
		this->line = line;
	}
	void setSource(const string& source) {
		if (source.empty()) throw "Source is empty";
		this->source = source;
	}
	void setTime(const string& createdTime) {
		if (createdTime.empty()) throw "Time is NULL";

		time_t currently = time(0);
		char dt[30]{};
		ctime_s(dt, sizeof dt, &currently);
		string time = dt;

		this->createdTime = dt;
	}


	void PrintMsg()const {
		cout << endl;
		cout << "Text:" << getText() << endl;
		cout << "Line:" << getLine() << endl;
		cout << "Source:" << getSource() << endl;
		cout << "Time: " << getTime() << endl;
	}
};

class DatabaseException :public Exception {
public:
	DatabaseException() = default;
	DatabaseException(const string& text, const int line, const string& source, const string& createdTime)
		:Exception(text, line, source, createdTime)
	{

	}
};

class InvalidArgumentException :public Exception {
public:
	InvalidArgumentException(const string& text, const int line, const string& source, const string& createdTime)
		:Exception(text, line, source, createdTime)
	{

	}
};

class User {
	int id = 0;
	string username = "";
	string password = "";
	string name = "";
	string surname = "";
public:
	User() = default;
	User(const int id, const string& username, const string& password, const string& name, const string& surname)
	{
		setId(id);
		setUsername(username);
		setPassword(password);
		setName(name);
		setSurname(surname);
	}

	int getId()const {
		return id;
	}
	string getUsername()const {
		return username;
	}
	string getPassword()const {
		return password;
	}
	string getName()const {
		return name;
	}
	string getSurname()const {
		return surname;
	}


	void setId(const int id) {
		if (id < 0) throw InvalidArgumentException("Id cannot be negative", __LINE__, "Ex20.cpp", __TIME__);
		this->id = id;
	}
	void setUsername(const string& username) {
		if (username.empty())
			throw InvalidArgumentException("Username is empty", __LINE__, "Ex20.cpp", __TIME__);
		if (username.length() < 6)
			throw InvalidArgumentException("Count of letters in the username must be more than 6.", __LINE__, "Ex20.cpp", __TIME__);
		if (username[0] >= char(97) && username[0] <= char(122))
			throw InvalidArgumentException("The first letter of the username should't be lowercase.", __LINE__, "Ex20.cpp", __TIME__);
		this->username = username;
	}
	void setPassword(const string& password) {
		if (password.empty())
			throw InvalidArgumentException("Password is empty", __LINE__, "Ex20.cpp", __TIME__);
		if (password.size() < 6)
			throw InvalidArgumentException("Count of letters in the password must be more than 6.", __LINE__, "Ex20.cpp", __TIME__);
		this->password = password;
	}
	void setName(const string& name) {
		if (name.empty())
			throw InvalidArgumentException("Name is empty", __LINE__, "Ex20.cpp", __TIME__);
		if (name.size() < 4)
			throw InvalidArgumentException("Count of letters in the name must be more than 4.", __LINE__, "Ex20.cpp", __TIME__);
		this->name = name;

	}
	void setSurname(const string& surname) {
		if (surname.empty())
			throw InvalidArgumentException("Surname is empty", __LINE__, "Ex20.cpp", __TIME__);
		if (surname.size() < 4)
			throw InvalidArgumentException("Count of letters in the surname must be more than 4.", __LINE__, "Ex20.cpp", __TIME__);
		this->surname = surname;
	}

	void show() {
		//cout << "Id: " << getId() << endl;
		cout << "Username: " << getUsername() << endl;
		cout << "Password: " << getPassword() << endl;
		cout << "Name: " << getName() << endl;
		cout << "Surname: " << getSurname() << endl;
	}
};

class Database {
	User** users = nullptr;
	int user_count = 0;
public:
	Database() = default;
	Database(User** users, const int user_count)
	{
		setUserCount(user_count);
		setUsers(users);
	}

	int getUserCount()const {
		return user_count;
	}
	User** getUsers()const {
		return users;
	}

	void setUserCount(const int user_count) {
		if (user_count < 0) throw InvalidArgumentException("User count can't be negative", __LINE__, "Ex20.cpp", __TIME__);
		this->user_count = user_count;
	}
	void setUsers(User** users) {
		if (users == NULL) throw InvalidArgumentException("Users NULL", __LINE__, "Ex20.cpp", __TIME__);
		this->users = users;
	}

	void addUser(User& user) {
		if (isExistsUser(user.getUsername()))
			throw DatabaseException("User already exist.", __LINE__, "Source.cpp", __TIME__);

		User** newUsers = new User * [user_count + 1];
		for (size_t i = 0; i < user_count; i++)
		{
			newUsers[i] = users[i];
		}

		newUsers[user_count] = &user;

		delete[] users;
		users = newUsers;
		newUsers = nullptr;
		user_count++;
	}

	bool isExistsUser(string username) {
		for (size_t i = 0; i < user_count; i++)
		{
			if (users[i]->getUsername() == username) return true;
		}
		return false;
	}

	User& getUserByUsername(string username) {
		if (!isExistsUser(username))
			throw DatabaseException("User not found", __LINE__, "Ex20.cpp", __TIME__);

		for (size_t i = 0; i < user_count; i++)
		{
			if (users[i]->getUsername() == username)
				return *(users[i]);
		}
	};

	void updateUser(User& olduser, const User& newuser) {

		for (size_t i = 0; i < user_count; i++)
		{
			if (users[i] == &olduser) {
				cout << "> > > Update successful " << olduser.getUsername() << " --> changed --> " << newuser.getUsername() << " < < <\n" << endl;
				olduser.setUsername(newuser.getUsername());
				olduser.setPassword(newuser.getPassword());
				olduser.setName(newuser.getName());
				olduser.setSurname(newuser.getSurname());

				return;
			}
		}
		throw DatabaseException("User not found !", __LINE__, "Ex20.cpp", __TIME__);
	}

	void deleteUserById(const int& id) {

		auto newUsers = new User * [user_count - 1];
		int index;
		for (size_t i = 0; i < user_count; i++)
		{
			if (users[i]->getId() == id) {
				index = i;
				break;
			}
			else throw DatabaseException("User not found !", __LINE__, "Ex20.cpp", __TIME__);
		}
		for (size_t j = 0, i = 0; j < index; j++, i++)
		{
			newUsers[j] = users[i];
		}
		for (size_t j = index; j < user_count; j++, index++)
		{
			newUsers[j] = users[index + 1];

		}

		user_count--;

		delete[] users;
		users = newUsers;
		newUsers = nullptr;


	}

	size_t hashCode(const string& str) {
		int h = 0;
		for (size_t i = 0; i < str.size(); ++i)
			h = h * 31 + static_cast<int>(str[i]);
		return h;
	}

	void hashUserDataById(const int& id) {
		for (size_t i = 0; i < user_count; i++)
		{
			if (users[i]->getId() == id) {
				auto hash = hashCode(users[i]->getUsername());
				users[i]->setUsername(to_string(hash));
				hash = hashCode(users[i]->getPassword());
				users[i]->setPassword(to_string(hash));
				hash = hashCode(users[i]->getName());
				users[i]->setName(to_string(hash));
				hash = hashCode(users[i]->getSurname());
				users[i]->setSurname(to_string(hash));
			}
		}
	}

	void show() {
		cout << "* * * * * * * * * * * Database INFO * * * * * * * * * * *" << endl;
		cout << "User count: " << user_count << endl;
		for (int i = 0; i < user_count; i++)
		{
			cout << "Id: " << i + 1 << endl;
			users[i]->show();
			cout << endl;
		}
		cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n" << endl;
	}
};

class Registration {
	Database _database;
public:
	Registration() = default;
	Registration(const Database& database) {
		_database = database;
	}

	Database& getDatabase() { return _database; }

	void signIn(string username, string password) {
		if (!getDatabase().isExistsUser(username))
			throw DatabaseException("User not found !", __LINE__, "Ex20.cpp", __TIME__);

		for (size_t i = 0; i < getDatabase().getUserCount(); i++)
		{
			if (getDatabase().getUsers()[i]->getUsername() == username) {
				if (getDatabase().getUsers()[i]->getPassword() != password)
					throw DatabaseException("Incorrect password !", __LINE__, "Ex20.cpp", __TIME__);
				else {
					system("cls");
					cout << "\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;
					cout << "\t *\t\t\t\t\t\t\t\t   *" << endl;
					cout << "\t  *\t\t> > > Welcome facebook profile < < <\t\t  *" << endl;
					cout << "\t *\t\t\t\t\t\t\t\t   *" << endl;
					cout << "\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;
					cout << "\n\n- - - - User info - - - -" << endl;
					getDatabase().getUsers()[i]->show();
				}
			}
		}
	}

	void signUp(const int id, const string& username, const string& password, const string& name, const string& surname) {
		User* newUser = new User[1];

		newUser->setId(id);
		newUser->setUsername(username);
		newUser->setPassword(password);
		newUser->setName(name);
		newUser->setSurname(surname);

		getDatabase().addUser(*newUser);
		cout << "> > > Registration successful " << username << " < < <\n" << endl;
	}
};

class System {
public:
	static void Control() {
		Database db;
		Registration facebook(db);

		try
		{
			User user1(1, "Kenan27k", "asdfg3", "Kenan", "Idayatov");
			User user2(2, "Revan28r", "rtyjn34", "Revan", "Idayatov");
			User user3(3, "Nebi029n", "12323dsd", "Nebi", "Nebili");
			facebook.getDatabase().addUser(user1);
			facebook.getDatabase().addUser(user2);
			facebook.getDatabase().addUser(user3);
			facebook.getDatabase().show();
			facebook.signUp(4, "Emiraslan123", "asdas788", "Emiraslan", "Alieff");
			facebook.getDatabase().show();
			User newuser(1, "Newuser123", "12345fgs", "newuser", "newuser345");
			facebook.getDatabase().updateUser(user1, newuser);
			facebook.getDatabase().show();
			facebook.getDatabase().deleteUserById(1);
			facebook.getDatabase().show();
			facebook.getDatabase().hashUserDataById(2);
			facebook.getDatabase().show();
			//facebook.signIn("Nebi029n", "12323dsd");  //correct sign in
			//facebook.signIn("Nebi029sds", "1a23sddsd"); //incorrect sign in
		}
		catch (Exception& ex)
		{
			cout << "Exception occurred!";
			ex.PrintMsg();
		}
	}
};



void main() {
	System::Control();

}
