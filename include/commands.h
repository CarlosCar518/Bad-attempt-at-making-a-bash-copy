#ifndef COMMANDS_H
#define COMMANDS_H

#define STOP 0
#define RUNNING 1
#define SUCCESS 0

typedef void (*cmd_fn)(void);

char* return_fixed_current_path();

typedef struct {
    char* buff;
    size_t pos;
    size_t cursor;
}flow_struct;

void dir_complete(flow_struct* st);

void command_print_arg();
void command_remove();
void command_print_serie();
void command_print_message();
void command_print_currentWorkingDir();
void command_change_dir();
void command_make_dir();
void command_remove_dir();
void command_help();
void command_listFiles();
void command_print_date();
void command_clear();
void _Smain();
void command_readFile();
void command_execute();
void command_vim();



#endif 