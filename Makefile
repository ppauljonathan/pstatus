all:
	g++ main.cpp -lamixer -o pstatus
add_to_path:
	sudo mv ./pstatus /usr/bin
clean:
	rm pstatus *.o *.out *.exe