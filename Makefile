all:
	g++ main.cpp -o pstatus
add_to_path:
	sudo mv ./pstatus /usr/bin
clean:
	rm pstatus *.o *.out *.exe