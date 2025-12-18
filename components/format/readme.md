简化板字符串格式化类，目前只实现了对整型和字符串的部分格式化

无依赖项

示例：

```c++
size_t len = Format::format(stringBuffer, maxStringSize, "num: %d", 123); //将格式化后的字符串"num: 123"存入stringBuffer，最多允许存maxStringSize个字符，返回存入的字符数
```