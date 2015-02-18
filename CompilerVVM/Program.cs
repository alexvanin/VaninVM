using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CompilerVVM
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 1)
            {
                Console.WriteLine("No input file");
                return;
            }
            try
            {
                /*
                 * This program translate source code into assembly instructions
                 * This can be done in several steps:
                 *  1. Compiler finds tokens (individual parts such as text, spec. symbols, numbers etc);
                 *  2. Compiler goes through tokens and defines functions
                 *  3. Compiler parse every function into assembler instruction
                  */
                TokenScanner scanner = null;
                ProcedureScanner procscanner = null;
                Parser parser = null;
                CodeGen code = null;
                Dictionary<string, string> TextConst = new Dictionary<string, string>();
                using (System.IO.TextReader input = System.IO.File.OpenText(args[0]))
                {
                    scanner = new TokenScanner(input, TextConst);
                    procscanner = new ProcedureScanner(scanner.Tokens);
                    parser = new Parser(scanner.Tokens, TextConst, procscanner.Methods);
                    
                    code = new CodeGen(TextConst, procscanner.Methods);
                    //ByteCode.GenerateByteCode(code.asm, args[0] + ".vvm");
                    System.IO.File.WriteAllLines(args[0] + ".vasm", code.asm);

                    return;
                }
            }
            catch (Exception e)
            {
                Console.Error.WriteLine(e.Message);
            }
            
        }
    }
}
