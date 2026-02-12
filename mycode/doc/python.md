[toc]

# install python3.14 without gil(multithread) in anaconda
]$ conda create -n py314_no_gil -c conda-forge python=3.14 python-freethreading

# qlib installation

```python
   # install these components before qlib installation
   ]$ pip install cvxpy dill fire gym joblib jupyter lightgbm loguru matplotlib mlflow nbconvert pyarrow pymongo python-redis-lock pyyaml redis ruamel.yaml tqdm

   # install torch
   ]$ # 最新的稳定版
   pip install torch torchvision torchaudio
```

---
# project configuration
如果您希望继续使用当前代码中的导入方式（不使用 `trading.` 前缀），您需要确保 `trading` 目录本身在 Python 路径中，而不是它的子目录。让我详细说明解决方案：

## **问题根源**
您当前的导入方式是：
```python
from core.bus import EventBus
from definitions.events import OrderRequest
```
Python 会查找 `sys.path` 中的每个目录，寻找 `core/` 文件夹。

## **解决方案**

### **方法1：正确的PYTHONPATH设置**
将 `trading` 目录添加到 PYTHONPATH，而不是它的子目录：

```bash
# 错误（您的当前设置）：
PYTHONPATH = D:/trading/; D:/trading/core; D:/trading/definitions

# 正确（应该这样设置）：
PYTHONPATH = D:/trading
```

**原理**：Python 在 `D:/trading` 中找到 `core/` 目录，然后可以导入 `core.bus`。

### **方法2：在PyCharm中配置（推荐）**

1. **将项目标记为源根目录**：
   - 在PyCharm项目窗口中，右键点击 `trading` 文件夹
   - 选择 `Mark Directory as` → `Sources Root`
   
   ![PyCharm标记源根目录](https://resources.jetbrains.com/help/img/idea/2023.3/mark_directory_as.png)

2. **或者配置项目结构**：
   - `File` → `Settings` → `Project: [项目名]` → `Project Structure`
   - 选中 `trading` 目录，点击上方的 `Sources` 按钮（或右键标记为Sources）
   
3. **配置运行/调试设置**：
   - `Run` → `Edit Configurations`
   - 在 `Environment variables` 中添加：
     ```
     PYTHONPATH=D:/trading
     ```
   - 或者在 `Working directory` 设置为 `D:/trading` 的父目录

### **方法3：创建 `.pth` 文件（最可靠）**

1. 找到您的 Python 解释器的 site-packages 目录：
   - 在 PyCharm 的 Python Console 中运行：
   ```python
   import site
   print(site.getsitepackages())
   ```

2. 通常路径是：
   ```
   D:/ProgramData/anaconda3/Lib/site-packages/
   ```

3. 在该目录下创建一个名为 `trading.pth` 的文件，内容为：
   ```
   D:/trading
   ```

4. 重启 PyCharm

### **方法4：在代码中动态添加路径**

在 `unit_test` 的每个文件开头添加：

```python
import sys
import os

# 获取当前文件的绝对路径
current_file = os.path.abspath(__file__)
# 获取unit_test目录
unit_test_dir = os.path.dirname(current_file)
# 获取trading目录
trading_dir = os.path.dirname(unit_test_dir)

# 将trading目录添加到Python路径
if trading_dir not in sys.path:
    sys.path.insert(0, trading_dir)

# 现在可以使用您的原始导入方式
from core.bus import EventBus
from definitions.events import OrderRequest, OrderValidated, TickEvent, OrderRiskRejected
from definitions.objects import Tick
from modules.base_module import BaseModule
```

### **方法5：创建 `__init__.py` 文件并配置 PyCharm**

1. **确保每个目录都是包**：
   ```
   trading/
   ├── __init__.py        # 新增
   ├── core/
   │   ├── __init__.py    # 应该已有
   │   └── bus.py
   ├── definitions/
   │   ├── __init__.py    # 应该已有
   │   ├── events.py
   │   └── objects.py
   ├── xtquant/
   │   ├── __init__.py    # 新增
   │   └── xtconstant.py
   ├── modules/
   │   ├── __init__.py    # 应该已有
   │   └── base_module.py
   └── unit_test/
       ├── __init__.py    # 新增
       └── 您的文件.py
   ```

2. **在 PyCharm 中重新加载项目**：
   - `File` → `Invalidate Caches and Restart...`

### **方法6：使用环境激活脚本**

对于 Anaconda，创建一个激活脚本：

1. 创建文件：`D:/ProgramData/anaconda3/etc/conda/activate.d/trading_path.bat`
2. 内容：
   ```batch
   @echo off
   set PYTHONPATH=D:/trading;%PYTHONPATH%
   ```

3. 重新打开 Anaconda Prompt 或重启 PyCharm

### **验证配置是否生效**

在您的 `unit_test` 文件中添加调试代码：

```python
import sys
print("当前 Python 路径:")
for path in sys.path:
    print(f"  {path}")

# 检查是否能导入
try:
    from core.bus import EventBus
    print("✓ 导入 core.bus 成功")
except ImportError as e:
    print(f"✗ 导入失败: {e}")
    
    # 手动检查路径
    import os
    print(f"\n检查 trading 目录: D:/trading")
    print(f"目录存在: {os.path.exists('D:/trading')}")
    print(f"core 目录存在: {os.path.exists('D:/trading/core')}")
    print(f"bus.py 存在: {os.path.exists('D:/trading/core/bus.py')}")
```

### **最简解决方案**

1. **在 PyCharm 中**：
   - 右键 `trading` 文件夹 → `Mark Directory as` → `Sources Root`
   - 重启 PyCharm

2. **如果还不行**：
   - 在 `unit_test` 文件开头添加：
   ```python
   import sys
   sys.path.insert(0, r'D:/trading')
   ```

### **为什么您之前的配置无效**

您设置了：
```
PYTHONPATH = D:/trading/; D:/trading/core; D:/trading/definitions
```

当 Python 在 `D:/trading/core` 中查找 `core.bus` 时：
1. 它会在 `D:/trading/core` 目录下找 `core` 文件夹 → 找不到
2. 所以无法导入 `core.bus`

**正确思路**：Python 需要在 `D:/trading` 中找 `core` 文件夹，然后找 `bus.py`。

选择最简单的方法开始尝试，通常方法2（在PyCharm中标记为Sources Root）是最简单有效的。


# conda configuration in linux
conda config --set auto_activate_base false
+ create new env
]$ conda create -n uat_env python=3.13
+ activate uat_env
]$ conda activate uat_env

