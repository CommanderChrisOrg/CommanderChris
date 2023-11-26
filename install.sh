#!/bin/bash
curl -o /usr/local/bin/chris  https://github.com/CommanderChrisOrg/CommanderChris/releases/download/v0.0.0/chris-macos
chmod +x /usr/local/bin/chris
curl -sSL https://raw.githubusercontent.com/CommanderChrisOrg/CommanderChris/main/setup.sh | bash