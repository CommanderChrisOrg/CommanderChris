#include <iostream>
#include <sstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include "gpt.hpp"


using namespace ftxui;

std::string exec(std::string command) {
   char buffer[128];
   std::string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}

bool run_confirmation_ui() {
    auto screen = ScreenInteractive::TerminalOutput();

    bool run_command = false;
    auto confirmation = Container::Horizontal({
        Button("Run Command", [&] { run_command = true; screen.Exit(); }),
        Button("Back To Terminal", [&] { screen.Exit(); }),
    });
 
    screen.Loop(confirmation);

    return run_command;
}

void display_command(std::string command){
    if(command == ""){ std::cout << "No results" << std::endl; return; }
    std::cout << "You should try" << std::endl;
    std::cout << command << std::endl << std::endl;
}

// Main function to run the program.
int main(int argc, char **argv) {
    if(argc == 1){ std::cerr << "Usage: " << argv[0] <<  " [question]" << std::endl; return 1; }

    std::stringstream prompt;
    for(int i = 1; i < argc; ++i){
        if(i > 1) prompt << " ";
        prompt << argv[i];
    }

    std::string command;
    if(std::getenv("OPENAI_API_KEY")) command = getCommandFromPrompt(prompt.str());
    else { std::cerr << "ERROR: missing OpenAI API key\n\nyou can add your key with: export OPENAI_API_KEY=[your key]" << std::endl; return 1; }

    display_command(command);

    if(command != "" && run_confirmation_ui()) std::cout << std::endl << exec(command) << std::endl;

    return 0;
}
