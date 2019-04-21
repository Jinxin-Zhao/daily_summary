### rule 4 & 8 [page:19 & 38]
+ new operator(normal new) & operator new
+ placement new 

  + new operator:
    - malloc raw memory(call operator new function)
    - set default value of Type
  + operator new:
    function ptototype:
    ```
    void * operator new(size_t size);
    [attention]:you can override this function,when you call new like this: 
    string *p = new string(),it'll call the 'operator new(size_t size)' you override, 
    ```
  + relationship between "new operator" & "operator new":
    ```
     string * p = new string("memory");
     devided into next 3 processes:
       1. void * pmemory = operator new(sizeof(string));
       2. call string::string("memory") on *pmemory;
      [attention]: only compiler can do this action,so if you want to new sth,you can only use new operator or placement new mentioned afterwards.
       3. string * ps = static_cast<string*>pmemory;
  + placement new
    - if you want to execute the 2th action above
    ```
    #include <new>
    //buffer(raw memory) has already been allocated;
    //but what if buffer is allocated by malloc()!!!
    string * constructS(void * buffer,int widgetSize){
        return new (buffer) string(widgeSize);
    }
    ```
  + deletion & deallocation(to avoid resource leak:)
    - new & delete 
    ```
    string * ps = new string();
    ...
    delete ps;
    [1.ps->~string();]
    [2.operator delete(ps);]
    ===========================

    void * buffer = operator new(50*sizeof(char));
    ...
    operator delete(buffer);
    ```
  + operator new[] & operator delete[](array new):
    ```
    string * ps = new string[10];
    ```
+ ===================================
  + don't support default constructor unless it's necessary
  ```
  class EquipmentPiece{
      public:
      EquipmentPrice(int IDNumber);
      ...
    
    int main(){
        EquipmentPiece bestP[10]; //error,no matched ctors

        typedef EquipmentPiece * PEP;
        PEP bestPiece[10]; //OK,no need to call ctors;
        PEP * bestPieces = new PEP[10]; //OK

        for(int i = 0;i < 10; ++i){
            bestPieces[i] = new EquipmentPiece(i);
        }
    }
  }

  1. remember to delete all the objects that elements in bestPieces points to;
  such as:
  for(int i = 0;i < 10; ++i){
    delete bestPieces[i];    
  } 
  otherwise it can cause memory leak(if you just call "delete [] bestPieces");
  2. you need spare space to restore pointers and objects they point to;
   ```

   + How to optimize:
    ```
    void * rawmemory = operator new [](10*sizeof(EquipmentPiece));
    EquipmentPiece * bestPieces = static_cast<EquipmentPiece *>(rawmemory);
    for(int i = 0;i < 10; ++i){
        new (&bestPieces[i]) EquipmentPiece(ID number);
    }
    ....

    //remember this
    for(int i = 9;i >=0;++i){
        bestPieces[i].~EquipmentPiece();
    }
    operator delete [](rawmemory);

    delete [] bestPieces; //undefined error
    ```


