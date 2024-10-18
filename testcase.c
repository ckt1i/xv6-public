#include "types.h"
#include "stat.h"
#include "user.h"

// 获取当前栈指针
uint get_stack_pointer() {
    uint sp;
    asm volatile("movl %%esp, %0" : "=r" (sp));
    return sp;
}

// 深度递归调用以测试更大规模的栈增长
void recursion(int depth) {
    char buffer[4096];  // 分配4KB的空间,确保每次递归占用整页
    memset(buffer, 1 , 4096); // 进行操作避免被优化掉
    uint sp = get_stack_pointer();
    printf(1, "Recursion depth: %d, stack address: 0x%x\n", depth , sp );

    // 在深度达到一定值之前递归
    if (depth < 100) {
        recursion(depth + 1);
    }
}

// 测试深度递归引发栈增长
void test_stack_growth() {
    printf(1, "\n=== Test: Stack Growth ===\n");
    printf(1, "Starting recursion test...\n");

    recursion(1);  // 初始递归调用,测试栈增长

    printf(1, "Stack recursion test completed.\n");
}

// 测试堆与栈的冲突
void test_stack_heap_collision() {
    printf(1, "\n=== Test: Stack-Heap Collision ===\n");
    printf(1, "Starting stack-heap collision test...\n");
    printf(1, "Allocating 222MB of memory on the heap...\n");
    int * p = (int *)malloc(222*1024*1024 - 512*1024);
    *p = 1;

    printf(1, "Making recursions .\n");
    recursion(1);  // 初始递归调用,测试栈增长
    printf(1, "Stack recursion test completed.\n");
}

int main(int argc, char *argv[]) {
    test_stack_growth();
    test_stack_heap_collision();
    exit();
}