#include <stdio.h>
#include <unistd.h>
class istream{

};


class ByteCodeStream:public istream{

};

class Scanner
{
public:
    Scanner(const istream& ){

    }
    virtual ~Scanner(){

    }
};


class ProgramNodeBuilder;
class Parser
{
public:
    Parser(){

    }
    virtual ~Parser(){

    }

    virtual void Parse(Scanner& ,ProgramNodeBuilder& ){

    }
};


class ProgramNode;
class ProgramNodeBuilder
{
public:
    ProgramNodeBuilder(): m_node(NULL){

    }
    virtual ~ProgramNodeBuilder(){

    }


    virtual ProgramNode* NewAssignment(ProgramNode* var, ProgramNode* exp) const {

    }

    virtual ProgramNode* NewReturnStatement(ProgramNode* value) const {

    }

    virtual ProgramNode* NewCondition(ProgramNode* conditon , ProgramNode* truePart, ProgramNode* falsePart){

    }
    ProgramNode* GetRootNode(){

    }
private:
    ProgramNode* m_node;
};


class ProgramNode
{
public:
    ProgramNode(){

    }
    virtual ~ProgramNode(){

    }

    virtual void AddNode(ProgramNode* ){

    }

    virtual void RemoveNode(ProgramNode* ){

    }
};


class CodeGenerator
{
public:
    CodeGenerator(){

    }
    virtual ~CodeGenerator(){

    }
};


class Compiler
{
public:
    Compiler(){

    }
    virtual ~Compiler(){

    }
    virtual void Parse(istream&,ByteCodeStream&);
};



void Compiler::Parse(istream& input, ByteCodeStream&  output)
{
    Scanner scanner(input);
    ProgramNodeBuilder builder;
    Parser parser;
    parser.Parse(scanner, builder);
}



int main(int argc, char *argv[])
{
    istream stream;
    ByteCodeStream byteOutput;

    Compiler* compiler = new Compiler;
    compiler->Parse(stream, byteOutput);
    return 0;
}



