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

private:
	_node_type ** _M_Bucket;
	int           _M_CurrBucket;
	_node_type *  _M_Node;
	int           _M_Size;
};





#endif
