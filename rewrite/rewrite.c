#include <unistd.h>
#include <termios.h>

struct test;
void init(unsigned int test){
	write(STDOUT_FILENO, "Hello.", 6);
}

typedef struct {
	void (*init)(unsigned int test);
} test;

int main() {
	test.init(100);

	return 0;
}
