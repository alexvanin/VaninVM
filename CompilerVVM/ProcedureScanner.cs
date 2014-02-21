using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CompilerVVM
{
    class ProcedureScanner
    {
        public Dictionary<string, MethodAtom> Methods { get; set; }
        int pointer;

        public ProcedureScanner(IList<object> Tokens)
        {
            // TODO: Complete member initialization
            pointer = 0;
            ScanForMethods(Tokens);
        }

        private void ScanForMethods(IList<object> Tokens)
        {
            Methods = new Dictionary<string, MethodAtom>();
            while (pointer != Tokens.Count)
            {
                MethodAtom method = null;
                if (Tokens[pointer++].Equals("method"))
                {
                    if (!Tokens[pointer].Equals("void") && !Tokens[pointer].Equals("int") && 
                        !Tokens[pointer].Equals("double") && !Tokens[pointer].Equals("string"))
                    {
                        throw new ProcedureException("Wrong method defenition");
                    }
                    int idvar = 0;
                    method = new MethodAtom(Tokens[++pointer].ToString());
                    method.Type = Tokens[pointer++ - 1].ToString();
                    if (!Tokens[pointer++].Equals(OP.OpenParam))
                        throw new ProcedureException("Wrong method defenition of method: "+method.Name);
                    else
                    {
                        while (!Tokens[pointer].Equals(OP.CloseParam))
                        {
                            Variable a = new Variable(idvar++, Tokens[pointer].ToString(), Tokens[pointer + 1].ToString());
                            method.Variables.Add(a);
                            method.NumOfParams++;
                            pointer += 2;
                        }
                    }
                    Methods.Add(method.Name, method);
                }
            }
        }
    }
}
