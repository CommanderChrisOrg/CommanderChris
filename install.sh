#!/bin/bash
sudo curl -o /usr/local/bin/chris_exec -L https://github.com/CommanderChrisOrg/CommanderChris/releases/download/v0.0/chris-macos
sudo chmod +x /usr/local/bin/chris_exec
curl -sSL https://raw.githubusercontent.com/CommanderChrisOrg/CommanderChris/main/setup.sh | bash