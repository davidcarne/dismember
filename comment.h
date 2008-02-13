#ifndef _COMMENT_H_
#define _COMMENT_H_
#include "types.h"
#include <string>
#include <map>

class Comment
{
 public:
	const std::string get_comment() const;
	const address_t get_address() const;
	const int get_lines() const;

	void set_comment(std::string);
	void set_address(address_t addr);

 protected:
	friend class CommentList;
	Comment(std::string comment, address_t addr);

 private:
	std::string comment;
	address_t address;
	int lines;
};

class CommentList
{
 public:
	CommentList();

	Comment *set_comment(address_t addr, std::string comment);
	Comment *get_comment(address_t addr) const;

 private:
	std::map<address_t, Comment *> commentmap;
};

#endif
