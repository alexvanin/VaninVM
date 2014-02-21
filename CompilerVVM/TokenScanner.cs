using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Globalization;

namespace CompilerVVM
{
    class TokenScanner
    {
        int idconst = 1;
        private IList<object> tokens;
        private IList<object> list;
        public IList<object> Tokens { get { return tokens; } }
        private void Scan(System.IO.TextReader input, Dictionary<string, string> dict)
        {
            while (input.Peek() != -1)
            {
                char ch = (char)input.Peek();

                if (char.IsWhiteSpace(ch))
                {
                    input.Read();
                }

                else if (char.IsLetter(ch) || ch == '_')
                {
                    StringBuilder accum = new StringBuilder();

                    while (char.IsLetter(ch) || ch == '_' || char.IsNumber(ch))
                    {
                        accum.Append(ch);
                        input.Read();

                        if (input.Peek() == -1)
                        {
                            break;
                        }
                        else
                        {
                            ch = (char)input.Peek();
                        }
                    }
                    this.tokens.Add(accum.ToString());
                }
                else if (ch == '"')
                {
                    StringBuilder accum = new StringBuilder();

                    input.Read();

                    if (input.Peek() == -1)
                    {
                        throw new ScannerException("Unterminated string");
                    }

                    while ((ch = (char)input.Peek()) != '"')
                    {
                        accum.Append(ch);
                        input.Read();

                        if (input.Peek() == -1)
                        {
                            throw new ScannerException("Unterminated string");
                        }
                    }

                    input.Read();
                    dict.Add(accum.ToString(), string.Format("text_const_{0}", idconst++));
                    this.tokens.Add(accum);
                }
                else if (char.IsDigit(ch))
                {
                    StringBuilder accum = new StringBuilder();

                    while (char.IsDigit(ch) || ch == '.')
                    {
                        accum.Append(ch);
                        input.Read();

                        if (input.Peek() == -1)
                        {
                            break;
                        }
                        else
                        {
                            ch = (char)input.Peek();
                        }
                    }
                    this.tokens.Add(new Number(accum.ToString()));
                }
                else if (ch == '(')
                {
                    this.tokens.Add(OP.OpenParam);
                    input.Read();

                    if (input.Peek() == -1)
                    {
                        throw new ScannerException("Unterminated parameter section");
                    }

                    while ((ch = (char)input.Peek()) != ')')
                    {
                        if (char.IsLetter(ch) || ch == '_')
                        {
                            StringBuilder accum = new StringBuilder();

                            while (char.IsLetter(ch) || ch == '_')
                            {
                                accum.Append(ch);
                                input.Read();

                                if (input.Peek() == -1)
                                {
                                    break;
                                }
                                else
                                {
                                    ch = (char)input.Peek();
                                }
                            }
                            this.tokens.Add(accum.ToString());
                        }
                        else if (char.IsDigit(ch))
                        {
                            StringBuilder accum = new StringBuilder();

                            while (char.IsDigit(ch) || ch == '.')
                            {
                                accum.Append(ch);
                                input.Read();

                                if (input.Peek() == -1)
                                {
                                    break;
                                }
                                else
                                {
                                    ch = (char)input.Peek();
                                }
                            }
                            this.tokens.Add(new Number(accum.ToString()));
                        }
                        else if (ch == '"')
                        {
                            StringBuilder accum = new StringBuilder();

                            input.Read();

                            if (input.Peek() == -1)
                            {
                                throw new ScannerException("Unterminated string");
                            }

                            while ((ch = (char)input.Peek()) != '"')
                            {
                                accum.Append(ch);
                                input.Read();

                                if (input.Peek() == -1)
                                {
                                    throw new ScannerException("Unterminated string");
                                }
                            }

                            input.Read();
                            dict.Add(accum.ToString(), string.Format("text_const_{0}", idconst++));
                            this.tokens.Add(accum);
                        }
                        else if (char.IsWhiteSpace(ch) || ch == ',')
                            input.Read();
                        else switch (ch)
                            {
                                case '=':
                                    input.Read();
                                    if ((char)input.Peek() == '=')
                                    {
                                        input.Read();
                                        this.tokens.Add(OP.Equal);
                                    }
                                    else this.tokens.Add(OP.Assigment);
                                    break;
                                case '<':
                                    input.Read();
                                    if ((char)input.Peek() == '=')
                                    {
                                        input.Read();
                                        this.tokens.Add(OP.LessEqual);
                                    }
                                    else this.tokens.Add(OP.Less);
                                    break;
                                case '>':
                                    input.Read();
                                    if ((char)input.Peek() == '=')
                                    {
                                        input.Read();
                                        this.tokens.Add(OP.GreaterEqual);
                                    }
                                    else this.tokens.Add(OP.Greater);
                                    break;
                                case '!':
                                    input.Read();
                                    if ((char)input.Peek() == '=')
                                    {
                                        input.Read();
                                        this.tokens.Add(OP.NotEqual);
                                    }
                                    else throw new Exception("!");
                                    break;
                            }
                    }
                    this.tokens.Add(OP.CloseParam);
                    input.Read();
                }
                else switch (ch)
                    {
                        case ';':
                            input.Read();
                            this.tokens.Add(OP.Semicolon);
                            break;
                        case '{':
                            input.Read();
                            this.tokens.Add(OP.OpenBlock);
                            break;
                        case '}':
                            input.Read();
                            this.tokens.Add(OP.CloseBlock);
                            break;
                        case '+':
                            input.Read();
                            this.tokens.Add(OP.Add);
                            break;
                        case '-':
                            input.Read();
                            this.tokens.Add(OP.Sub);
                            break;
                        case '*':
                            input.Read();
                            this.tokens.Add(OP.Mul);
                            break;
                        case '/':
                            input.Read();
                            this.tokens.Add(OP.Div);
                            break;
                        case '%':
                            input.Read();
                            this.tokens.Add(OP.Mod);
                            break;
                        case '=':
                            input.Read();
                            if ((char)input.Peek() == '=')
                            {
                                input.Read();
                                this.tokens.Add(OP.Equal);
                            }
                            else this.tokens.Add(OP.Assigment);
                            break;
                        default:
                            throw new ScannerException("Scanner encountered unrecognized character '" + ch + "'");
                    }
            }
        }

        public TokenScanner(TextReader input, Dictionary<string, string> TextConstant)
        {
            tokens = new List<object>();
            this.Scan(input, TextConstant);
        }
    }

    enum OP
    {
        Semicolon,
        OpenBlock,
        CloseBlock,
        OpenParam,
        CloseParam,
        Assigment,
        Equal,
        NotEqual,
        Less,
        Greater,
        LessEqual,
        GreaterEqual,
        Add,
        Sub,
        Mul,
        Div,
        Mod
    }
}
