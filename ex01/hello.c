#include <stdio.h>
#include <string.h>

int main() {
    // 版本1：基础HelloWorld
    printf("Hello, World!\n");

    // 版本2：增加键盘输入功能
    char user_input[100];
    printf("请输入你想说的内容：");
    fgets(user_input, sizeof(user_input), stdin);
    // 去掉输入自带的换行符
    user_input[strcspn(user_input, "\n")] = '\0';
    printf("你输入的内容是：%s\n", user_input);

    return 0;
}
