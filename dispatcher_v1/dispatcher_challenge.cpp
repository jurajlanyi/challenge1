#include <iostream>
#include <map>
#include <string>
#include <iostream>

#include <list>
#include <fstream>

#include <functional>

//
// supporting tools and software
//
// Validate and test your json commands
// https://jsonlint.com/

// RapidJSON : lots and lots of examples to help you use it properly
// https://github.com/Tencent/rapidjson
//

// std::function
// std::bind
// std::placeholders
// std::map
// std::make_pair

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

// Define this if you want to use MyList<int> instead of std::list<int>
//#define USE_LINKED_LIST

#ifdef USE_LINKED_LIST

#include "linked_list.h"

#else
std::ostream& operator<< (std::ostream &out, list<int> &intList) // Pretty print
{
    int n = 1;
    out << "List size is " << intList.size() << " items:" << endl;
    for(auto item : intList) {
        out << "Item " << n++ << "= " << item << endl;
    }
    return out;
}
#endif

bool g_done = false;

//
// TEST COMMANDS - just copy and paste including outer {}
//

auto help_command = R"(
 {
  "command":"help",
  "payload": {
    "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }",
  }
 }
)";

auto exit_command = R"(
 {
  "command":"exit",
  "payload": {
     "reason":"Exiting program on user request."
  }
 }
)";

// TODO: extend implementation with more actions
auto list_test_command = R"(
 {
  "command":"list_test",
  "payload": {
   "init": [7, 4, 0, 8, 3, 1, 6, 2, 5, 6],
   "action" : "sort",
   "output" : "list_test.out"
  }
 }
)";


class CommandDispatcher; // fwd declaration

class Controller {
public:
    bool help(rapidjson::Value &payload)
    {
        // LANJ: if usage is found, just display
        // if document is gived, start document viewer

        cout << "Controller::help: command \n";
        if (payload.HasMember("usage")) {
            string strUsage = payload["usage"].GetString();
            cout << "Help instruction: Usage is" << endl << strUsage << endl << "Help end." << endl;
        }
        return true;
    }

    bool exit(rapidjson::Value &payload)
    {
        // LANJ: reason could be logged into system log, ...

        cout << "Controller::exit: command: \n";
        string strExitReason = "unspecified";
        if (payload.HasMember("reason")) {
            strExitReason = payload["reason"].GetString();
        }
        cout << "Program exit, reason: " << strExitReason << endl;
        g_done = true;
        return true;
    }

// TODO: extend implementation with more actions type and multiple actions
    bool list_test(rapidjson::Value &payload)
    {
        cout << "Controller::list_test: command: \n";

        if (!payload.HasMember("init") || !payload["init"].IsArray()) {
            cout << "No init array found" << endl;
            return false;
        }
        if (!payload.HasMember("output") || !payload["output"].IsString()) {
            cout << "No output string found" << endl;
            return false;
        }
        if (!payload.HasMember("action") || !payload["action"].IsString()) {
            cout << "No action string found" << endl;
            return false;
        }

        const Value& array = payload["init"];
#ifndef USE_LINKED_LIST
        std::list<int> intList;
#else
        MyList<int> intList;
#endif
        for (SizeType i = 0; i < array.Size(); i++) {
            int iValue = array[i].GetInt();
            intList.push_back(iValue);
        }
        string action = payload["action"].GetString();

        if (action == "sort") {
            intList.sort();
        }

        string outputFilename = payload["output"].GetString();
        std::ofstream ofs;
        ofs.open(outputFilename, std::ofstream::out | std::ofstream::trunc);
        if (ofs.is_open()) {
            ofs << intList;     // Pretty print :-)
            ofs.close();
        }

        cout << "list_test exit, result written to file: " << outputFilename << endl;
        return true;
    }

    void registerHandlers(CommandDispatcher& dispatcher);
    // implement 3-4 more commands
};

// gimme ... this is actually tricky
// Bonus Question: why did I type cast this?

// LANJ - 'bool(rapidjson::Value &)' is not a cast, but a function "signature" used in template parameter

typedef std::function<bool(rapidjson::Value &)> CommandHandler;

class CommandDispatcher {
public:
    // ctor - need impl
    CommandDispatcher()
    {
        // LANJ: ctor implementation not needed
    }

    // dtor - need impl
    virtual ~CommandDispatcher()
    {
        // question why is it virtual? Is it needed in this case?

        // LANJ - not needed here
        // it depends whether we derive another class from it and then use base class pointer and destruct the object so
        // it is wiser to have it virtual

        // LANJ: dtor implementation not needed, unless there is some specific about:
        // handler = std::bind(&Controller::help, this, placeholders::_1);
        // TODO - study docs
    }

