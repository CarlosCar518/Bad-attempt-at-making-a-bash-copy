#ifndef COMMANDS_H
#define COMMANDS_H

#define STOP 0
#define RUNNING 1
#define SUCCESS 0

typedef void (*cmd_fn)(void);

void command_exit();
void command_print_arg();
void command_remove();
void command_print_serie();
void command_print_message();
void command_wd();
void command_change_dir();
void command_make_dir();
void command_remove_dir();
void command_help();
void command_listFiles();
void command_print_date();
void command_joke();
void command_clear();

#endif 