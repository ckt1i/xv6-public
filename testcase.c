#include "types.h"
#include "stat.h"
#include "user.h"
#include "memlayout.h"

// 获取当前栈指针
uint get_stack_pointer() {
    uint sp;
    asm volatile("movl %%esp, %0" : "=r" (sp));
    return sp;
}

// 测试栈是否位于堆之上
void test_stack_position() {
    printf(1, "\n=== Test 1: Stack Position ===\n");
    uint sp = get_stack_pointer();
    uint heap_end = (uint)sbrk(0);

    printf(1, "Current stack pointer: 0x%x\n", sp);
    printf(1, "Heap end: 0x%x\n", heap_end);

    if (sp > heap_end) {
        printf(1, "Test passed: Stack is above heap.\n");
    } else {
        printf(1, "Test failed: Stack is not correctly positioned.\n");
    }
}

// 测试堆和栈是否分离
void test_heap_stack_separation() {
    printf(1, "\n=== Test 2: Heap and Stack Separation ===\n");

    uint heap_start = (uint)sbrk(0);
    sbrk(4096 * 10);  // 增加10页堆
    uint heap_end = (uint)sbrk(0);
    uint sp = get_stack_pointer();

    printf(1, "Heap range: 0x%x - 0x%x\n", heap_start, heap_end);
    printf(1, "Stack pointer: 0x%x\n", sp);

    if (sp > heap_end) {
        printf(1, "Test passed: Stack is above heap after sbrk.\n");
    } else {
        printf(1, "Test failed: Stack and heap overlap.\n");
    }
}

// 递归调用以强制栈增长
void recursive_call(int depth) {
    char buffer[1024];  // 在栈上分配空间，逐步增长栈
    printf(1, "Recursion depth: %d, buffer address: 0x%x\n", depth, buffer);

    // 递归调用，强制栈增长
    if (depth < 200) {
        recursive_call(depth + 1);
    }
}

// 测试栈增长
void test_stack_growth() {
    printf(1, "\n=== Test 3: Stack Growth ===\n");
    printf(1, "Testing stack growth with recursion...\n");
    recursive_call(1);  // 初始调用

    printf(1, "Test completed.\n");
}

// 深度递归调用以测试更大规模的栈增长
void deep_recursion(int depth) {
    char buffer[4096];  // 分配4KB的空间，确保每次递归占用整页
    printf(1, "Recursion depth: %d, buffer address: 0x%x\n", depth, buffer);

    // 在深度达到一定值之前递归
    if (depth < 200) {
        deep_recursion(depth + 1);
    }
}

// 测试深度递归引发栈增长
void test_deep_stack_growth() {
    printf(1, "\n=== Test 4: Deep Stack Growth ===\n");
    printf(1, "Starting deep recursion test...\n");

    deep_recursion(1);  // 初始递归调用，测试栈增长

    printf(1, "Deep recursion test completed.\n");
}

// 强制堆增长以接近栈区域
void force_stack_heap_collision() {
    // 通过增加堆的大小来接近栈区域
    sbrk((KERNBASE - (uint)sbrk(0)) / 2);  // 增加到接近栈的区域

    char buffer[1024];  // 在栈上分配空间
    printf(1, "Allocated stack buffer at: 0x%x\n", buffer);
}

// 测试堆与栈的冲突
void test_stack_heap_collision() {
    printf(1, "\n=== Test 5: Stack-Heap Collision ===\n");

    force_stack_heap_collision();  // 强制堆增长，接近栈

    printf(1, "Test completed.\n");
}

int main() {
    // 执行所有测试
    test_stack_position();         // 测试栈位置
    test_heap_stack_separation();  // 测试堆与栈的分离
    test_stack_growth();           // 测试栈增长
    test_deep_stack_growth();      // 测试深度递归栈增长
    test_stack_heap_collision();   // 测试堆与栈的冲突

    exit();
}
