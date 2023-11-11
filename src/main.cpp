#include <iostream>
#include <sstream>
#include <vector>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "ftxui/component/component_options.hpp"
#include "ftxui/screen/color.hpp"
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

Element TerminalFrame(std::vector<Element> term_lines, Component &input_cmd){
    return vbox({
            text(L"Terminal"),
            vbox({
                vbox(std::move(term_lines)) | focusPositionRelative(0, 1) | yframe | flex,
                separator(),
                input_cmd->Render(),
            }) | border,
        }) | flex;
}

Element PromptFrame(std::string prompt_response, Component &input_prompt){
    return vbox({
            text(L"CommanderChris") | bold,
            paragraph(prompt_response),
            input_prompt->Render(),
        }) | flex; 
}

void run_term() {
    // Set the placeholder text for the input field.
    std::string cmd_content;
    std::string prompt_content;
    std::string prompt_response = "Response will appear here";
    std::vector<Element> term_lines;

    // Create an input component where users can type commands.
    InputOption opt_cmd = InputOption::Default();
    opt_cmd.content = &cmd_content;
    opt_cmd.placeholder = "Type commands here...";
    opt_cmd.on_enter = [&]{
        term_lines.emplace_back(color(Color::Grey0, text(cmd_content)));
        std::string out = exec(cmd_content);
        std::stringstream out_stream(out);
        std::string line;
        while(getline(out_stream, line, '\n')) term_lines.emplace_back(text(line));
        cmd_content = "";
    };
    Component input_cmd = Input(opt_cmd);

    // Create an input component where users can prompt gpt
    InputOption opt_prompt = InputOption::Default();
    opt_prompt.content = &prompt_content;
    opt_prompt.placeholder = "Ask me anything";
    opt_prompt.on_enter = [&]{
        prompt_content = prompt_content.substr(0, prompt_content.length() - 1);
        std::string command = getCommandFromPrompt(prompt_content);
        if(command.length() == 0) prompt_response = "No Results";
        else {
            prompt_response = "Command: " + command;
            cmd_content = command;
        }
        prompt_content = "";
    };
    Component input_prompt = Input(opt_prompt);
    
    auto screen = ScreenInteractive::Fullscreen();
    auto terminal_comp = Renderer(
        input_cmd,
        [&] { return TerminalFrame(term_lines, input_cmd); 
     });
    auto prompt_comp = Renderer(
        input_prompt,
        [&] { return PromptFrame(prompt_response, input_prompt); 
     });
    auto terminal = Container::Horizontal({
        terminal_comp,
        Renderer([&]{ return separator(); }),
        prompt_comp,
    });
    // Enter the main loop for the screen with the layout renderer.
    screen.Loop(terminal);
}

// Main function to run the program.
int main() {
    if(std::getenv("OPENAI_API_KEY")) run_term();
    else std::cerr << "ERROR: missing OpenAI API key\n\nyou can add your key with: export OPENAI_API_KEY=[your key]" << std::endl;

    return 0;
}
