#include <iostream>
#include <stdio.h>
using namespace std;

// #define LENGTH 60*60*24*365*300UL

// int main()
// {
// 	printf("LENGTH is %ld\n",LENGTH);
// 	return 0;
// }

// static inline int fls(int x)
// {
// 	int position;
// 	int i ;
// 	if(0 != x)
// 	{
// 		for(i = (x >> 1),position = 0;i != 0;++position)
// 		{
// 			i >>= 1;
// 		}
// 	}
// 	else
// 	{
// 		position = -1;
// 	}
// 	return position + 1;
// }

// static inline unsigned int roundup_pow_of_two(unsigned int x)
// {
// 	return 1UL << fls(x - 1);
// }

static inline bool is_power_of_2(uint32_t n)
{
	return (n != 0 && (n & (n-1)) == 0);
}

static inline uint32_t roundup_pow_of_two(uint32_t a)
{
	if(a == 0){
		return 0;
	}
	uint32_t position = 0;
	for(int i = a;i != 0;i>>=1){
		position++;
	}
	return static_cast<uint32_t>(1<<position);
}

struct kfifo
{
	uint8_t * buffer;
	uint32_t in;
	uint32_t out;
	uint32_t size;  //must be 2^

	kfifo(uint32_t _size)
	{
		if(!is_power_of_2(_size)){
			_size = roundup_pow_of_two(_size);
		}

		buffer = new uint32_t[_size];
		in = 0;
		out = 0;
		size = _size;
	}

	uint32_t put(const uint8_t * data,uint32_t len)
	{
		//current left space
		len = min(len,size - in + out);
		
		//the position in to the end of buffer
		auto l = min(len,size - (in & (size -1)));

		memcpy(buffer + (in & (size - 1)), data,l);

		//copy the left data if they exist
		memcpy(buffer,data+l,len -l);

		in += len;

		return len;
	}

	uint32_t get(uint8_t * data,uint32_t len)
	{
		len = min(len,in - out);
	}


};



int main()
{
	unsigned int r = 4294967298;  //unsigned int max = 4294967295
	//r is  2(loop)
	cout<<r<<endl;

    double a = (1.0*7/9);
    cout<<a<<endl;

	return 0;
}
