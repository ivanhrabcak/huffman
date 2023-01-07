all: 
	gcc -g -Wall -o huffman src/main.c src/data_structures/array_list.c src/data_structures/binary_buffer.c src/data_structures/binary_tree.c src/huffman.c

clean: 
	$(RM) huffman