## Unix MultiThread Programing
### Simple multi-producer & one consumer
- consumer start consuming items after all the producers have joined already



- consumer start consuming while all the producers are producing the items


when threads run the func, the local variable in func has a copy int every thread,the things you need to do is to synchronize the global varible in case that there are lots of threads modifying the global varible