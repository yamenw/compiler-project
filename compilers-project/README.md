"fixed" grammar

```
Program -> Header Declarations Block.
Header -> program id(input,output) ;
Declarations -> CONST Declarations'
ConstantDefinitions -> ConstantDefinition ConstantDefinitions'
ConstantDefinition -> id=num;
VariableDeclarations -> VariableDeclaration VariableDeclarations'
VariableDeclaration -> IdentifierList : Type ;
IdentifierList -> id IdentifierList'
Type -> integer | real | char | boolean
Block -> begin Statements end
Statements -> Statement Statements'
Statement -> id := Expression| begin Statements end | if Expression then Statement
| repeat Statement until Expression | writeln(SimpleExpression) | ϵ
ExpressionList -> Expression ExpressionList'
Expression -> SimpleExpression Expression'
SimpleExpression -> Term SimpleExpression' | addop Term SimpleExpression'
Term -> Factor Term'
Factor -> id | num | ( Expression ) | not F actor
 ConstantDefinitions' -> ConstantDefinition ConstantDefinitions' | ϵ
VariableDeclarations' -> VariableDeclaration VariableDeclarations' | ϵ
IdentifierList' -> , id IdentifierList' | ϵ
Statements' -> ; Statement Statements' | ϵ
ExpressionList' -> , Expression ExpressionList' | ϵ
SimpleExpression' -> addop Term SimpleExpression' | ϵ
Term' -> mulop Factor Term' | ϵ
Declarations' -> ConstantDefinitions |VAR VariableDeclarations | ConstantDefinitions VAR VariableDeclarations ϵ
Expression' -> relop SimpleExpression | ϵ
```

OG grammar

```
Program -> Header Declarations Block.
Header -> program id(input,output) ;
Declarations -> CONST ConstantDefinitions|VAR VariableDeclarations | 
CONST ConstantDefinitions VAR VariableDeclarations
<epsilon>
ConstantDefinitions -> ConstantDefinitions  ConstantDefinition
                                         | ConstantDefinition
ConstantDefinition -> id=num;
VariableDeclarations  -> VariableDeclarations  VariableDeclaration
                                         | VariableDeclaration
VariableDeclaration -> IdentifierList : Type ;
IdentifierList  -> IdentifierList , id | id
Type -> integer | real | char | boolean

Block  ->  begin Statements end
Statements  ->   Statements ;  Statement | Statement
Statement -> id := Expression
                        | Block
                        | if Expression then Statement
                        | repeat Statement until Expression 
                        | writeln(SimpleExpression)
                        | <epsilon>
ExpressionList -> ExpressionList , Expression | Expression

Expression -> SimpleExpression relop SimpleExpression
                | SimpleExpression

SimpleExpression -> Term
                        | addop Term
                        | SimpleExpression addop Term

Term -> Term mulop Factor
                | Factor
Factor -> id        | num        | ( Expression )        | not  Factor 
```