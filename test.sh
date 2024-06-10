#!/bin/bash

echo -e "Adding aliases:"
echo -e "alias eep='echo Eep!'"
alias eep='echo "Eep!"'
echo -e "alias expr='expr 5 + 5'"
alias expr='expr 5 + 5'
echo -e "alias cat='cat /etc/hostname'"
alias cat='cat /etc/hostname'
echo -e "alias ls='ls /usr/bin'"
alias ls='ls /usr/bin'
echo -e "alias myalias1='echo My Alias 1'"
alias myalias1='echo "My Alias 1"'
echo -e "alias myalias2='echo My Alias 2'"
alias myalias2='echo "My Alias 2"'
echo -e "alias myalias3='echo My Alias 3'"
alias myalias3='echo "My Alias 3"'
echo -e "All aliases added."

echo -e "\nWhat will happen:"
echo -e "List all aliases currently set using the alias command"
echo -e "Execute the respective alias command"
echo -e "Remove the alias using 'unalias'"

echo -e "\nTest 1:"
echo -e "List aliases:"
alias
echo -e "Execute expr alias:"
expr
echo -e "Remove expr alias:"
unalias expr
alias

echo -e "\nTest 2:"
echo -e "List aliases:"
alias
echo -e "Execute cat alias:"
cat
echo -e "Remove cat alias:"
unalias cat
alias

echo -e "\nTest 3:"
echo -e "List aliases:"
alias
echo -e "Execute ls alias:"
ls
echo -e "Remove ls alias:"
unalias ls
alias

echo -e "\nTest 4:"
echo -e "List aliases:"
alias
echo -e "Execute echo alias with a test message:"
echo -e "This is a test message."
echo -e "Remove echo alias that not exist need to print ERR:"
unalias echo
alias

echo -e "\nTest 5:"
echo -e "List aliases:"
alias
echo -e "Execute myalias1 alias:"
myalias1
echo -e "Remove myalias1 alias:"
unalias myalias1
alias

echo -e "\nTest 6:"
echo -e "List aliases:"
alias
echo -e "Execute myalias2 alias:"
myalias2
echo -e "Remove myalias2 alias:"
unalias myalias2
alias

echo -e "\nTest 7:"
echo -e "List aliases:"
alias
echo -e "Execute myalias3 alias:"
myalias3
echo -e "Remove myalias3 alias:"
unalias myalias3
alias

echo -e "\nTest 8:"
echo -e "List aliases:"
alias
echo -e "Execute eep alias:"
eep
echo -e "Remove eep alias:"
unalias eep
echo -e "RESULT:#cmd:84|#alias:0|#script lines:95>"

