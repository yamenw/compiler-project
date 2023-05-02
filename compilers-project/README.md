```
    Program -> Header Declarations Block.
               Header -> program id(input,output) ;
         Declarations -> CONST Declarations'
                       | VAR Declarations'
                       | ϵ
        Declarations' -> ConstantDefinitions
                       | VariableDeclarations
                       | ϵ
  ConstantDefinitions -> ConstantDefinition ConstantDefinitions'
   ConstantDefinition -> id=num;
 VariableDeclarations -> VariableDeclaration VariableDeclarations'
  VariableDeclaration -> IdentifierList : Type ;
       IdentifierList -> id IdentifierList'
                 Type -> integer
                       | real
                       | char
                       | boolean
                Block -> begin Statements end
           Statements -> Statement Statements'
                       | ϵ
          Statements' -> ; Statement Statements'
                       | ϵ
            Statement -> id := Expression
                       | begin Statements end
                       | if Expression then Statement
                       | repeat Statement until Expression
                       | writeln(SimpleExpression)
       ExpressionList -> Expression ExpressionList'
                       | ϵ
      ExpressionList' -> , Expression ExpressionList'
                       | ϵ
           Expression -> SimpleExpression Expression'
          Expression' -> relop SimpleExpression
                       | ϵ
     SimpleExpression -> Term SimpleExpression''
                       | addop Term SimpleExpression'
    SimpleExpression' -> addop Term SimpleExpression'
                       | ϵ
   SimpleExpression'' -> addop Term SimpleExpression'
                       | ϵ
                 Term -> Factor Term'
                Term' -> mulop Factor Term'
                       | ϵ
               Factor -> id
                       | num
                       | ( Expression )
                       | not Factor
 ConstantDefinitions' -> ConstantDefinition ConstantDefinitions'
                       | ϵ
VariableDeclarations' -> VariableDeclaration VariableDeclarations'
                       | ϵ
      IdentifierList' -> , id IdentifierList'
                       | ϵ
```