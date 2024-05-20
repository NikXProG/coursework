#ifndef PARSER_CONTROL_H
#define PARSER_CONTROL_H

#include "parser.h"

class parser_control{

public:

    virtual ~parser_control() noexcept = default;

    virtual bool lexical_analysis_check(const char* command);
    virtual bool syntax_analysis_check(const char* command);
    virtual bool semantic_analysis_check(const char* command);


};
#endif