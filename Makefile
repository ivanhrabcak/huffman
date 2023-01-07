all: 
	gcc -g -Wall -o huffman src/main.c src/array_list.c src/binary_buffer.c src/binary_tree.c src/huffman.c

clean: 
	$(RM) huffman