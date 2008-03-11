#ifndef _ABSTRACT_DATA_H_
#define _ABSTRACT_DATA_H_

#include <string>

/* TODO: Replace this all with boost::variant */

class AbstractData
{
 public:
	AbstractData() {};
	virtual ~AbstractData() {};

	virtual std::string text() = 0;
	virtual int number() = 0;
	virtual bool boolean() = 0;
};

class GenericAbstractData
{
 public:
	GenericAbstractData(int data);
	GenericAbstractData(bool data);
	GenericAbstractData(std::string data);
	~GenericAbstractData();

	enum DataType {
		TEXT,
		NUMBER,
		BOOLEAN
	};

	DataType type();

	std::string text();
	int number();
	bool boolean();

 protected:
 	DataType m_type;
	//union data {
	struct data {
		std::string text;
		int number;
		bool boolean;
	} data;
};

#endif
