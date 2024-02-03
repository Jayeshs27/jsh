[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description:
## Execute Instruction: 
1 .To Run the shell 
>  Run 'make' then './a.out'.
2. To Delete the executable 
>  Run 'make clean'.
3. I have Implement all the specifications mention in the Project and Added their codes in respective files.
4. For pastevents All the history will store in file 'pastevents.txt'.

## Assumptions
1. For Input I have assume following:
    * Max size of input in prompt : 4096 byte
    * Max number of commands present in input separated by ; or & : 128
    * Max size of each command : 256 byte 
    * Max Words in each Command : 64
    * Max size of each word : 128 byte
    * Max sizeo of Path (for warp, peek, seek instructions) : 256 byte
2. If there are multiple commands taking time greater that 2s then i have printed in following syntax.
>  &lt; first command &gt; &lt;time taken by all commands together &gt;
3. To execute *warp* instruction '/' should not present at start of the path except in case of absolute path.
4. To execute Background Processes provided commands and there should be valid.In case of invalid commands or arguments shell will not print Error message.
5. To execute *echo* command string is not required enclosed in " ".
6. In *seek* the target string should not contain any extension link .txt, .c etc. 
> for example to find main.c only main is sufficient
5. Only in case of command containing *pastevents execute* it should not contain any extra spaces ( for pastevents execute part only ).

