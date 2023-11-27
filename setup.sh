#!/bin/bash
curl -o ~/.chris_bash -L https://raw.githubusercontent.com/CommanderChrisOrg/CommanderChris/main/.chris_bash
curl -o ~/.chris_zsh -L https://raw.githubusercontent.com/CommanderChrisOrg/CommanderChris/main/.chris_zsh

if ! grep -q "source ~/.chris_bash" ~/.bashrc; then
    echo -e "\nsource ~/.chris_bash" >> ~/.bashrc
fi

if ! grep -q "source ~/.chris_zsh" ~/.zshrc; then
    echo -e "\nsource ~/.chris_zsh" >> ~/.zshrc
fi