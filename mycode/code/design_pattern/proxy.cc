#include "proxy.h"


String::String(const char * iniValue)
    :value(new StringValue(iniValue)){

}

String::String(const String & rhs){
    if(rhs.value->shareable){
        value = rhs.value;
        ++value->refCount;
    }else{
        value = new StringValue(rhs.value->data);
    }
}

String::String &operator=(const String &rhs){
    if(value == rhs.value){
        return *this;
    }
    if(--value->refCount == 0){
        delete value;
    }
    value = rhs.value;
    ++value->refCount;
    return *this;
}

String::~String(){
    if(--value->refCount == 0)
        delete value;
}

const char &String::operator[](int index) const{
    return value->data[index];
}

//copy-on-write(Lazy rule)
//Providing each call of this nonconst [] is for writing
char &operator[](int index){
    //if we're sharing a value with other String objects,
    //break off a separate copy of the value for ourselves
    if(value->refCount > 1){
        --value->refCount;
        value = new StringValue(value->data);
    }
    value->shareable = false;
    //return a reference to a character inside our unshared StringValue object
    return value->data[index];
}

////proxy  class
//const String::CharProxy String::operator[](int index) const {
//    return CharProxy(const_cast<String&>(*this),index);
//}

//String::CharProxy String::operator[](index){
//    return CharProxy(*this,index);
//}

String::CharProxy::CharProxy(String & str,int index)
        :theString(str),charIndex(index){

}

String::CharProxy::operator char () const{
    return theString.value->data[charIndex];
}

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
String::StringValue::StringValue(const char *initValue)
    : refCount(1),shareable(true) {
    data = new char[strlen(initValue) + 1];
    strcpy(data, initValue);
}

String::StringValue::~StringValue() {
    delete [] data;
}
