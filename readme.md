# C0文法小型编译器
>程序结构
- 首先，编译器的设计根据编译的总体流程而定，首先是词法分析部分。
- 词法分析部分内容在symbol.c和symbol.h文件中进行相应分析。
- 词法分析之后，将词法分析部分得到的token传递给语法分析程序，然后语法分析 程序根据相应的token进行相应的语法分析，构建符号表。
- 在语法分析的过程中，利用语法制导翻译，生成相应的四元式代码，利用四元式生成相应的中间代码。
- 将四元式进行优化之后，生成相应的汇编代码。

- 程序运行流程
- 语法分析部分的函数主要根据文法来进行，语法分析部分主要采用递归下降子程序法进行相应的值求解，所以语法成分中每一个非终结符号都可以作为相应的函数。
- 一些简单的非终结符可以不作为特殊函数处理，如加法运算符，乘法运算符等，因为词法分析已经能够对简单的语法单元进行相应的识别。
- 而比较复杂的语法单元如：
**<赋值语句> ：：= <标识符>=<表达式> | <标识符>`[`<表达式>`]` = 表达式**
其中则可以将赋值语句生成相应的函数来进行递归下降分析，
- 其中标识符因为可以被词法分析单元所识别，所以无需用特定的函数来递归下降分析，然后表达式较为复杂，所以表达式需要生成相应特定的程序来进行相应的分析。
- 所以该文法用到了两个相应的函数，分别用来递归下降分析赋值语句和递归下降分析表达式，其中在递归下降分析赋值语句的过程中调用了递归下降分析表达式的方法。
- 语义分析的函数功能主要是语法制导翻译过程中所需的函数，其中这些函数大部分嵌入在递归下降分析语法过程中，典型的语义分析函数有四元式的生成，符号表元素的插入。
- 完成了语法分析之后，有相应的中间代码输出函数以及对四元式进行相应优化的代码函数。
最后可以调用相应的mips代码生成函数。
