// #include <functional>

// template<typename event_type>
// class i_rule{
// public:
//     virtual ~i_rule() {}
//     virtual int run(event_type * e) = 0;
// };

// template<typename rule_type,typename event_type>
// class base_rule : public i_rule<event_type>
// {
//     typedef std::function<int (event_type * e,int code,const char * err_msg)> event_func_type;
// public:
//     base_rule() : next_func_(NULL),error_func_(NULL){
//     }
//     virtual ~base_rule(){
//         if(next_func_){
//             delete  next_func_;
//         }
//         if(error_func_){
//             delete error_func_;
//         }
//     }
//     template<typename FUNC1,typename FUNC2>
//     void set_func(FUNC1 next_f,FUNC2 error_f){
//         set_next_func(next_f);
//         set_error_func(error_f);
//     }
//     template<typename FUNC>
//     void set_next_func(FUNC f){
//         if(next_func_){
//             delete next_func_;
//         }
//         next_func_ = new event_func_type(f);
//     }
//     template<typename FUNC>
//     void set_error_func(FUNC f){
//         if(error_func_){
//             delete error_func_;
//         }
//         error_func_ = new event_func_type(f);
//     }
//     virtual int run(event_type *e) {
//         const char *msg = "NULL";
//         return ((rule_type *)this)->do_execute(e, &msg);
//     }
//     int execute(event_type *e, int code, const char *err_msg) {
//         if (code != 0) {
//             return do_error(e, code, err_msg);
//         }
//         const char *msg = "NULL";
//         int ret = ((rule_type *)this)->do_execute(e, &msg);
//         if (0 == ret) {
//             return do_next(e, 0, "success");
//         } else {
//             return do_error(e, ret, msg);
//         }
//     }

//     int do_execute(event_type *e, const char **err_msg);

// protected:
//     int  do_next(event_type *e, int code, const char *err_msg) {
//         if (next_func_ == NULL) {
//             //XLOG(XLOG_ERROR, "base_rule::%s, event_id[0X%0X] next_func_ is NULL\n", __FUNCTION__, event_type::ID);
//             return FIN_STAT;
//         } else {
//             return (*next_func_)(e, code, err_msg);
//         }
//     }
//     int  do_error(event_type *e, int code, const char *err_msg) {
//         if (error_func_ == NULL) {
//             //XLOG(XLOG_ERROR, "base_rule::%s, event_id[0X%0X] error_func_ is NULL\n", __FUNCTION__, event_type::ID);
//             return FIN_STAT;
//         } else {
//             return (*error_func_)(e, code, err_msg);
//         }
//     }

// protected:
//     event_func_type *next_func_;
//     event_func_type *error_func_;


// };


#include <iostream>
using namespace std;

template<typename rule_type>
class A
{
public:
    int execute() {
        int ret = ((rule_type *)this)->do_execute();
    }
    //int do_execute();
};

class B : public A<B>
{
public:
    int do_execute() {printf("this is derived class");}
};

class tt
{
public:
    void zhaojx()
    {
        ((test*)this)->a();
    }
    void a();
    int aa;
};
class test : public tt{
public:
    //void a() {printf("this is d\n");}
};