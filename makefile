subsystem:
	make -C backend/asm
	make -C backend/cpu
	make -C frontend
	make -C middleend
	g++ main.cpp -o ./tatarlang


