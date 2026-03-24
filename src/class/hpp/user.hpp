#ifndef USER_HPP
#define USER_HPP

#include <string>

enum e_userGrade
{
	REGULAR = 0,
	OPERATOR
};

class User
{
	public:
		User();
		User(std::string PuserName);
		User(User &src);
		~User();
	private:
		int fd;
		std::string nickName;
		std::string userName;
		int grade;
};

#endif //USER_HPP