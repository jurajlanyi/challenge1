#include <iostream>
#include <map>
#include <string>
#include <iostream>

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

bool g_done = false;

//
// TEST COMMANDS
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


class CommandDispatcher; // fwd declaration
// lanj - todo - parse payload, do something reasonable

class Controller {
public:
    bool help(rapidjson::Value &payload)
    {
        cout << "Controller::help: command: ";

        // implement

        return true;
    }

    bool exit(rapidjson::Value &payload)
    {
        cout << "Controller::exit: command: \n";

        // implement

//        g_done = true; // LANJ - commented our for some check
#pragma message ("TODO : LANJ - g_done = true")

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
    }

    // dtor - need impl
    virtual ~CommandDispatcher()
    {
        // question why is it virtual? Is it needed in this case?

        // LANJ - not needed here
        // it depends whether we derive another class from it and then use base class pointer and destruct the object so
        // it is wiser to have it virtual
    }

    bool addCommandHandler(std::string command, CommandHandler handler)
    {
        cout << "CommandDispatcher: addCommandHandler: " << command << std::endl;
        command_handlers_.insert(make_pair(command, handler));

        return true;
    }

    bool dispatchCommand(std::string command_json)
    {
        cout << "COMMAND: " << command_json << endl;

        // LANJ TODO - this has to be parsed/checked first !
        Document jsonDoc;
        ParseResult jsonOK = jsonDoc.Parse(command_json.c_str());

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
        rapidjson::Value& value = jsonDoc["payload"];

        // get command and payload
        // implement
        // lanj todo - parse PAYLOAD

        HandlerMap::iterator it;
        it = command_handlers_.find(command_member);
        if (it != command_handlers_.end()) {
            cout << "COMMAND: activating " << command_member << endl;
            CommandHandler handler = it->second;

#pragma message ("TODO : LANJ - what about payload/value ?")

//            rapidjson::Value value;
            handler(value);
            // LANJ: TODO - what about return values/data ?
//            cout << "VALUE is : " << value.GetString() << endl;
//            cout << "VALUE is : " << value.GetObject() << endl;

            StringBuffer buffer;
            Writer<StringBuffer> writer(buffer);
// payload is of Value type and could be altered in command handler
            value.Accept(writer); // write JSON value to a string

            cout << "Buffer: - return JSON object" << buffer.GetString() << endl;

            //const char* output = buffer.GetString();
        }
        else {
            cout << "Command handler not found error" << endl;
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
    // maybe this could be done programatically by iterating Controller methods

    //CommandHandler handler = std::bind(Controller::help, this, placeholders::_1);
    CommandHandler handler;

    // what about error checking ?
    handler = std::bind(&Controller::help, this, placeholders::_1);
    dispatcher.addCommandHandler("help", handler);

    handler = std::bind(&Controller::exit, this, placeholders::_1);
    dispatcher.addCommandHandler("exit", handler);
}

int main()
{
    std::cout << "COMMAND DISPATCHER: STARTED" << std::endl;

    cout << help_command << endl;

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
            cout << "COMMANDS: {\"command\":\"exit\", \"payload\":{\"reason\":\"User requested exit.\"}}\n";
            cout << "\tenter command : ";
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
        // plain commands from cmd line
        if (!is_json_command)
            command_dispatcher.dispatchCommand(command);
*/
    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;
    return 0;
}
