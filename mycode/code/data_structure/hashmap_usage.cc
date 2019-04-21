#ifndef __HASH_MAP__
#define __HASH_MAP__

class HashInt{
public:
	int operator()(const int key) const{
		return key;
	}
};

class HashString{
public:

};


template<class _Key,class _Tp>
class CHashNode{
public:
	_Key _M_Key;
	_Tp  _M_Value;
	CHashNode * _M_next;
};

template <class _Key,class _Tp>
class CHashMapIterator{
public:
	typedef CHashNode<_Key,_Tp> _node_type;
	typedef CHashMapIterator<_Key,_Tp> _Self;
	typedef _Tp  & reference;
	typedef _Tp  * pointer;

	CHashMapIterator(_node_type ** _Bucket,int _Size){
		_M_Bucket = _Bucket;
		_M_Size   = _Size;
		_M_CurrBucket = 0;
		for(;_M_CurrBucket < _Size;_M_CurrBucket++){
			_M_Node = _M_Bucket[_M_CurrBucket];
			if(_M_Node != NULL){
				break;
			}
		}
	}
	CHashMapIterator(){
		_M_Bucket = NULL;
		_M_Node   = NULL;
		_M_CurrBucket = _M_Size;
	}
	reference operator*() const {return _M_Node->_M_Value;}

	_Key & GetKey() {return _M_Node->_M_Key;}

//why & 
	_Self & operator++(){

	}
	_Self operator++(int){
		_Self __tmp = *this;
		++*this;
		return __tmp;
	}

	inline bool IsEnd(){
		return _M_CurrBucket == _M_Size;
	}

private:
	_node_type ** _M_Bucket;
	int           _M_CurrBucket;
	_node_type *  _M_Node;
	int           _M_Size;
};

template <class _Key,class _Tp,class _HashFcn>
class CHashMap{
public:
	typedef CHashMapIterator<_Key,_Tp> iterator;
	typedef CHashNode<_Key,_Tp> _node_type;

	CHashMap(int _Size = 53){
		_M_Bucket = new _node_type*[_Size];
		_M_Size   = _Size;
		Clear();
	}
	virtual ~CHashMap(){
		delete _M_Bucket;
	}
	int GetPos(const _Key & key){

	}
	void Insert(const _Key & key,const _Tp & value){

	}
	_Tp * Find(const _Key &key){

	}
	void Erase(const _Key & key){

	}
	iterator Begin(){
		return iterator(_M_Bucket,_M_Size);
	}
	_node_type * Alloc(){

	}
	int GetCount(){

	}
	void Clear(){

	}
private:
	_HashFcn _M_hash;
	_node_type ** _M_Bucket;
	deque<_node_type> _M_Link;
	_node_type * _M_Free;
	int _M_Size;
	int _M_Count;
};



#endif
