using System;
using System.Collections.Generic;
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
                    ByteCode.GenerateByteCode(code.asm, args[0] + ".vvm");
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
