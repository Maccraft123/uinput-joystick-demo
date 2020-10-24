default:
	rm fw-input || true
	gcc main.c -o fw-input -O2 -Wall -Wextra -Wpedantic
