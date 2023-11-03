给你一个字符串 s ，请你去除字符串中重复的字母，使得每个字母只出现一次。需保证 返回结果的字典序最小（要求不能打乱其他字符的相对位置）
 
示例 1：

输入：s = "bcabc"
输出："abc"
示例 2：

输入：s = "cbacdcbc"
输出："acdb"
 

提示：

1 <= s.length <= 104
s 由小写英文字母组成


//solution 1
### 解题思路：
+ 先遍历一遍字符串，记录一下所有字符出现的次数；
+ 从头开始读取每个字符，每读取一个就将其count减一，代表后面还会有count-1次出现这个字符；
+ 用一个数组标记该字符是否出现在了对的位置（所谓对的位置其实就是说这个字符满足： 到目前为止是第一次出现而且能够使得最终生成的字符串字典序最小）；
+ 如果现在读取到的字符(c)比当前累计的字符串(res)最后一个字符(x)小，and count[x] > 0 (which means we should pop back 'x' and continue to read res.back())
+ 然后接着插入这个最小字符，并且将它标记为在对的位置；

class Solution {
public:
    string removeDuplicateLetters(string s) {
        int count[256] = {0};
        bool is_right[256] = {false};
        string res = "";
        for(auto c : s){
            count[c]++;
        }
        for(auto c : s){
            count[c]--;
            if(is_right[c]){
                continue;
            } else {
                while(count[res.back()] > 0 && c < res.back()){
                    is_right[res.back()] = false;
                    res.pop_back();
                }
                res += c;
                is_right[c] = true;
            }
        }
        return res;

    }
};


//solution 2
### 解题思路（递归贪心算法）
| --- | --- | --- | --- |
|答案字符串	| 待处理字符串 | 第一个唯一字母	| 最小字母 |
| | defcbeabdc | f | d |
| d	| efcbeabc | f | e |
| de |	fcbabc | f | f |
| def |	cbabc | a | a |
| defa | bc	| b | b |
| defab | c	| c | c |
| defabc | | | |	

+ 假想某个“最小字母”左边有唯一的字母，比如CBDDF，B左边有C，C在该字符串中是唯一的。这种情况下，以B作为贪心选择是不合理的（应该选C）。
+ 如果“最小字母”左边的字母，在“最小字母”右边也有它们的重复，那么左边的这些字母完全可以舍去，比如CBABC，A左边的CB可以舍去。
+ 所以字符串s中的“最小字母s[pos]应该满足：
    - s[0 .... pos-1] 中的字母在字符串中不是唯一的；
    - s[pos .....] 中的字母包含了所有在字符串中唯一的字母；
    - 取出 s[pos] ，加入答案字符串中。把右边的子串 s[pos+1...] 中重复的字母 s[pos] 去掉后，对新的字符串重复上述操作。
class Solution {
public:
    string removeDuplicateLetters(string s) {
        if(s.length() == 0) return s;
        int count[26] = {0};
        for(auto i = 0; i < s.size(); ++i){
            count[s[i] - 'a']++;
        }
        int pos = 0;
        for(auto i = 0; i < s.size(); ++i){
            if(s[i] < s[pos])
                pos = i;
            if(--count[s[i] - 'a'] == 0) break;
        }
        string subStrs = s.substr(pos+1,s.length() - pos - 1);
        return s[pos] + removeDuplicateLetters(removeTheWord(subStrs,s[pos]));
    }

private:
    string removeTheWord(string s, char c){
        while(true){
            auto i = s.find(c);
            if(i != -1)
             s.erase(i,1);
            else
             break;
        }
        return s;
    }
};

