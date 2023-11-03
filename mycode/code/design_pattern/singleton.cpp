#include <iostream>
#include <stdio.h>
using namespace std;


/*
template <typename T>
class  Singleton
{
public:
    struct object_creator
    {
        object_creator()
		{
			cout<<"create"<<endl;
			Singleton<T>::instance();
		}
        inline void do_nothing()const {}
    };
	//static object_creator * getcreator() { return &create_object;}
    static object_creator create_object;

public:
    typedef T object_type;
    static object_type * instance()
    {
        static object_type obj;
        //据说这个do_nothing是确保create_object构造函数被调用
        //这跟模板的编译有关
        //create_object.do_nothing();
        return &obj;
    }

};
template <typename T>
typename Singleton<T>::object_creator Singleton<T>::create_object;

class QMManager
{
public:
	QMManager() {};
    ~QMManager(){};
    friend class Singleton<QMManager>;
public:
    void do_something(){};
	void print() {cout<<"this is QMManger"<<endl;}
};

int main()
{
    Singleton<QMManager>::instance()->print();

    return 0;
}

void funcA()
{
    Singleton<QMManager>::getcreator();
}
*/

class QMSqlite
{
protected:
    static QMSqlite instance_;
    QMSqlite() {printf("QMSqlite constructor\n");}
    ~QMSqlite(){};
public:
    static QMSqlite *instance()
    {
        return &instance_;
    }
    void do_something() {printf("QMSqlite do_something\n");}
};


class QMManager
{
protected:
    static QMManager instance_;
    QMManager()
    {
        printf("QMManager constructor\n");
        QMSqlite::instance()->do_something();
    }
    ~QMManager(){};
public:
    static QMManager *instance()
    {
        return &instance_;
    }
};


QMManager QMManager::instance_;
QMSqlite QMSqlite::instance_;

int main()
{
    return 0;
}













