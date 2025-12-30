#include<windows.h>

void foo(void) { // Function signature or prototype
	OutputDebugStringA("Hello World!\n");
}

int CALLBACK WinMain(
	HINSTANCE hInstance,      // 程序“本身”的句柄
	HINSTANCE hPrevInstance,  // 历史遗留参数
	LPSTR     lpCmdLine,      // 命令行参数（不含程序名的一个字符串）
	int       nShowCmd        // 窗口初始显示方式
) {
	// CALLBACK HINSTANCE LPSTR _In_ 等都是Win系统自定义的宏定义
	// _In_ 告诉编译系统该参数为输入参数
	// 注意要使用WinMain函数需要设置项目 属性 链接器 系统 子系统：窗口
	// 否则改用main即可
	foo();
}