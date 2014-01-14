#ifndef LEAKER_SINGLETON_H_
#define LEAKER_SINGLETON_H_

template <typename T>
class Singleton
{
public:
	static T & instance(){
		static T t_;
		return t_;
	}
protected:
	Singleton(){}
	virtual ~Singleton(){}
private:
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);
};

#endif