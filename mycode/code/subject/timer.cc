#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define HEAP_SIZE   100
typedef struct Heap_t Heap_t;
typedef struct Timer_t Timer_t;
typedef struct Timer_Ctl_t Timer_Ctl_t;
typedef void (*cb_func)(void *, Timer_Ctl_t*, void *);

struct Heap_t{
  int size;
  int buffer[HEAP_SIZE];
};

struct Timer_t{
  int expire;
  cb_func cb;
};

struct Timer_Ctl_t{
  Heap_t heap;
  Timer_t timer;
};

/*初始化*/
void Timer_Init(Timer_Ctl_t *timer_ctl)
{
  timer_ctl->heap.size = 0;
  timer_ctl->timer.expire = 0;
}

/*插入数据到堆中*/
int Insert_Heap(Heap_t *heap, int value)
{
  int size = heap->size;
  if(size == HEAP_SIZE)
  {
    printf("This Heap is full\n");
    return 1;
  }
  heap->size ++;
  heap->buffer[size] = value;
  if(size > 0)
  {
    int i = size;
    int j = (i-1)/2;
    while(j >= 0 && i > 0)
    {
        if( heap->buffer[j] <= value)
            break;
        else
        {
            heap->buffer[i] = heap->buffer[j];
            i = j;
            j = (i-1)/2;
        }
    }
    heap->buffer[i]=value;
  }
  return 0;
}

/*删除堆顶数据*/
int Delete_Heap(Heap_t *heap)
{
  int parent,child = 1;
  if(heap->size == 0)
  {
    printf("This Heap is Empty\n");
    return 1;
  }
  heap->size --;
  int size = heap->size;
  for(parent = 0; parent*2+1 < size ; parent = child)
  {
    child = 2 * parent + 1;
    if(child + 1 < size)
    {
      if(heap->buffer[child] > heap->buffer[child+1])
        child ++;
    }
    if(heap->buffer[size] < heap->buffer[child])
      break;
    else
      heap->buffer[parent] =  heap->buffer[child];
  }
  heap->buffer[parent] = heap->buffer[size];
  return 0;
}

/*获取当前时间*/
time_t Get_Current_Time()
{
  return time(NULL);
}

/*获取定时时间*/
time_t Parse_Expire_Time(int delay)
{
  return time(NULL)+delay;
}

/*启动定时器*/
void Ev_Timer_Start(int Timeout_s,cb_func cb,Timer_Ctl_t *timer_ctl)
{
  int expire = Parse_Expire_Time(Timeout_s);
  timer_ctl->timer.cb = cb;
  Insert_Heap(&timer_ctl->heap,expire);
}

/*定时器回调函数*/
void Test_CallBack_Func(void *arg1, Timer_Ctl_t *timer_ctl , void *arg2)
{
  static int timeout = 0;
  printf("Hello World\n");
  timeout ++;
  Ev_Timer_Start(timeout,Test_CallBack_Func,timer_ctl);
}

/*定时器运行*/
void Timer_Run(Timer_Ctl_t *timer_ctl)
{
  int size = timer_ctl->heap.size;
  if(size == 0)
    return ;
  time_t now = Get_Current_Time();
  timer_ctl->timer.expire = timer_ctl->heap.buffer[0];
  if(timer_ctl->timer.expire <= now)
  {
    printf("now:%d\n",(int)now);
    if(timer_ctl->timer.cb)
      timer_ctl->timer.cb(NULL,timer_ctl,NULL);
    Delete_Heap(&timer_ctl->heap);
  }
  usleep(20000);
}

int main()
{
  Timer_Ctl_t timer_ctl;
  Timer_Init(&timer_ctl);
  Ev_Timer_Start(2,Test_CallBack_Func,&timer_ctl);
  while(1)
  {
    Timer_Run(&timer_ctl);
  }
  return 0;
}