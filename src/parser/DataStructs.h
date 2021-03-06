#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include "AST.h"
#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;
using std::pair;

class FunctionSignature
{
public:
	FunctionSignature(string const& name, vector<ZVarTypeId> const& paramTypeIds);
	int compare(FunctionSignature const& other) const;
	bool operator==(FunctionSignature const& other) const {return compare(other) == 0;}
	bool operator<(FunctionSignature const& other) const {return compare(other) < 0;}
	string name;
	vector<ZVarTypeId> paramTypeIds;
};

class FunctionTypeIds
{
public:
	FunctionTypeIds() : returnTypeId(-1), paramTypeIds() {}
	FunctionTypeIds(ZVarTypeId returnTypeId, vector<ZVarTypeId> const& paramTypeIds);
	int compare(FunctionTypeIds const& other) const;
	bool operator==(FunctionTypeIds const& other) const {return compare(other) == 0;}
	bool operator<(FunctionTypeIds const& other) const {return compare(other) < 0;}
	ZVarTypeId returnTypeId;
	vector<ZVarTypeId> paramTypeIds;
	static FunctionTypeIds const null;
};

class ZClass;

class SymbolTable
{
public:
    SymbolTable();
	~SymbolTable();
	// Nodes
    int getNodeId(AST* node) const;
    void putNodeId(AST* node, int id);
    vector<int> getPossibleNodeFuncIds(AST* node) const;
    void putPossibleNodeFuncIds(AST* node, vector<int> possibleFuncIds);
	// Types
	ZVarType* getType(ZVarTypeId typeId) const;
	ZVarType* getType(AST* node) const;
	ZVarTypeId getTypeId(ZVarType const& type) const;
	ZVarTypeId assignTypeId(ZVarType const& type);
	ZVarTypeId getOrAssignTypeId(ZVarType const& type);
	// Classes
	ZClass* getClass(int classId) const;
	ZClass* createClass(string const& name);
	// Variables
    ZVarTypeId getVarTypeId(int varId) const;
    ZVarTypeId getVarTypeId(AST* node) const;
    ZVarType* getVarType(int varId) const;
    ZVarType* getVarType(AST* node) const;
    void putVarTypeId(int varId, ZVarTypeId typeId);
    void putVarType(int varId, ZVarType const& type);
	// Inlined Constants
	void inlineConstant(int varId, long value);
	void inlineConstant(AST* node, long value);
	bool isInlinedConstant(int varId) const;
	bool isInlinedConstant(AST* node) const;
	long getInlinedValue(int varId) const;
	long getInlinedValue(AST* node) const;
	// Functions
    ZVarTypeId getFuncReturnTypeId(int funcId) const;
    ZVarTypeId getFuncReturnTypeId(AST *node) const;
    vector<ZVarTypeId> getFuncParamTypeIds(int funcId) const;
    void putFuncTypeIds(int funcId, ZVarTypeId returnTypeId, vector<ZVarTypeId> const& paramTypeIds);
	// Global Pointers
    vector<int> const& getGlobalPointers() const {return globalPointers;}
    vector<int>& getGlobalPointers() {return globalPointers;}
    void addGlobalPointer(int varId) {globalPointers.push_back(varId);}
	// Other
    void printDiagnostics();
private:
    map<AST*, int> nodeIds;
	vector<ZVarType*> types;
	map<ZVarType*, ZVarTypeId, ZVarType::PointerLess> typeIds;
	vector<ZClass*> classes;
    map<AST*, vector<int> > possibleNodeFuncIds;
    map<int, ZVarTypeId> varTypes;
    map<int, long> inlinedConstants;
    map<int, FunctionTypeIds> funcTypes;
    vector<int> globalPointers;
};

struct SymbolData
{
    SymbolTable* symbols;
    vector<ASTFuncDecl*> globalFuncs;
    vector<ASTVarDecl*> globalVars;
    vector<ASTArrayDecl*> globalArrays;
    vector<ASTScript*> scripts;
    map<ASTScript*, int> runsymbols;
    map<ASTScript*, int> numParams;
    map<ASTScript*, ScriptType> scriptTypes;
    map<ASTScript*, int> thisPtr;
};

struct FunctionData
{
    SymbolTable* symbols;
    vector<ASTFuncDecl*> functions;
    vector<ASTVarDecl*> globalVars;
    vector<ASTVarDecl*> newGlobalVars;
    vector<ASTArrayDecl*> globalArrays;
    vector<ASTArrayDecl*> newGlobalArrays;
	int globalVarCount;
    map<string, int> scriptRunSymbols;
    map<string, int> numParams;
    map<string, ScriptType> scriptTypes;
    map<string, int> thisPtr;
};

struct IntermediateData
{
    map<int, vector<Opcode *> > funcs;
    vector<Opcode *> globalsInit;
    vector<Opcode *> globalasInit;
    map<string, int> scriptRunLabels;
    map<string, int> numParams;
    map<string, ScriptType> scriptTypes;
    map<string, int> thisPtr;
};

class LinkTable
{
public:
    int functionToLabel(int fid);
    int getGlobalID(int vid);
    int addGlobalVar(int vid);
    void addGlobalPointer(int vid)
    {
        globalIDs[vid]=0;
    }
private:
    map<int, int> funcLabels;
    map<int, int> globalIDs;
};

class StackFrame
{
public:
    void addToFrame(int vid, int offset)
    {
        stackoffset[vid] = offset;
    }
    int getOffset(int vid);
private:
    map<int, int> stackoffset;
};

struct OpcodeContext
{
    StackFrame *stackframe;
    LinkTable *linktable;
    SymbolTable *symbols;
	vector<Opcode*> initCode;
};

#endif

