using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CompilerVVM
{
    public class Parser
    {
        int pointer;
        IList<object> Tokens;
        int idvar;
        Dictionary<string, string> Constants;
        Dictionary<string, MethodAtom> Methods;
        int jumpid;

        public Parser(IList<object> tokens, Dictionary<string, string> TextConst, Dictionary<string, MethodAtom> Methods)
        {
            Tokens = tokens;
            pointer = 0;
            jumpid = 0;
            Constants = TextConst;
            this.Methods = Methods;
            ParseMethod();
        }

        private void ParseMethod()
        {
            //<program> := <method>+
            while (pointer != Tokens.Count)
            {
                //<method> := method <type_method> <ident>(<param>*) <block>
                //<type> := int | double | string
                //<type_method> := <type> | void
                MethodAtom method = null;
                if (!Tokens[pointer++].Equals("method"))
                    throw new ParserException("No method detected");
                else
                {
                    method = Methods[Tokens[++pointer].ToString()];
                    idvar = method.NumOfParams;
                    method.Code.Add(String.Format(".proc {0}", method.Name, method.NumOfParams));

                    pointer += 3 + (2 * method.NumOfParams);
                    if (Tokens[pointer].Equals(OP.OpenBlock))
                    {
                        pointer++;
                        ParseBlock(method);
                    }
                    else throw new ParserException("No code block in method: " + method.Name);

                    if (method.Name == "main")
                        method.Code.Add("STOP");
                    else if (method.Type == "void")
                        method.Code.Add("RETURN");
                    else
                    {
                        if (method.Code[method.Code.Count - 1] != "RETURN")
                            throw new ParserException("No return sequence in method:" + method.Name);
                    }
                    method.Code.Add(".endp");

                    method.NumOfLocals = method.Variables.Count;
                    method.Code[0] = String.Format(".proc {0} {1} {2}", method.Name, method.NumOfLocals, method.NumOfParams);
                }
            }

        }

        private void ParseBlock(MethodAtom method)
        {
            while (!Tokens[pointer].Equals(OP.CloseBlock))
            {
                if (Tokens[pointer].Equals(OP.Semicolon))
                {
                    pointer++;
                }
                else if (code_types.Contains(Tokens[pointer].ToString()))
                    DeclareVar(method);
                else if (Tokens[pointer + 1].Equals(OP.Assigment))
                {
                    List<Variable> NeededVars = method.Variables.FindAll(x => x.Name == Tokens[pointer].ToString());
                    if (NeededVars.Count == 0)
                        throw new ParserException(string.Format("Variable {0} is not defined", Tokens[pointer].ToString()));
                    else
                    {
                        pointer += 2;
                        ParseExpression(method, NeededVars[NeededVars.Count - 1].Type);
                        switch (NeededVars[NeededVars.Count - 1].Type)
                        {
                            case "int":
                                method.Code.Add(string.Format("STOREIVAR {0}", NeededVars[NeededVars.Count - 1].ID));
                                break;
                            case "double":
                                method.Code.Add(string.Format("STOREDVAR {0}", NeededVars[NeededVars.Count - 1].ID));
                                break;
                            case "string":
                                method.Code.Add(string.Format("STORESVAR {0}", NeededVars[NeededVars.Count - 1].ID));
                                break;
                        }
                    }
                }
                else if (Tokens[pointer].ToString() == "return")
                {
                    ParseSingleExpr(method, pointer + 1, method.Type);
                    method.Code.Add("RETURN");
                    pointer += 3;
                }
                else if (Tokens[pointer].ToString() == "if")
                {
                    if (!Tokens[pointer + 1].Equals(OP.OpenParam) || !Tokens[pointer + 5].Equals(OP.CloseParam))
                        throw new ParserException("Wrong if statement");
                    string var1 = GetTypeOfVar(method, pointer + 2);
                    string var2 = GetTypeOfVar(method, pointer + 4);
                    if (var1 != var2)
                        throw new ParserException("Incompatible variable types in if statement");
                    int jump = jumpid++;
                    ParseSingleExpr(method, pointer + 4, var2);
                    ParseSingleExpr(method, pointer + 2, var1);

                    switch ((OP)Tokens[pointer + 3])
                    {
                        case OP.Equal:
                            method.Code.Add(string.Format("IFICMPNE jump_{0}", jump));
                            break;
                        case OP.NotEqual:
                            method.Code.Add(string.Format("IFICMPE jump_{0}", jump));
                            break;
                        case OP.Greater:
                            method.Code.Add(string.Format("IFICMPLE jump_{0}", jump));
                            break;
                        case OP.GreaterEqual:
                            method.Code.Add(string.Format("IFICMPL jump_{0}", jump));
                            break;
                        case OP.Less:
                            method.Code.Add(string.Format("IFICMPGE jump_{0}", jump));
                            break;
                        case OP.LessEqual:
                            method.Code.Add(string.Format("IFICMPG jump_{0}", jump));
                            break;
                    }
                    method.Code.Add("POP");
                    method.Code.Add("POP");
                    if (!Tokens[pointer + 6].Equals(OP.OpenBlock))
                        throw new ParserException("No { code } in if statement");
                    pointer += 7;
                    ParseBlock(method);
                    if (Tokens[pointer].ToString() != "else")
                    {
                        method.Code.Add(string.Format("jump_{0}:", jump));
                        method.Code.Add("POP");
                        method.Code.Add("POP");
                    }
                    else
                    {
                        int jump2 = jumpid++;
                        method.Code.Add(string.Format("JA jump_{0}", jump2));
                        if (!Tokens[pointer + 1].Equals(OP.OpenBlock))
                            throw new ParserException("No { code } in else statement");
                        method.Code.Add(string.Format("jump_{0}:", jump));
                        method.Code.Add("POP");
                        method.Code.Add("POP");
                        pointer += 2;
                        ParseBlock(method);
                        method.Code.Add(string.Format("jump_{0}:", jump2));
                    }
                }
                else if (Tokens[pointer].ToString() == "while")
                {
                    if (!Tokens[pointer + 1].Equals(OP.OpenParam) || !Tokens[pointer + 5].Equals(OP.CloseParam))
                        throw new ParserException("Wrong while statement");
                    string var1 = GetTypeOfVar(method, pointer + 2);
                    string var2 = GetTypeOfVar(method, pointer + 4);
                    if (var1 != var2)
                        throw new ParserException("Incompatible variable types in while statement");
                    int jump = jumpid++;
                    int jump2 = jumpid++;

                    method.Code.Add(string.Format("jump_{0}:", jump));
                    ParseSingleExpr(method, pointer + 4, var2);
                    ParseSingleExpr(method, pointer + 2, var1);

                    switch ((OP)Tokens[pointer + 3])
                    {
                        case OP.Equal:
                            method.Code.Add(string.Format("IFICMPNE jump_{0}", jump2));
                            break;
                        case OP.NotEqual:
                            method.Code.Add(string.Format("IFICMPE jump_{0}", jump2));
                            break;
                        case OP.Greater:
                            method.Code.Add(string.Format("IFICMPLE jump_{0}", jump2));
                            break;
                        case OP.GreaterEqual:
                            method.Code.Add(string.Format("IFICMPL jump_{0}", jump2));
                            break;
                        case OP.Less:
                            method.Code.Add(string.Format("IFICMPGE jump_{0}", jump2));
                            break;
                        case OP.LessEqual:
                            method.Code.Add(string.Format("IFICMPG jump_{0}", jump2));
                            break;
                    }
                    method.Code.Add("POP");
                    method.Code.Add("POP");
                    if (!Tokens[pointer + 6].Equals(OP.OpenBlock))
                        throw new ParserException("No { code } in while statement");
                    pointer += 7;
                    ParseBlock(method);
                    method.Code.Add(string.Format("JA jump_{0}", jump));
                    method.Code.Add(string.Format("jump_{0}:", jump2));
                }
                else if (Tokens[pointer].ToString() == "do")
                {
                    int jump = jumpid++;
                    int jump2 = jumpid++;
                    method.Code.Add(string.Format("JA jump_{0}", jump2));
                    method.Code.Add(string.Format("jump_{0}:", jump));
                    method.Code.Add("POP");
                    method.Code.Add("POP");
                    method.Code.Add(string.Format("jump_{0}:", jump2));
                    pointer += 2;
                    ParseBlock(method);

                    if (!Tokens[pointer + 1].Equals(OP.OpenParam) || !Tokens[pointer + 5].Equals(OP.CloseParam) ||
                        Tokens[pointer].ToString() != "until")
                        throw new ParserException("Wrong until statement");
                    string var1 = GetTypeOfVar(method, pointer + 2);
                    string var2 = GetTypeOfVar(method, pointer + 4);
                    if (var1 != var2)
                        throw new Exception("Incompatible variable types in until statement");

                    ParseSingleExpr(method, pointer + 4, var2);
                    ParseSingleExpr(method, pointer + 2, var1);

                    switch ((OP)Tokens[pointer + 3])
                    {
                        case OP.Equal:
                            method.Code.Add(string.Format("IFICMPNE jump_{0}", jump));
                            break;
                        case OP.NotEqual:
                            method.Code.Add(string.Format("IFICMPE jump_{0}", jump));
                            break;
                        case OP.Greater:
                            method.Code.Add(string.Format("IFICMPLE jump_{0}", jump));
                            break;
                        case OP.GreaterEqual:
                            method.Code.Add(string.Format("IFICMPL jump_{0}", jump));
                            break;
                        case OP.Less:
                            method.Code.Add(string.Format("IFICMPGE jump_{0}", jump));
                            break;
                        case OP.LessEqual:
                            method.Code.Add(string.Format("IFICMPG jump_{0}", jump));
                            break;
                    }
                    method.Code.Add("POP");
                    method.Code.Add("POP");
                    pointer += 7;
                }
                else if (Tokens[pointer].ToString() == "print")
                {
                    pointer += 2;
                    string type = GetTypeOfVar(method, pointer);
                    ParseSingleExpr(method, pointer, type);
                    pointer += 3;
                    switch (type)
                    {
                        case "int":
                            method.Code.Add("IPRINT");
                            break;
                        case "double":
                            method.Code.Add("DPRINT");
                            break;
                        case "string":
                            method.Code.Add("SPRINT");
                            break;
                    }
                }
                else if (Tokens[pointer+1].Equals(OP.OpenParam))
                {
                    ParseCall(method);
                }
            }
            pointer++;
        }
        private void ParseExpression(MethodAtom method, string type)
        {
            if (Tokens[pointer + 1].Equals(OP.Semicolon))
            {
                ParseSingleExpr(method, pointer, type);
                pointer += 2;
            }
            else if (Tokens[pointer + 1].Equals(OP.OpenParam))
            {
                ParseCall(method, type);
            }
            else if (Tokens[pointer].Equals(OP.Sub))
            {
                ParseSingleExpr(method, pointer + 1, type);
                switch (type)
                {
                    case "int":
                        method.Code.Add("INEG");
                        break;
                    case "double":
                        method.Code.Add("DNEG");
                        break;
                    default:
                        throw new ParserException("Incompatible types");
                }
            }
            else if (Tokens[pointer + 1].Equals(OP.Add))
            {
                int prevpointer = pointer;
                pointer += 2;

                ParseExpression(method, type);
                ParseSingleExpr(method, prevpointer, type);

                switch (type)
                {
                    case "int":
                        method.Code.Add("IADD");
                        break;
                    case "double":
                        method.Code.Add("DADD");
                        break;
                    default:
                        throw new ParserException("Incompatible types");
                }
            }
            else if (Tokens[pointer + 1].Equals(OP.Sub))
            {
                int prevpointer = pointer;
                pointer += 2;

                ParseExpression(method, type);
                ParseSingleExpr(method, prevpointer, type);

                switch (type)
                {
                    case "int":
                        method.Code.Add("ISUB");
                        break;
                    case "double":
                        method.Code.Add("DSUB");
                        break;
                    default:
                        throw new ParserException("Incompatible types");
                }
            }
            else if (Tokens[pointer + 1].Equals(OP.Mul))
            {
                int prevpointer = pointer;
                pointer += 2;

                ParseExpression(method, type);
                ParseSingleExpr(method, prevpointer, type);

                switch (type)
                {
                    case "int":
                        method.Code.Add("IMUL");
                        break;
                    case "double":
                        method.Code.Add("DMUL");
                        break;
                    default:
                        throw new ParserException("Incompatible types");
                }
            }
            else if (Tokens[pointer + 1].Equals(OP.Div))
            {
                int prevpointer = pointer;
                pointer += 2;

                ParseExpression(method, type);
                ParseSingleExpr(method, prevpointer, type);

                switch (type)
                {
                    case "int":
                        method.Code.Add("IDIV");
                        break;
                    case "double":
                        method.Code.Add("DDIV");
                        break;
                    default:
                        throw new ParserException("Incompatible types");
                }
            }

            else if (Tokens[pointer + 1].Equals(OP.Mod))
            {
                int prevpointer = pointer;
                pointer += 2;

                ParseExpression(method, type);
                ParseSingleExpr(method, prevpointer, type);

                switch (type)
                {
                    case "int":
                        method.Code.Add("IMOD");
                        break;
                    default:
                        throw new ParserException("Incompatible types");
                }
            }
        }

        private void ParseCall(MethodAtom method, string type = null)
        {
            if (Methods.ContainsKey(Tokens[pointer].ToString()))
            {
                MethodAtom CallMethod = Methods[Tokens[pointer].ToString()];
                if (type != null && type != CallMethod.Type)
                    throw new ParserException("Incompatible types when call method" + CallMethod.Name);

                pointer += 2;
                List<object> param = new List<object>();
                while (!Tokens[pointer++].Equals(OP.CloseParam))
                {
                    param.Add(Tokens[pointer]);
                }

                if (param.Count != CallMethod.NumOfParams)
                    throw new ParserException("Wrong params when call method" + CallMethod.Name);

                for (int i = 0; i < param.Count; i++)
                {
                    ParseSingleExpr(method, pointer - 2 - i, CallMethod.Variables[i].Type);
                }

                method.Code.Add(string.Format("CALL {0}", CallMethod.Name));

            }
            else throw new ParserException("Undefined method to call");
        }

        private void ParseSingleExpr(MethodAtom method, int pointer, string type)
        {
            if (Tokens[pointer] is StringBuilder)
            {
                if (type == "string")
                    method.Code.Add(string.Format("SLOAD {0}", Constants[Tokens[pointer++].ToString()]));
                else
                    throw new ParserException("Incompatible type");
            }
            else if (Tokens[pointer] is Number)
            {
                switch (type)
                {
                    case "int":
                        if (!Tokens[pointer].ToString().Contains("."))
                        {
                            method.Code.Add(string.Format("ILOAD {0}", Tokens[pointer++].ToString()));
                            break;
                        }
                        else throw new ParserException("Incompatible type");

                    case "double":
                        method.Code.Add(string.Format("DLOAD {0}", Tokens[pointer++].ToString()));
                        break;
                }
            }
            else
            {
                List<Variable> NeededVars = method.Variables.FindAll(x => x.Name == Tokens[pointer].ToString());
                if (NeededVars.Count != 0)
                {
                    if (NeededVars[NeededVars.Count - 1].Type != type)
                        throw new ParserException("Incompatible type");
                    else
                    {
                        switch (type)
                        {
                            case "int":
                                method.Code.Add(string.Format("LOADIVAR {0}", NeededVars[NeededVars.Count - 1].ID));
                                break;
                            case "double":
                                method.Code.Add(string.Format("LOADDVAR {0}", NeededVars[NeededVars.Count - 1].ID));
                                break;
                            case "string":
                                method.Code.Add(string.Format("LOADSVAR {0}", NeededVars[NeededVars.Count - 1].ID));
                                break;
                        }
                    }
                }
                else throw new ParserException("Can't parse sequence");
            }
        }

        private void DeclareVar(MethodAtom method)
        {
            method.Variables.Add(new Variable(idvar++, Tokens[pointer].ToString(), Tokens[pointer + 1].ToString()));
            if (Tokens[pointer + 2].Equals(OP.Semicolon))
            {
                pointer += 3; return;
            }
            else if (Tokens[pointer + 2].Equals(OP.Assigment))
            {
                pointer += 3;
                ParseExpression(method, method.Variables[idvar - 1].Type);
                switch (method.Variables[idvar - 1].Type)
                {
                    case "int":
                        method.Code.Add(string.Format("STOREIVAR {0}", idvar - 1));
                        break;

                    case "double":
                        method.Code.Add(string.Format("STOREDVAR {0}", idvar - 1));
                        break;

                    case "string":
                        method.Code.Add(string.Format("STORESVAR {0}", idvar - 1));
                        break;
                }

                if (Tokens[pointer].Equals(OP.Semicolon))
                {
                    pointer++; return;
                }
            }
            else
                throw new ParserException("Wrong variable defenition");
        }

        private string GetTypeOfVar(MethodAtom method, int pointer)
        {
            string result = null;
            if (Tokens[pointer] is StringBuilder)
                result = "string";
            else if (Tokens[pointer] is Number)
            {
                if (Tokens[pointer].ToString().Contains("."))
                    result = "double";
                else result = "int";
            }
            else if (Methods.ContainsKey(Tokens[pointer].ToString()))
            {
                result = Methods[Tokens[pointer].ToString()].Type;
            }
            else
            {
                List<Variable> isVariable = method.Variables.FindAll(x => x.Name == Tokens[pointer].ToString());
                if (isVariable.Count == 0)
                    throw new ParserException("No defined variable with name "+Tokens[pointer].ToString());
                else result = isVariable[isVariable.Count - 1].Type;
            }
            return result;
        }

        List<string> code_types = new List<string>() { "int", "double", "string" };
    }
}