    bool addCommandHandler(std::string command, CommandHandler handler)
    {
        cout << "CommandDispatcher: addCommandHandler: " << command << std::endl;
        command_handlers_.insert(make_pair(command, handler));

        return true;
    }

    bool dispatchCommand(std::string command_json)
    {
        cout << "dispatchCommand:COMMAND:" << endl;

        Document jsonDoc;
        ParseResult jsonOK = jsonDoc.Parse(command_json.c_str());
        if (!jsonOK)
            return false;

        if (!jsonDoc.HasMember("command")) {
            cout << "Error: No command member found in JSON" << endl;
            return false;
        }
        if (!jsonDoc.HasMember("payload")) {
            cout << "Error: No payload member found in JSON" << endl;
            return false;
        }
        if (!(jsonDoc["command"].IsString())) {
            cout << "Error: command not a string" << endl;
            return false;
        }
        string command_member = jsonDoc["command"].GetString();
        rapidjson::Value& payload = jsonDoc["payload"];

        HandlerMap::iterator it;
        it = command_handlers_.find(command_member);
        if (it != command_handlers_.end()) {
            cout << "COMMAND: activating command '" << command_member << "'" << endl;
            CommandHandler handler = it->second;
            bool retValue = handler(payload);   // LANJ: Exceptions possible ? Handling, etc ...
            cout << "Command Handler returned, return value: " << retValue << endl;

#pragma message ("TODO : LANJ - what to do with returned payload/value ?")
            // LANJ: TODO - what about return values/data ?
            // payload is of Value type and could be altered inside command handler
            // for now just display it
            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
            payload.Accept(writer); // write JSON value to a string
//            cout << "Command handler returned JSON payload: " << buffer.GetString() << endl; // lengthy string, don't display
            cout << "Command handler returned JSON payload: " << "\"some long string\"" << endl;
        }
        else {
            cout << "Error: Command handler '" << command_member << "' not found" << endl;
        }

        return true;
    }

    // gimme ...
private:
    typedef std::map<std::string, CommandHandler> HandlerMap;
    std::map<std::string, CommandHandler> command_handlers_;

    // another gimme ...
    // Question: why delete these?

    // LANJ - to prevent creating copy objects and copying dispatcher
// because we store CommandHandler's they are similar to function pointers,
// and in this case they point to Controller object.
// Controller object should be in same scope as CommandDispatcher obect

    // delete unused constructors and assign. operator
    CommandDispatcher (const CommandDispatcher&) = delete;
    CommandDispatcher& operator= (const CommandDispatcher&) = delete;

};

void Controller::registerHandlers(CommandDispatcher& dispatcher)
{
    // LANJ: maybe this could be done programatically by iterating Controller methods

    CommandHandler handler;

    // what about error checking, like duplicate handler, or multiple handlers for the same command ?
    handler = std::bind(&Controller::help, this, placeholders::_1);
    dispatcher.addCommandHandler("help", handler);

    handler = std::bind(&Controller::exit, this, placeholders::_1);
    dispatcher.addCommandHandler("exit", handler);

    handler = std::bind(&Controller::list_test, this, placeholders::_1);
    dispatcher.addCommandHandler("list_test", handler);
}

int main()
{
    std::cout << "COMMAND DISPATCHER: STARTED" << std::endl;

    CommandDispatcher command_dispatcher;
    Controller controller;                 // controller class of functions to "dispatch" from Command Dispatcher

    // Implement
    // add command handlers in Controller class to CommandDispatcher using addCommandHandler

    controller.registerHandlers(command_dispatcher); // registers controller's member handlers

    // gimme ...
    // command line interface
    string command;
    string inputLine;

    while( ! g_done ) {
        if (command.empty()) {
            cout << endl << "COMMANDS: {\"command\":\"exit\", \"payload\":{\"reason\":\"User requested exit.\"}}\n";
            cout << "COMMANDS: TODO - show all commands ?\n";
            cout << "\tenter command (or Ctrl-Z): ";
        }
        if (cin.eof()) {
            cout << "Input EOF, terminating" << endl;
            break;
        }
        getline(cin, inputLine);
        command += inputLine;

        Document jsonDoc;
        ParseResult jsonOK = jsonDoc.Parse(command.c_str());
        if (!jsonOK)
            continue;

        command_dispatcher.dispatchCommand(command);
        command.clear();

/*
        // allow plain commands from cmd line - not needed anymore
        if (!is_json_command)
            command_dispatcher.dispatchCommand(command);
*/
    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;
    return 0;
}
