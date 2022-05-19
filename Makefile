all:
	g++ main.cpp -lasound -o pstatus
add_to_path:
	sudo mv ./pstatus /usr/bin
clean:
	rm pstatus *.o *.out *.exe