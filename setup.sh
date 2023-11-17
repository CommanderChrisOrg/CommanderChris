#!/bin/bash
curl -O https://raw.githubusercontent.com/CommanderChrisOrg/CommanderChris/main/chris_bash.sh
cat chris_bash.sh >> ~/.bashrc
cat chris_bash.sh >> ~/.zshrc
rm chris_bash.sh