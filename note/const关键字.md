# const关键字

## 常量指针

指向字符串常量的指针，pString为变量，*pString为常量。

```c++
//两种写法作用相同
const char *pString1;
char const *pString2;
```

## 指针常量

指向字符串(变量或者常量)的指针常量，pString为常量，*pString为变量或者常量。

```c++
char* const pString;
```

## 指向常量的指针常量

指向字符串常量的指针常量，pString为常量，*pString为常量。

```c++
const char *const pString;
```

## 类常量成员

```c++
class ClassName
{
public:
	const int a;
	const int b;
	static int c
	static const int d;
	//类成员常量只能通过构造函数的初始化成员列表来赋值
	ClassName(int x) : a(1), b(x)
	{
	}
};

//静态常量成员赋值必须与静态成员保持一致，在类定义外部进行
static int ClassName::c = 3;
static const int ClassName::d = 4;
```

## 类常量成员函数

```c++
class ClassName2
{
public:
	//类常量成员函数的声明与定义
	void func() const
	{
		//类常量成员函数不能修改类的成员变量，并且调用本类的成员函数也必须为常量成员函数。
	}
};
```

## 常量类实例对象

常量类实例对象的定义，常量对象只能被调用其类的常量成员函数。

```c++
const ClassName2 obj;
```