# 如果项目目录有enviroment.yml
conda env update -f environment.yml
# 如果项目提供的是requirements.txt
pip install -r requirements.txt

# 1. Python Base

## python 下划线五种含义
(https://zhuanlan.zhihu.com/p/36173202)
### 单前导下划线：_var
+ 下划线前缀的含义是告知其他程序员：以单个下划线开头的变量或方法仅供内部使用。 该约定在PEP 8中有定义。相当于私有变量命名。

### 单末尾下划线：var_
+ 有时候，一个变量的最合适的名称已经被一个关键字所占用。 因此，像class或def这样的名称不能用作Python中的变量名称。 在这种情况下，你可以附加一个下划线来解决命名冲突。


### 双前导下划线：__var
+ 双下划线前缀会导致Python解释器重写属性名称，以避免子类中的命名冲突。这也叫做名称修饰(name mangling),解释器更改变量的名称，以便在类被扩展的时候不容易产生冲突。

### 双前导和末尾下划线：__var__
+ 也许令人惊讶的是，如果一个名字同时以双下划线开始和结束，则不会应用名称修饰。 由双下划线前缀和后缀包围的变量不会被Python解释器修改.这些dunder方法通常被称为神奇方法 - 但Python社区中的许多人（包括我自己）都不喜欢这种方法。
最好避免在自己的程序中使用以双下划线（“dunders”）开头和结尾的名称，以避免与将来Python语言的变化产生冲突。


### 单下划线：_


## List
### List遵循左闭又开的区间原则： 比如
  ```python
  planets = ['Mercury', 'Venus', 'Earth', 'Mars', 'Jupiter', 'Saturn', 'Uranus', 'Neptune']
  planets[1:-1]    #All the planets except the first and last .  [1, end)
  ```
### Lists are "mutable"
  ```python
  planets[3] = 'Malacandra'
  ```
### functions
+ len(planets), sorted(planets), sum(planets),min(planets),max(planets)
  ```python
    list.append()
    list.pop() # removes and returns the last element of a list:
    list.index('Earth') # get the index of Earth.   2

    # Is Earth a planet?
    "Earth" in planets    # it will return True
  ```

## Tuples
### The syntax for creating them uses parentheses instead of square brackets
  ```python
  t = (1,2,3) 
  t = 1,2,3   # equivalent to above

  ```
### The Tuple is immutable
+ the as_integer_ratio() method of float objects returns a numerator(分子) and a denominator(分母) in the form of a tuple
  ```python

    x = 0.125
    x.as_integer_ratio() 
    # output
    # (1,8)
    
    numerator, denominator = x.as_integer_ratio()
    print(numerator / denominator)
    # output 
    # 0.125
  ```

## String
### immutable
### '' & ""
+ string single quote and double quotes
    ```python
        print("Pluto's a planet!")
        print('My dog is named "Pluto"')
    ```
+ functions
  ```python
    claim = "Pluto is a planet!"
    claim.upper()
    # output
    # 'PLUTO IS A PLANET!'
    claim.lower()

    # Searching for the first index of a substring
    claim.index('plan') # output 11
  ```

## Dictionaries
+ Dictionaries are a built-in Python data structure for mapping keys to values.
  ```python
    numbers = {'one':1, 'two':2, 'three':3}
  ```

## call function
+ __call__ is used to implement forward in "nn.model",该方法的功能类似于在类中重载 () 运算符，使得类实例对象可以像调用普通函数那样，以“对象名()”的形式使用。作用：为了将类的实例对象变为可调用对象。

  ```python
    class Module(nn.Module):
      def __init__(self):
        super().__init__()
        # ......
      
      def forward(self, x):
        # ......
        return x
    
    # input data
    data = ....

    model = Module()

    model(data)

    # the clause above is equal to 
    model.forward(data)


  ##### example #####
  class Student:
    def __call__(self, param):
      print('call function, param type {}, value {}'.format(type(param), param))
      res = self.forward(param)
      return res

    def forward(self, input_):
      print("forward called")
      return input_

    a = Student()
    input_param = a('data')
    print('the input param: ', input_param)
  ```

# 2. Python Library

## numpy
+ astype will always create a new array(a copy),even if the dtype is the same as before
  ```python
  In[]: int_array = np.arrange(10)
  In[]: calibers = np.array([.22,.27,.357])

  In[]: int_array.astype(calibers.dtype)
  Out[]: array([0., 1., 2., ...., 10.])
  ```
+ slice is a view of original array, modifications on the slice will take effect in the original array
  ```python
  In[]: np.arrange(10)

  In[]: arr[5:8]
  Out[]: array([5,6,7])

  In[]: arr[5:8] = 12
  In[]: arr
  Out[]: array([0,1,2,3,4,12,12,12,8,9])
  ```
+ bool index
+ magic index: always copy data to a new array
+ np slice:
```python
    # We create a 4 x 5 ndarray that contains integers from 0 to 19
X = np.arange(20).reshape(4, 5)

# We print X
print()
print('X = \n', X)
print()

# We select all the elements that are in the 2nd through 4th rows and in the 3rd to 5th columns
Z = X[1:4,2:5]

# We print Z
print('Z = \n', Z)

# We can select the same elements as above using method 2
W = X[1:,2:5]

# We print W
print()
print('W = \n', W)

# We select all the elements that are in the 1st through 3rd rows and in the 3rd to 5th columns
Y = X[:3,2:5]

# We print Y
print()
print('Y = \n', Y)

# We select all the elements in the 3rd row
v = X[2,:]

# We print v
print()
print('v = ', v)

# We select all the elements in the 3rd column
q = X[:,2]

# We print q
print()
print('q = ', q)

# We select all the elements in the 3rd column but return a rank 2 ndarray
R = X[:,2:3]

# We print R
print()
print('R = \n', R)
```
### numpy sum
+ sum不传参数时是所有元素总和。
+ sum的axis参数
    ```python
        def sum(self, axis=None, dtype=None, out=None, keepdims=False, initial=0, where=True): # real signature unknown; restored from __doc__
        """
        a.sum(axis=None, dtype=None, out=None, keepdims=False, initial=0, where=True)
        
            Return the sum of the array elements over the given axis.
        
            Refer to `numpy.sum` for full documentation.
        
            See Also
            --------
            numpy.sum : equivalent function
        """
        pass
    
    # example
    import numpy as np
    a = np.array([[[1,2,3,2], [1,2,3,1], [2,3,4,1]], [[1,0,2,0], [2,1,2,0],[2,1,1,1]]])
    # out:
    array([[1,2,3,2],
           [1,2,3,1],
           [2,3,4,1]],
           [[1,0,2,0],
            [2,1,2,0],
            [2,1,1,1]])
    # 如何理解axis，axis是精确定位某个元素需要经过多少数组的长度，如定位第一个元素a[0][0][0], 一般来讲a[n0][...][n],则axis就是n。
    # 如果sum函数的axis取其中某一个值，则代表这个维度已经确定，举例来说axis=0,则sum就是a[0][n1][n2]+a[1][n1][n2].
    a.sum(axis=0)
    # output
    array([[2,2,5,2],
    [3,3,5,1],
    [4,4,5,2]])

    # example 2
    # 如果是二维数组
    ar.sum(axis=1,keepdims=True),计算每一行的和
    ```

## Pandas
### Series
+ Series is a object like 1d Array with data labels(index).
  ```python
  from pandas import Series, DataFrame

  ####
  In[]: obj = pd.Series([4,7,-5,3])
  In[]: obj
  Out[]: 
  0   4
  1   7
  2   -5
  3   3
  dtype: int64

  ####
  In[]: obj.values
  Out[]: array([4,7,,-5,3])

  In[]: obj.index
  Out[]: RangeIndex(start=0,stop=4,step=1)

  ####
  In[]: obj_2 = pd.Series([4,7,-5,3],index=['d','b','a','c'])

  In[]: obj_2
  Out[]: 
  d   4
  b   7
  a   -5
  c   3
  dtype: int64

  In[]: obj_2.index
  Out[]: Index(['d','b','a','c'],dtype='object')

  ####
  sdata = {'Ohio': 35000, 'Texas': 71000,'Oregon': 16000}
  obj3 = pd.Series(sdata)

  In[]: obj3
  Out[]:
  Ohio    35000
  Oregon  16000
  Texas   71000
  dtype: int64
  # key is sorted by direction order

  In[]: states = ['California','Ohio','Texas']
  In[]: obj4 = pd.Series(sdata,index=states)
  In[]: obj4
  Out[]:
  California  NaN
  Ohio        35000
  Texas       71000

  In[]: pd.isnull(obj4) # or obj4.isnull()
  Out[]:
  California   True
  Ohio         False
  Texas        False
  dtype: bool
  ```

### DataFrame
+ DataFrame is a table type DS,it has a series of ordered columns with different types of values(numpy,string,bool etc.)
+ DataFrame has both row index and column index.
  ```python
  data = {'state': ['Ohio','Ohio','Ohio','Nevada','Nevada','Nevada'], 'year': [2000,2001,2002,2001,2002,2003],'pop': [1.5,1.7,3.6,2.4,2.9,3.2]}

  In[]: frame = pd.DataFrame(data)
  In[]: frame
  Out[]: 
       pop   state   year
  0    1.5   Ohio    2000
  1    1.7   Ohio    2001
  2    3.6   Ohio    2002
  3    2.4   Nevada  2001
  4    2.9   Nevada  2002
  5    3.2   Nevada  2003

  ####
  In[]: frame.head()  # the first 5 rows
  ```
### DataFrame and Series arithmetic
+ the arithmetic between DataFrame and Series will match by DataFrame's column and Series's index.
  ```python
  frame = pd.DataFrame(np.arrange(6.).reshape(2,3),columns=list('bde'),index=['Utah','Ohio'])

  In[]: series = frame.iloc[0]

  In[]: frame
  Out[]: 
       b    d    e
  Utah 0.0  1.0  2.0
  Ohio 3.0  4.0  5.0

  In[]: series
  Out[]: 
  b  0.0
  d  1.0
  e  2.0
  Name: Utah, dtype: float64

  In[]: frame - series
  Out[]:
       b    d    e
  Utah 0.0  0.0  0.0
  Ohio 3.0  4.0  5.0

  ```

## matplotlib
### Figure
  ```python
  fig = plt.figure()  # an empty figure with no Axes
  fig, ax = plt.subplots() # a figure with a single Axes
  fig, axs = plt.subplots(2,2) # a figure with a 2x2 grid of Axes
  ```
### Axes
+ This is what you think of as 'a plot', it is the region of the image with the data space. A given figure can contain many Axes, but a given Axes object can only be in one Figure.
+ The Axes contains 2(or 3 in the case of 3D) Axis(single) objects.

### Types of inputs to plotting functions
  ```python
  # to convert a pandas.DataFrame
  a = pandas.DataFrame(np.random.rand(4,5), columns = list('abcde'))
  a_asarray = a.values

  # to convert a numpy.matrix
  b = np.matrix([1,2],[3,4])
  b_asarray = np.asarray(b)
  ```





### 3 Ways to deal with missing values
#### A Simple Option: Drop Columns with Missing Values
+ The simplest option is to drop columns with missing values.
+ Unless most values in the dropped columns are missing, the model loses access to a lot of (potentially useful!) information with this approach. As an extreme example, consider a dataset with 10,000 rows, where one important column is missing a single entry. This approach would drop the column entirely!
#### A Better Option: Imputation
+ The imputed value won't be exactly right in most cases, but it usually leads to more accurate models than you would get from dropping the column entirely.
#### An Extension To Imputation
+ In this approach, we impute the missing values, as before. And, additionally, for each column with missing entries in the original dataset, we add a new column that shows the location of the imputed entries.

### Categorical Value
+ a categorical variable takes only a limited number of values.
+ Consider a survey that asks how often you eat breakfast and provides four options: "Never", "Rarely", "Most days", or "Every day". In this case, the data is categorical, because responses fall into a fixed set of categories.
+ If people responded to a survey about which what brand of car they owned, the responses would fall into categories like "Honda", "Toyota", and "Ford". In this case, the data is also categorical.
#### Three Approaches
##### Drop Categorical Variables
+ The easiest approach to dealing with categorical variables is to simply remove them from the dataset. This approach will only work well if the columns did not contain useful information.
##### Ordinal Encoding:
+ Ordinal encoding assigns each unique value to a different integer.This assumption makes sense in this example, because there is an indisputable ranking to the categories. Not all categorical variables have a clear ordering in the values, but we refer to those that do as ordinal variables. For tree-based models (like decision trees and random forests), you can expect ordinal encoding to work well with ordinal variables.
##### One-Hot Encoding
+ One-hot encoding creates new columns indicating the presence (or absence) of each possible value in the original data. Thus, you can expect this approach to work particularly well if there is no clear ordering in the categorical data (e.g., "Red" is neither more nor less than "Yellow"). We refer to categorical variables without an intrinsic ranking as nominal variables.
+ One-hot encoding generally does not perform well if the categorical variable takes on a large number of values (i.e., you generally won't use it for variables taking more than 15 different values).
###### Calculator how many entries are added to the dataset by replacing the column with a one-hot encoding?
+ example: consider a dataset with 10,000 rows, and containing one categorical column with 100 unique entries.
+ begin by calculating many entries are needed to encode the categorical variable(by multiplying the number of rows by the number of columns in the one-hot encoding),then substract the number of entries in the original column.


### Cross-Validation
+ imagine you have a dataset with 5000 rows. You will typically keep about 20% of the data as a validation dataset, or 1000 rows. But this leaves some random chance in determining model scores. That is, a model might do well on one set of 1000 rows, even if it would be inaccurate on a different 1000 rows.
+ At an extreme, you could imagine having only 1 row of data in the validation set. If you compare alternative models, which one makes the best predictions on a single data point will be mostly a matter of luck!
+ In general, the larger the validation set, the less randomness (aka "noise") there is in our measure of model quality, and the more reliable it will be. Unfortunately, we can only get a large validation set by removing rows from our training data, and smaller training datasets mean worse models!
+ In cross-validation, we run our modeling process on different subsets of the data to get multiple measures of model quality.
#### given these tradeoffs, when should you use each approach?
+ For small datasets, where extra computational burden isn't a big deal, you should run cross-validation.
+ For larger datasets, a single validation set is sufficient. Your code will run faster, and you may have enough data that there's little need to re-use some of it for holdout.


+ In general, a small learning rate and large number of estimators will yield more accurate XGBoost models, though it will also take the model longer to train since it does more iterations through the cycle. As default, XGBoost sets learning_rate=0.1


### Data leakage
+ Data leakage (or leakage) happens when your training data contains information about the target, but similar data will not be available when the model is used for prediction. This leads to high performance on the training set (and possibly even the validation data), but the model will perform poorly in production.

In other words, leakage causes a model to look accurate until you start making decisions with the model, and then the model becomes very inaccurate.

There are two main types of leakage: target leakage and train-test contamination.

#### target leakage
+ To prevent this type of data leakage, any variable updated (or created) after the target value is realized should be excluded.(比如y代表“是否得肺炎”，但是训练数据里有一列是“吃抵抗药物”，因为抵抗药物这一栏会很频繁得更新，吃了之前和之后都会对y得结果造成影响，所以这一类得列应该排除掉),这一列和结果y具有强相关性，比如信用卡花费金额， y表示持有信用卡，一般来说如果未持有信用卡，那么这个列“花费金额”将会是0，如果持有信用卡，那么数据集里面花费金额为0的概率只有0.02，所以最后的结果准确性会很高。

#### Train-Test Containmination
+ A different type of leak occurs when you aren't careful to distinguish training data from validation data.
