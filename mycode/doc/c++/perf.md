从你提供的 `top` 输出看，`monrisk` 进程占用了 **118.3%** 的 CPU，这明显是异常的。以下是排查步骤和建议：

## 1. **实时监控进程状态**
```bash
# 监控该进程的线程
top -H -p 1031513

# 或者使用更好的工具
htop -p 1031513
```

## 2. **分析线程堆栈**
```bash
# 查看所有线程
ps -L -p 1031513

# 获取线程的堆栈信息
pstack 1031513

# 或者使用 gdb 获取更详细的信息
gdb -p 1031513
(gdb) thread apply all bt
(gdb) detach
```

## 3. **使用 perf 进行性能分析**
```bash
# 记录性能数据（运行30秒）
sudo perf record -p 1031513 -g -- sleep 30

# 生成报告
sudo perf report

# 实时查看热点函数
sudo perf top -p 1031513
```

## 4. **查看系统调用**
```bash
# 跟踪进程的系统调用
sudo strace -p 1031513 -c -T -f

# 查看具体的系统调用
sudo strace -p 1031513 -f -e trace=file,network
```

## 5. **内存分析**
```bash
# 查看内存映射
pmap -x 1031513

# 查看内存统计
cat /proc/1031513/status | grep -E "Vm|Rss"
```

## 6. **检查日志文件**
```bash
# 查看应用日志
find / -name "*monrisk*" -type f 2>/dev/null | xargs grep -l "error\|exception" 2>/dev/null

# 查看系统日志
sudo journalctl -f -n 100
dmesg | tail -50
```

## 7. **常见问题类型判断**

### **CPU 使用率高可能的原因：**
1. **死循环** - 最常见的原因
2. **频繁的 GC**（如果是 Java/Python）
3. **锁竞争** - 多线程程序
4. **大量计算** - 算法复杂度问题
5. **频繁的 I/O 等待**（但你的 `wa` 为0，排除磁盘I/O）

## 8. **特定语言的调试**

### 如果是 **Java** 程序：
```bash
jstack 1031513 > stack.txt
jstat -gcutil 1031513 1000 10
```

### 如果是 **Python** 程序：
```bash
# 安装 py-spy 后使用
sudo py-spy top --pid 1031513
sudo py-spy dump --pid 1031513
```

### 如果是 **C/C++** 程序：
```bash
# 使用 addr2line 分析地址
addr2line -e /path/to/binary [地址]
```

## 9. **快速诊断脚本**
创建一个诊断脚本：
```bash
#!/bin/bash
PID=1031513

echo "=== 进程基本信息 ==="
ps -fp $PID

echo -e "\n=== 线程统计 ==="
ps -L -p $PID | wc -l

echo -e "\n=== 打开文件 ==="
ls -l /proc/$PID/fd 2>/dev/null | head -20

echo -e "\n=== 内存映射 ==="
pmap $PID | tail -5

echo -e "\n=== 运行时间 ==="
ps -p $PID -o etime=
```

## 10. **应急处理**

如果进程影响系统稳定性：
```bash
# 降低优先级
sudo renice 19 -p 1031513

# 限制CPU使用（需要 cgroups）
sudo systemd-run --scope -p CPUQuota=50% -p CPUShares=512

# 最后手段：重启进程
sudo kill -TERM 1031513  # 优雅关闭
sudo kill -9 1031513     # 强制关闭
```

## 建议的排查顺序：
1. **先用 `top -H` 查看是哪个线程占用高**
2. **用 `pstack` 或 `gdb` 获取堆栈**
3. **用 `perf` 分析热点函数**
4. **检查应用日志和配置**

请先执行第1、2步，把结果贴出来，我可以帮你进一步分析具体是代码中的哪部分有问题。
