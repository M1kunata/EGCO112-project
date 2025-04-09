compile: compafun.cpp
	 g++ compafun.cpp -o company

run: company
	 ./company


clean: company
	 rm company