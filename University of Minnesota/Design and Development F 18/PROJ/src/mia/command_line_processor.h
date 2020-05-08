// Copyright Deandra Bardell 2018

#ifndef MIA_COMMAND_LINE_PROCESSOR_H_
#define MIA_COMMAND_LINE_PROCESSOR_H_

#include <string>
#include <vector>

#include "mia/image_editor_commands.h"

namespace image_tools {

class CommandLineProcessor {
 public:
  // Constructor does nothing
  CommandLineProcessor();

  // Destructor deletes pointers in commands_ vector
  virtual ~CommandLineProcessor();

  // Used to parse the user commands and executes the commands
  // after input correctness is verified.
  // returns 1 if there are no errors, 0 otherwise
  int ProcessCommandLine(const int &argc, char *argv[]);

  // Used to print list of commands user can input
  void PrintHelp();

  // Used to check if a file exists by ProcessCommandLine
  bool FileExists(const char* fname);

 private:
  std::vector<ImageEditorCommand*> commands_;
};


}  // namespace image_tools

#endif  // MIA_COMMAND_LINE_PROCESSOR_H_
