#include "format.h"

size_t Format::intFormat(char *buffer, int num, const char *const operate) {
    int temp = 1;
    int n = 0;
    char c = operate[0];
    bool isNegative = false;

    if(num < 0) { //是负数就加个负号
        buffer[0] = '-';
        buffer++; //指向下一个字符
        num *= -1; //变成非负数
        isNegative = true;
    }

    if(c == '0') { //类似%02d
        c = operate[1] - '0';
        n = c; //记录至少需要的位数
        while(c--) temp *= 10; //计算指定位数
        while(num >= temp) {
            temp *= 10; //指定位数不够，继续增加位数
            n++;
        }
        c = '0'; //不足的补0
    }
    else if(c >= '1' && c <= '9') { //类似%2d
        c -= '0';
        n = c; //记录至少需要的位数
        while(c--) temp *= 10; //计算指定位数
        while(num >= temp) {
            temp *= 10; //指定位数不够，继续增加位数
            n++;
        }
        c = ' '; //不足的补空格
    }
    else { //%d
        while(num >= temp) {
            temp *= 10; //不限定位数，就计算一下位数
            n++;
        }
    }
    
    if(n == 0) n = 1; //至少有1个数
    temp = n;
    if(num == 0) buffer[--n] = '0'; //如果本身就是0就先补一个0
    while(n--) { //从小到大打印每个数字
        buffer[n] = (num ? '0' + num % 10 : c); //num>0就补num的个位数，等于0就补占位符
        num /= 10;
    }

    return temp + (isNegative ? 1 : 0);
}

size_t Format::floatFormat(char *buffer, double num, const char *const operate) {
    int temp = 1;
    int m = 0, n = 0;
    char c = operate[0];
    bool isNegative = false;

    if(num < 0) { //是负数就加个负号
        buffer[0] = '-';
        buffer++; //指向下一个字符
        num *= -1; //变成非负数
        isNegative = true;
    }

    if(c == '.') { //类似%.2f
        c = operate[1] - '0';
        n = c; //记录需要的小数位数
        while(c--) temp *= 10; //计算指定位数
    }
    else { //%f
        c = 6; //不限定小数位数，就默认显示6位小数
        n = c;
        while(c--) temp *= 10;
    }
    c = '0';
    
    int x = ((int)(num * temp * 10) + 5) / 10; //乘temp被并四舍五入后转为整形
    int a = x / temp; //整数部分
    int b = x % temp; //小数部分
    
    temp = 1; //计算整数部分的位数
    while(a >= temp) {
        temp *= 10;
        m++;
    }

    if(m == 0) m = 1; //整数部分至少有1个数
    temp = m;
    if(a == 0) buffer[--m] = '0'; //如果本身就是0就先补一个0
    while(m--) { //从小到大打印整数部分的每个数字
        buffer[m] = ('0' + a % 10); //写入a的个位数
        a /= 10;
    }

    buffer[temp] = '.'; //添加小数点
    buffer += temp + 1; //buffer移到输出小数的位置

    temp += n + 1;
    while(n--) { //从小到大打印小数部分的每个数字
        buffer[n] = (b ? '0' + b % 10 : '0'); //b>0就补b的个位数，等于0就补0
        b /= 10;
    }

    return temp + (isNegative ? 1 : 0);
}

size_t Format::uintFormat(char *buffer, const char *const operate) {
    return size_t();
}

size_t Format::strFormat(char *buffer, char* str, const char *const operate) {
    size_t size = 0;
    while(str[size] != '\0') {
        buffer[size] = str[size];
        size++;
    }
    return size;
}

size_t Format::format(char *str, const size_t maxSize, const char *const format, ...) {
    va_list args;
	va_start(args, format);

	size_t size = vformat(str, maxSize, format, args);

	va_end(args);
    return size;
}

size_t Format::vformat(char *str, const size_t maxSize, const char *const format, va_list args) {
    size_t ssize = 0, fsize = 0, temp = 0;
    char c = 0;

    while (ssize < maxSize && fsize < maxSize && format[fsize] != 0) { //不越界就继续执行
        c = format[fsize];

        if(c != '%') { //不是百分号就直接复制
            str[ssize++] = c;
            fsize++;
            continue;
        }

        fsize++; //略过百分号
        temp = fsize;

        while(1) { //获得数据类型
            c = format[fsize];
            if((c >= '0' && c <= '9') || c == '.' || c == 'l' || c == '-') fsize++;
            else break;
        }

        if(c == 'd') ssize += intFormat(str + ssize, va_arg(args, int), format + temp); //整形
        else if(c == 'f') ssize += floatFormat(str + ssize, va_arg(args, double), format + temp); //浮点
        else if(c == 's') ssize += strFormat(str + ssize, va_arg(args, char*), format + temp); //字符串
        else if(c == '%') str[ssize++] = c; //单纯的百分号

        fsize++; //下一个
    }   
    
    str[ssize] = '\0'; //末尾补0
    return ssize;
}

int Format::toInt(const char *str, size_t* resLength) {
    int res = 0;
    bool negative = false;
    size_t i = 0;
    if(str[i] == '-') {
        negative = true;
        i++;
    }
    for(; str[i] != 0 && str[i] >= '0' && str[i] <= '9'; i++) {
        res = res * 10 + str[i] - '0';
    }
    if(resLength) *resLength = i;
    return negative ? -res : res;
}

float Format::toFloat(const char *str, size_t* resLength) {
    float res = 0;
    bool negative = false;
    size_t i = 0;
    if(str[i] == '-') {
        negative = true;
        i++;
    }
    for(; str[i] != 0 && str[i] >= '0' && str[i] <= '9'; i++) { //整数部分
        res = res * 10 + str[i] - '0';
    }
    if(str[i] == '.') { //小数部分
        i++;
        float tmp = 1;
        for(; str[i] != 0 && str[i] >= '0' && str[i] <= '9'; i++) {
            tmp *= 0.1;
            res += (str[i] - '0') * tmp;
        }
    }
    if(resLength) *resLength = i;
    return negative ? -res : res;
}
