#ifndef PASTEVENTS_H__
#define PASTEVENTS_H__


extern int History_count;
int Remove_Past(const char *filename);
void Add_to_history(char* cmds);
void PastEvents(char** cmd_seq, int cmd_cnt, char* commands);
int Count_lines(const char* file,char* buf);\
void Past_Execute(int index, char* filePath, char* commands);
void replaceSubstring(char *original, const char *target, const char *replacement);

#endif