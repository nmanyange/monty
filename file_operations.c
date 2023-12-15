#include "monty.h"

/**
 * open_file - opens a file
 * @file_name: the file namepath
 * Return: void
 */

void open_file(char *file_name)
{
	FILE *fd = fopen(file_name, "r");

	if (file_name == NULL || fd == NULL)
		err(2, file_name);

	read_file(fd);
	fclose(fd);
}


/**
 * read_file - reads a file
 * @fd: pointer to file descriptor
 * Return: void
 */

void read_file(FILE *fd)
{
	int line_number, format = 0;
	char *buffer = NULL;
	size_t len = 0;

	for (line_number = 1; getline(&buffer, &len, fd) != -1; line_number++)
	{
		format = parse_line(buffer, line_number, format);
	}
	free(buffer);
}


/**
 * parse_line - Separates each line into tokens to determine
 * which function to call
 * @buffer: line from the file
 * @line_number: line number
 * @format:  storage format. If 0 Nodes will be entered as a stack.
 * if 1 nodes will be entered as a queue.
 * Return: Returns 0 if the opcode is stack. 1 if queue.
 */

int parse_line(char *buffer, int line_number, int format)
{
	char *opcode, *value;
	const char *delim = "\n ";

	if (buffer == NULL)
		err(4);

	opcode = strtok(buffer, delim);
	if (opcode == NULL)
		return (format);
	value = strtok(NULL, delim);

	if (strcmp(opcode, "stack") == 0)
		return (0);
	if (strcmp(opcode, "queue") == 0)
		return (1);

	find_func(opcode, value, line_number, format);
	return (format);
}

/**
 * find_func - find the appropriate function for the opcode
 * @opcode: opcode
 * @value: argument of opcode
 * @format:  storage format. If 0 Nodes will be entered as a stack.
 * @line_number: line number
 * if 1 nodes will be entered as a queue.
 * Return: void
 */
void find_func(char *opcode, char *value, int line_number, int format)
{
	int j;
	int flagg;

	instruction_t func_list[] = {
		{"push", add_to_stack},
		{"pall", print_stack},
		{"pint", print_top},
		{"pop", pop},
		{"nop", nop},
		{"swap", swap},
		{"add", add_nodes},
		{"sub", sub_nodes},
		{"div", div_nodes},
		{"mul", mul_nodes},
		{"mod", mod_nodes},
		{"pchar", print_char},
		{"pstr", print_str},
		{"rotl", rotl},
		{"rotr", rotr},
		{NULL, NULL}
	};

	if (opcode[0] == '#')
		return;

	for (flagg = 1, j = 0; func_list[j].opcode != NULL; j++)
	{
		if (strcmp(opcode, func_list[j].opcode) == 0)
		{
			call_fun(func_list[j].f, opcode, value, line_number, format);
			flagg = 0;
		}
	}
	if (flagg == 1)
		err(3, line_number, opcode);
}


/**
 * call_fun - Calls the required function.
 * @func: Pointer to the function that is about to be called.
 * @op: string representing the opcode.
 * @value: string representing a numeric value.
 * @line_number: line number for the instruction.
 * @format: Format specifier. If 0 Nodes will be entered as a stack.
 * if 1 nodes will be entered as a queue.
 */
void call_fun(op_func func, char *op, char *value, int line_number, int format)
{
	stack_t *node;
	int flagg;
	int j;

	flagg = 1;
	if (strcmp(op, "push") == 0)
	{
		if (value != NULL && value[0] == '-')
		{
			value = value + 1;
			flagg = -1;
		}
		if (value == NULL)
			err(5, line_number);
		for (j = 0; value[j] != '\0'; j++)
		{
			if (isdigit(value[j]) == 0)
				err(5, line_number);
		}
		node = create_node(atoi(value) * flagg);
		if (format == 0)
			func(&node, line_number);
		if (format == 1)
			add_to_queue(&node, line_number);
	}
	else
		func(&head, line_number);
}
