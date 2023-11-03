#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

// #include <sys/types.h>

class Debugger {
public:
    Debugger(pid_t pid);

    void run();

    void handle_command(const std::string &line);

    void continue_execution();
private:
    pid_t m_pid;
};

#endif //DEBUGGER_HPP
