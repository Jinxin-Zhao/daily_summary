#include <cstring>
#include "proxy.h"

template <class T>
RCPtr<T>::RCPtr(T *realPtr):pointee(realPtr){
    init();
}

template <class T>
RCPtr<T>::RCPtr(const RCPtr & rhs):pointee(rhs.pointee){
    init();
}

template <class T>
void RCPtr<T>::init() {
    if(pointee == 0){
        return ;
    }
    if(pointee->isShareable() == false){
        pointee = new T(*pointee);
    }
    pointee->addReference();
}

template <class T>
RCPtr<T> & RCPtr<T>::operator=(const RCPtr &rhs) {
    if(pointee != rhs.pointee){
        if(pointee){
            pointee->removeReference();
        }
        pointee = rhs.pointee;
        init();
    }
    return *this;
}

template <class T>
RCPtr<T>::~RCPtr() {
    if(pointee){
        //in removeReference func,do some destructive actions
        pointee->removeReference();
    }
}

template <class T>
T& RCPtr<T>::operator*() const {
        return *pointee;
}

template <class T>
T* RCPtr<T>::operator->() const {
        return pointee;
}
////////////////////////////////////////////////////////////////
RCObject::RCObject() :refCount(0),shareable(true){
}

RCObject::RCObject(const RCObject &) :refCount(0),shareable(true){
}

RCObject & RCObject::operator=(const RCObject &rhs) {
    return  *this;
}

RCObject::~RCObject(){
}

void RCObject::addReference() {
    ++refCount;
}

void RCObject::removeReference() {
    if(--refCount == 0)
        delete  this;
}

void RCObject::markUnshareable() {
    shareable = false;
}

bool RCObject::isShareable() const {
    return shareable;
}

bool RCObject::isShared() const {
    return refCount > 1;
}


///////////////
String::String(const char * iniValue):value(new StringValue(iniValue)){
}

//String::String(const String & rhs){
//    if(rhs.value->shareable){
//        value = rhs.value;
//        ++value->refCount;
//    }else{
//        value = new StringValue(rhs.value->data);
//    }
//}
//
//String::String &operator=(const String &rhs){
//    if(value == rhs.value){
//        return *this;
//    }
//    if(--value->refCount == 0){
//        delete value;
//    }
//    value = rhs.value;
//    ++value->refCount;
//    return *this;
//}
//
//String::~String(){
//    if(--value->refCount == 0)
//        delete value;
//}

////original version
//const char &String::operator[](int index) const{
//    return value->data[index];
//}
//
////copy-on-write(Lazy rule)
////Providing each call of this nonconst [] is for writing
//char & String::operator[](int index){
//    //if we're sharing a value with other String objects,
//    //break off a separate copy of the value for ourselves
//    if(value->isShared()){
//        value = new StringValue(value->data);
//    }
//    value->markUnshareable();
//    //return a reference to a character inside our unshared StringValue object
//    return value->data[index];
//}

const String::CharProxy String::operator[](int index) const {
    return CharProxy(const_cast<String&>(*this),index);
}

String::CharProxy String::operator[](int index){
    return CharProxy(*this,index);
}

////proxy  class
String::CharProxy::CharProxy(String & str,int index)
        :theString(str),charIndex(index){
}

String::CharProxy::operator char () const{
    return theString.value->data[charIndex];
}
////copy-on-write(Lazy rule)
//each call of this nonconst [] is for writing
String::CharProxy& String::CharProxy::operator=(const CharProxy & rhs){
    if(theString.value->isShared()){
        theString.value = new StringValue(theString.value->data);
    }
    theString.value->data[charIndex] = rhs.theString.value->data[rhs.charIndex];
    return *this;
}

String::CharProxy& String::CharProxy::operator=(char c){
    if(theString.value->isShared()){
        theString.value = new StringValue(theString.value->data);
    }
    theString.value->data[charIndex] = c;
    return *this;
}

const char * String::CharProxy::operator&() const{
    return &(theString.value->data[charIndex]);
}

char * String::CharProxy::operator&(){
    if(theString.value->isShared()){
        theString.value = new StringValue(theString.value->data);
    }
    theString.value->markUnshareable();
    return &(theString.value->data[charIndex]);
}
//////////////
void String::StringValue::init(const char *initValue) {
    data = new char[strlen(initValue) + 1];
    strcpy(data,initValue);
}

String::StringValue::StringValue(const char *initValue){
    init(initValue);
}

String::StringValue::StringValue(const StringValue & rhs){
    init(rhs.data);
}

String::StringValue::~StringValue() {
    delete [] data;
}
