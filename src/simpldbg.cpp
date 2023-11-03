#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include "debugger.hpp"
#include "linenoise.h"

std::vector<std::string> split_input(const std::string &line) {
    std::istringstream iss{line};
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>{});
    return results;
}

bool is_prefix(std::string_view s, std::string_view of) {
    return s.size() <= of.size() && std::equal(s.begin(), s.end(), of.begin());
}

Debugger::Debugger(pid_t pid) : m_pid{pid} {}

void Debugger::run() {
    int status;

    waitpid(m_pid, &status, 0);

    char *line;
    while ((line = linenoise("sdbg> ")) != nullptr) {
        handle_command(line);
        linenoiseHistoryAdd(line);
        linenoiseFree(line);
    }
}

void Debugger::handle_command(const std::string &line) {
    auto args = split_input(line);
    auto command = args[0];

    if (is_prefix(command, "continue")) {
        std::cout << "Continuing execution\n";
        continue_execution();
    }
    else {
        std::cerr << "Unknown command\n";
    }
}

void Debugger::continue_execution() {
    #if defined(__APPLE__)
    ptrace(PT_CONTINUE, m_pid, nullptr, 0);
    #elif defined(__linux__)
    ptrace(PTRACE_CONT, m_pid, nullptr, nullptr);
    #endif

    int status;
    waitpid(m_pid, &status, 0);
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <program>\n";
        return 1;
    }

    auto program = argv[1];

    auto pid = fork();

    if (pid == 0) {

        #if defined(__APPLE__)
        ptrace(PT_TRACE_ME, 0, nullptr, 0);
        #elif defined(__linux__)
        ptrace(PTRACE_TRACEME, 0, nullptr, 0);
        #endif

        execl(program, program, nullptr);

        std::cerr << "Failed to execute " << program << "\n";
        return 1;
    } else if (pid >= 1) {
        std::cout << "Debug session begining on : " << pid << "\n";
        Debugger dbg{pid};
        dbg.run();
    }
    return 0;
}
