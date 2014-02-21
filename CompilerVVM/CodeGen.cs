using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CompilerVVM
{
    class CodeGen
    {
        Dictionary<string, string> ConstData;
        Dictionary<string, MethodAtom> Methods;
        public string[] asm { get; set; }


        public CodeGen(Dictionary<string, string> ConstData, Dictionary<string, MethodAtom> Methods)
        {
            this.ConstData = ConstData;
            this.Methods = Methods;
            asm = null;

            foreach (KeyValuePair<string, MethodAtom> p in Methods)
            {
                Optimization(p.Value);
            }


            FillAsmCode();
        }

        private void FillAsmCode()
        {
            List<string> Code = new List<string>();
            if (ConstData.Count > 0)
            {
                Code.Add(".data");
                foreach (KeyValuePair<string, string> pair in ConstData)
                {
                    Code.Add(string.Format("{0} \"{1}\"", pair.Value, pair.Key));
                }
                Code.Add(".endd");
            }

            foreach (KeyValuePair<string, MethodAtom> entry in Methods)
            {
                for (int i = 0; i < entry.Value.Code.Count; i++)
                    Code.Add(entry.Value.Code[i]);
            }

            asm = Code.ToArray();
        }

        private void Optimization(MethodAtom Method)
        {
            List<string> OptimizationSub = new List<string>(){"LOADDVAR", "LOADIVAR", "LOADSVAR", "STOREDVAR", "STOREIVAR",
                            "STORESVAR"};

            for (int i = 0; i < Method.Code.Count; i++ )
            {
                string[] parts = Method.Code[i].Split(' ');
                if (OptimizationSub.Contains(parts[0]) && int.Parse(parts[1]) < 4)
                {
                    Method.Code[i] = Method.Code[i].Replace(" ", "");
                }
            }
        }
    }
}
