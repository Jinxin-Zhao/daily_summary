### 求数组中未出现的最小正整数
+ Description
  给定一个无序整型数组，找到数组中未出现的最小正整数。(不考虑利用空间或者排序)
  要求: 不使用额外空间，时间复杂度要求为O(N)。
  Example:
  Input: [-1,2,3,4] output: 1
  Input: [1,2,3,4] output: 5
+ Solution:
  一个数组未出现的最小正整数范围是 [1,N+1]（N是数组的长度）。

  + 最优情况下的数组如下（也是我们在从左到右遍历数组的情况下不断交换想要达到的理想状态）
  ```cpp
  index:         0 1 2 3 4
  array[index]:  1 2 3 4 5

  可以发现这是我们需要调整数组最终想要达到的状态： array[index] = index + 1;
  最小未出现的正整数范围是 [1,6]，显而易见最小未出现的正整数是6。
  
  ```
  + 数组元素异常情况处理：
  第一种： arr[left] <= left
  ```cpp
  [-1,2,3,4]
  arr[0] <= 0 (should be 1);
  ```
  第二种： arr[left] > r
  这里的r = arr.length(作为这个范围窗口右边界，当前面left索引遇到异常情况的值时，这个r会减1，并且会将r位置的数赋值到index位置。意味着可表示的最小正整数范围变成[left+1,r-1]
  ```cpp
  [10,2,3,4]
  arr[0] > 4
  ```
  第三种： arr[left] = arr[arr[left] - 1] （两个值一样的元素）
  ```cpp
  [1,2,2,4]
  arr[2] == arr[2 - 1] == 2
  ```
  + 下面从一个具体的例子来看:
  ```cpp
  [1,10,9,7,2,3,4]

  终止条件： l < r

  left从下标0开始，r = 7(一共7个元素)
  第一步遇到了arr[0] == 0 + 1,l继续向右遍历 
  [  l           ]
  [1,10,9,7,2,3,4]
  此时遇到了10 > r = 7,是异常数字，可以排除掉。
  此时做的操作是将r位置的值放到l位置，然后r向左移动(--r),此时 r = 6;
  [  l         r]
  [1,4,9,7,2,3,4]

  此时发现4是符合正常范围的正整数,在区间[1,7]里面，此时就会将4放到它应该出现的地方，将它和位置3（arr[arr[l] - 1]）的值互换
  [  l         r]
  [1,7,9,4,2,3,4]

  此时会判断出l所在位置的值7 > r == 6，因此会将r所在位置的值放到l位置,--r,值变成5
  [  l       r  ]
  [1,3,9,4,2,3,4]

  此时l位置的值3属于范围[1 + 1,7 - 1],将这个值放到对的位置,和l==2位置的值互换
  [  l       r  ]
  [1,9,3,4,2,3,4]

  此时检测到l位置的数9不属于[2,6],将arr[--r]（arr[4]）值赋到l位置
  [  l     r    ]
  [1,2,3,4,2,3,4]

  此时的l位置满足arr[l] == l + 1;
  l向右边移动,发现接下来的3和4都满足，直接到下面这种情况
  [      l r    ]
  [1,2,3,4,2,3,4]
  
  此时l和r相遇了，此时由于arr[l] == l + 1 (arr[3] == 3+1 == 4)
  我们会走到l++,此时l变成了4，所以最小未出现的正整数是5。
  ```

+ Implementation:
  ```cpp
  int LeastNum(int * arr,int length){
    int left = 0;
    int r = length;

    while(left < r){
        int t = arr[left];
        if(arr[left] == left + 1){
            left ++;
        } else if (arr[left] <= left || arr[left] > r || arr[arr[left] - 1] == arr[left]){
            arr[left] = arr[--r];
        } else {
            swap(arr,left,arr[left] - 1);
        }
    }
    return left + 1;
    }
  ```
  

### [分治思想] 最大子数组问题(算法导论P38)
+ Description:
  你被准许在某个时刻买进某一公司股票，并在之后某个日期将其卖出（买进卖出都是在当天交易结束后进行）需要实现利益最大化。
+ Solution:
  - 我们首先想到的其实就是暴力列举出所有的日期组合，只需要满足日期在买入日期之后即可。n天中有(n,2)种日期组合，大约时间复杂度是$O(n^2)$
  - 第二种方法就是转变问题：我们考察每日价格的变化，第 $i$ 天的价格变化定义为第 $i$ 天和第 $i-1$天的价格差。问题就转化为寻找变化后的数组 A [1,2,....,n-1]的最大非空连续字数组（这种子数组不一定唯一），当然只有在数组中包含负数的时候，最大子数组问题才有意义（否则最大子数组不就是整个数组）。
   那我们如何求最大子数组，下面介绍一种动态规划方法:
    Example: 
    Input: [-3,4,-1,2,-6,6,-1]
    output: [4,-1,2] sum = 5;

    - 不过我们的状态转移方程怎么写尼，或许我们会想到这样定义：
    array[0,....i]中的最大子数组和为dp[i].那么我们可以用dp[i]推出dp[i+1]吗？
    由于最大子数组必然是要连续的，按照对dp的定义，并不能保证array[0...i]中的最大子数组与array[i+1]一定是相邻的，所以由我们定义的dp[i]并不能推出dp[i+1]。
    重新定义如下：$以array[i]为结尾的最大子数组和为dp[i]$
    此时dp[i]有两种选择，要不和前面的相邻子数组连接，形成一个更大的子数组；要不就直接自成一派，以当前的元素array[i]作为下面最大子数组的开始（这种情况代表了前一天买，第二天就卖掉），所以状态转移方程可以这么写：$
    dp[i] = max(array[i],dp[i-1] + array[i])$;

    ```cpp
    int FindMaxSubArray(int * array,int length){
        int n = length;
        if(n == 0) return 0;
        std::unique_ptr<int> dp(new int[n]);
        dp[0] = array[0];
        for(int i = 1; i < n; ++i){
            dp[i] = (array[i] > dp[i-1] + array[i]) ? array[i] : dp[i-1] + array[i];
        }
        int result = 0;
        for(int i = 0; i < n; ++i){
            result = (result > dp[i]) ? result : dp[i];
        }
        return result;
    }
    ```

### 去除重复字母（leetcode 316)
+ Description:
  给定一个字符串s，请去除字符串中重复的字母，使每个字母只出现一次。需保证返回结果的字典序最小（要求不能打乱其他字符相对位置）
  example 1:
    input: s = "bcabc"
    output: "abc"
  example 2:
    input: s = "cbacdcbc"
    output: "acdb"
+ Solution:
  贪心方法：用两个数组: vector<int> cnt(用于计字母出现数),vector<bool> in_right_place;
  我们拿样例: "bcabc"
  + 先遍历一遍字符串并对字母进行计数: cnt['a']=1,cnt['b']=2;cnt['c']=2;
  + 再次从头遍历，当读到第一个字符b时，cnt['b']--,代表未来会出现b的次数；
  in_right_place判断正确位置：每次读到一个未在正确位置上的字符，就可以将这个字符拿来和res比较字典序，如果能发现存在一个新的字符串使得字典序更小则代替。 具体还可以看样例：
  当读到‘a’的时候，此时res='bc'
  =>'a'比‘c’字典序优先而且‘c’的计数cnt['c']>0,代表未来还会出现,所以弹出c并撤销‘c’是正确位置的标记: in_right_place['c']=false,此时res="b";
  =>'a'比'b'字典序优先且'b'未来还会出现(cnt['b']>0),所以弹出b并撤销‘b’是正确位置的标记: in_right_place['b']=false,此时res="";

  ```cpp
  class Solution{
    public:
      string removeDuplicateLetters(string s){
        int cnt[256]={0};
        bool in_right_place[256]={false};
        string res = "";
        for (auto c : s)
          cnt[c]++;
        for(auto c : s){
          cnt[c]--;
          if(in_right_place[c]){
              continue;
          }
          else {
              while(cnt[res.back()] > 0 && c < res.back()){
                in_right_place[res.back()] = false;
                res.pop_back();
              }
              res += c;
              in_right_place[c] = true;
          }
        }
        return res;
      }
  };
  ```