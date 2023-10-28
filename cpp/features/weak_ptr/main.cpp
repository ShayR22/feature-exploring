#include <memory>
#include <stdio.h>
#include <unistd.h>
#include <thread>

class A {
public:
	A() {printf("ctor\n");}
	~A() {printf("dtor\n");}
	void set_value(int x) { _x = x; }
	int get_value() { return _x; }
private:
	int _x{42};
};

std::shared_ptr<A> a = std::make_shared<A>();
std::weak_ptr<A> b = a;


void owner_function() {
	sleep(5);
	printf("object killing with ref count of %ld\n", a.use_count());
	a.reset();
}

void non_owner_function() {
	for (int i = 0; i < 10; i++) {
		auto shared_ptr = b.lock();
		if (shared_ptr) {
			shared_ptr->set_value(i);
			printf("ref_count = %ld, update value to %d\n", shared_ptr.use_count(), shared_ptr->get_value());
		} else {
			printf("no value\n");
		}
		sleep(2);
		printf("ref_count = %ld\n", shared_ptr.use_count());
	}
}

void weak_ptr_test() {
	std::jthread owner_thread(owner_function);
	std::jthread non_owner_thread(non_owner_function);
}

void shared_ptr_test() {
	std::shared_ptr<int> a;
	std::shared_ptr<int> b = a;
	a = std::make_shared<int>(42);
	if (b) {
		printf("value, ref_count = %ld\n", b.use_count());
	} else {
		printf("no value, ref_count = %ld\n", b.use_count());
	}
}

int main() {
//	weak_ptr_test();
	shared_ptr_test();
	return 0;
}
