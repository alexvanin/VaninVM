using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CompilerVVM
{
    public class ScannerException : Exception
    {
        public ScannerException(string message)
            : base(message)
        {
            ;
        }
    }

    public class ProcedureException : Exception
    {
        public ProcedureException(string message)
            : base(message)
        {
            ;
        }
    }

    public class ParserException : Exception
    { 
         public ParserException(string message)
            : base(message)
        {
            ;
        }
    }
}
