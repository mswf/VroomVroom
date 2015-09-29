#ifndef singleton_h
#define singleton_h

//Usage :
//
//class MyClass : public Singleton<MyClass>
//{
//}
//
//MyClass::getInstance().doSomething();

template <class T>
class Singleton
{
public:
	static T& getInstance()
	{
		static T _instance;
		return _instance;
	}

	virtual void destroyInstance() {};

	// hide normal/copy/assignment constructors and destructor
protected:
	Singleton() {};
	virtual ~Singleton() {};

private:
	Singleton(Singleton const&);
	Singleton& operator=(Singleton const&);
};


#endif /* singleton_h */