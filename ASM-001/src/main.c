#include <stdio.h>

int add(int a, int b) { return a + b; }

int main() {
	int (*binary_fn)(int, int) = add;
	int ans					   = binary_fn(5, 5);

	printf("%d\n", ans);
	return 0;
}
