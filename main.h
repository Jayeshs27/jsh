#ifndef __MAIN_H__
#define __MAIN_H__


int main();

void Sep_cmd_Execute(char** single_cmd, int ct, char* commands, int Is_bg_flag,  bool* pastevents_flag, bool* fg_flag);

bool Separate(char* commands);


#endif
