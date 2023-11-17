#!/bin/bash
curl -o ~/.chris_bash https://raw.githubusercontent.com/CommanderChrisOrg/CommanderChris/main/.chris_bash
echo "source ~/.chris_bash" >> ~/.bashrc
echo "source ~/.chris_bash" >> ~/.zshrc
source ~/.chris_bash