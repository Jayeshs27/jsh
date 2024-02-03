#ifndef __WARP_H__
#define __WARP_H__


extern char CurrPath[256];
extern char PrevPath[256];
extern char homePath[256];


bool homeDir(char* homePath, char* currPath);

bool ChangeDir(char* path);

bool Execute(char* Path, char* CurrPath, char* homePath, char* PrevPath);

void Seq_Run(char** cmd_seq, int cmd_cnt);

void warp(char* *cmd_seq, int cmd_cnt);

void warp_Execute(char* cmd);

void PrintDir();

void StorePrev();

void RestorePrev();

#endif