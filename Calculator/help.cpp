void helptext() {
	cout << "\n\nSimple calculator ver0.2 help.";
	cout << "\n\nType in expressions (i.e. 5+5) followed by the Enter key.";
	cout << "\nMultiple expressions separated by ; are also allowed (i.e. 5+5; 10-7;)";
	cout << "\n\nEnter ; to recover from any errors.";
	cout << "\n\nKeywords:";
	cout << "\n--------";
	cout << "\nhelp		- provide help";
	cout << "\nquit		- end program";
	cout << "\nsqrt		- perform square root (i.e. sqrt(6+7);)";
	cout << "\npow		- perform power (i.e. pow(5))";
	cout << "\n!		- calculate factorial (i.e. !4)";
	cout << "\n#		- set a variable (i.e. # boo = 20)";
	cout << "\n~		- change previously set variable (i.e. ~ boo = boo +20)";
	cout << "\nconst	- set constant variable (i.e. const foo = 3.764)";
	cout << "\n\nProvided constants are pi and e\n";
}