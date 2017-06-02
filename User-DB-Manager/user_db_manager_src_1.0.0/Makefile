GDC_INCL = .
GDC_LIB  = .

LIB_PATHS   = -L$(GDC_LIB)


CC=gcc

all: user_db_manager test_db_manager

user_db_manager:  main.o db_management.o file_parser.o pdf_generator.o plot_graph.o gnuplot_interface.o zmq_server.o
	$(CC) -o user_db_manager main.o db_management.o file_parser.o pdf_generator.o gnuplot_interface.o plot_graph.o  zmq_server.o \
	-lzmq -ljson-c -lsqlite3 -lhpdf -lm -lpthread -lstdc++ 

test_db_manager: zmq_client.o
	$(CC) -o test_db_manager zmq_client.o -lzmq -ljson-c -lm -lpthread -lstdc++

zmq_client.o: zmq_client.c
	$(CC) -g -c zmq_client.c

plot_graph.o: plot_graph.c plot_graph.h
	$(CC)  -c plot_graph.c

gnuplot_interface.o: gnuplot_interface.c gnuplot_interface.h
	$(CC) -c gnuplot_interface.c

pdf_generator.o: pdf_generator.c pdf_generator.h
	 $(CC) -c pdf_generator.c

main.o: main.c db_management.h file_parser.h 
	$(CC) -g -c main.c

file_parser.o:file_parser.c file_parser.h
	$(CC)  -g -c file_parser.c

db_management.o: db_management.c db_management.h
	$(CC) -c db_management.c

zmq_server.o: zmq_server.c zmq_server.h
	$(CC) -g -c zmq_server.c

# --- clean ---
clean:
	rm -f *.o user_db_manager test_db_manager

# --- install ---
install:
	cp ./user_db_manager /usr/local/bin/

#creating user_db_config directory to store the DB Management file and calibration data config file.

	cp ./user_db_mgr_config.conf /usr/local/bin/user_db_config

# moving startup script and conf file in init directory

	chmod +x ./lib/user_db_manager.sh
	cp ./lib/user_db_manager.sh /etc/init.d/
	sudo update-rc.d user_db_manager.sh remove
	sudo update-rc.d -f user_db_manager.sh defaults 99 01
	
# --- uninstall ---
uninstall:	
	rm /etc/init.d/user_db_manager.sh
	sudo update-rc.d user_db_manager.sh remove
	rm /usr/local/bin/user_db_manager
	
