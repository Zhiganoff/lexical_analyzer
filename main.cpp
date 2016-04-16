#include <iostream>
#include <string.h>

using namespace std;

enum type_of_lex
{
    LEX_NULL, // 0
    LEX_AND, // 1
    LEX_BEGIN, // 2
    LEX_BOOL, // 3
    LEX_DO, // 4
    LEX_ELSE, // 5
    LEX_END, // 6
    LEX_IF, // 7
    LEX_FALSE, // 8
    LEX_INT, // 9
    LEX_NOT, // 10
    LEX_OR, // 11
    LEX_PROGRAM, // 12
    LEX_READ, // 13
    LEX_THEN, // 14
    LEX_TRUE, // 15
    LEX_VAR, // 16
    LEX_WHILE, // 17
    LEX_WRITE, // 18
    LEX_FIN, // 19
    LEX_SEMICOLON, // 20
    LEX_COMMA, // 21
    LEX_COLON, // 22
    LEX_ASSIGN, // 23
    LEX_LPAREN, // 24
    LEX_RPAREN, // 25
    LEX_EQ, // 26
    LEX_LSS, // 27
    LEX_GTR, // 28
    LEX_PLUS, // 29
    LEX_MINUS, // 30
    LEX_TIMES, // 31
    LEX_SLASH, // 32
    LEX_LEQ, // 33
    LEX_NEQ, // 34
    LEX_GEQ, // 35
    LEX_NUM, // 36
    LEX_ID, // 37
    POLIZ_LABEL, // 38
    POLIZ_ADDRESS, // 39
    POLIZ_GO, // 40
    POLIZ_FGO,
    PRE_DEFINE,
    PRE_UNDEF,
    PRE_IFNDEF,
    PRE_ELSE,
};

class Lex
{
    type_of_lex t_lex;
    int v_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0)
    {
        t_lex = t; v_lex = v;
    }
    type_of_lex get_type () { return t_lex; }
    int get_value () { return v_lex; }
    friend ostream& operator << ( ostream &s, Lex l )
    {
        s << '(' << l.t_lex << ',' << l.v_lex << ");";
        return s;
    }
};

class Ident
{
    char * name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident ()
    {
        declare = false;
        assign = false;
    }
    char *get_name ()
    {
        return name;
    }
    void put_name (const char *n)
    {
        name = new char [ strlen(n) + 1 ];
        strcpy ( name, n );
    }
    bool get_declare ()
    {
        return declare;
    }
    void put_declare ()
    {
        declare = true;
    }
    type_of_lex get_type ()
    {
        return type;
    }
    void put_type ( type_of_lex t )
    {
        type = t;
    }
    bool get_assign ()
    {
        return assign;
    }
    void put_assign ()
    {
        assign = true;
    }
    int get_value ()
    {
        return value;
    }
    void put_value (int v)
    {
        value = v;
    }
};

class tabl_ident {
    Ident *p;
    int size;
    int top;
public:
    tabl_ident(int max_size) {
        p = new Ident[size = max_size];
        top = 1;
    }

    ~tabl_ident() {
        delete[]p;
    }

    Ident &operator[](int k) {
        return p[k];
    }

    int put(const char *buf);
};

int tabl_ident::put(const char *buf) {
    for (int j = 1; j < top; ++j)
        if (!strcmp(buf, p[j].get_name()))
            return j;
    p[top].put_name(buf);
    ++top;
    return top - 1;
}

class Scanner
{
    enum state { H, PREPROC, IDENT, NUMB, COM, ALE, DELIM, NEQ };
    static char * TW[];
    static type_of_lex words[];
    static char * TD[];
    static type_of_lex dlms[];
    state CS;
    FILE * fp;
    char c;
    char buf[80];
    int buf_top;
    void clear ()
    {
        buf_top = 0;
        for ( int j = 0; j < 80; ++j )
            buf[j] = '\0';
    }
    void add ()
    {
        buf [ buf_top ++ ] = c;
    }
    int look ( const char *buf, char **list )
    {
        int i = 0;
        while ( list[i] )
        {
            if ( !strcmp(buf, list[i]) )
                return i;
            ++i;
        }
        return 0;
    }
    void gc ()
    {
        c = fgetc (fp);
    }
public:
    Lex get_lex ();
    Scanner ( const char * program )
    {
        fp = fopen ( program, "r" );
        CS = H;
        clear();
        gc();
    }
};

char * Scanner::TW[] = {
        "", // 0 позиция 0 не используется
        "and", // 1
        "begin", // 2
        "bool", // 3
        "do", // 4
        "else", // 5
        "end", // 6
        "if", // 7
        "false", // 8
        "int", // 9
        "not", // 10
        "or", // 11
        "program", // 12
        "read", // 13
        "then", // 14
        "true", // 15
        "var", // 16
        "while", // 17
        "write", // 18
        "#define", // 19
        "#undef", // 20
        "#ifdef", // 21
        "ifndef", // 22
        NULL,
};

char * Scanner:: TD[] = {
        "", // 0 позиция 0 не используется
        "@", // 1
        ";", // 2
        ",", // 3
        ":", // 4
        ":=", // 5
        "(", // 6
        ")", // 7
        "=", // 8
        "<", // 9
        ">", // 10
        "+", // 11
        "-", // 12
        "*", // 13
        "/", // 14
        "<=", // 15
        "!=", // 16
        ">=", // 17
        NULL
};

tabl_ident TID(100);

type_of_lex Scanner::words[] = {
        LEX_NULL,
        LEX_AND,
        LEX_BEGIN,
        LEX_BOOL,
        LEX_DO,
        LEX_ELSE,
        LEX_END,
        LEX_IF,
        LEX_FALSE,
        LEX_INT,
        LEX_NOT,
        LEX_OR,
        LEX_PROGRAM,
        LEX_READ,
        LEX_THEN,
        LEX_TRUE,
        LEX_VAR,
        LEX_WHILE,
        LEX_WRITE,
        PRE_DEFINE,
        PRE_UNDEF,
        PRE_IFNDEF,
        PRE_ELSE,
        LEX_NULL
};

type_of_lex Scanner::dlms[] = {
        LEX_NULL,
        LEX_FIN,
        LEX_SEMICOLON,
        LEX_COMMA,
        LEX_COLON,
        LEX_ASSIGN,
        LEX_LPAREN,
        LEX_RPAREN,
        LEX_EQ,
        LEX_LSS,
        LEX_GTR,
        LEX_PLUS,
        LEX_MINUS,
        LEX_TIMES,
        LEX_SLASH,
        LEX_LEQ,
        LEX_NEQ,
        LEX_GEQ,
        LEX_NULL
};

Lex Scanner::get_lex ()
{
    int d, j;
    CS = H;
    do
    {
        switch ( CS )
        {
            case H:
                if ( c ==' ' || c =='\n' || c=='\r' || c =='\t' )
                    gc ();
                else if ( c == '#') {
                    clear();
                    add ();
                    gc ();
                    CS = PREPROC;
                }
                else if ( isalpha(c) )
                {
                    clear ();
                    add ();
                    gc ();
                    CS = IDENT;
                }
                else if ( isdigit(c) )
                {
                    d = c - '0';
                    gc ();
                    CS = NUMB;
                }
                else if ( c== '{' )
                {
                    gc ();
                    CS = COM;
                }
                else if ( c== ':' || c== '<' || c== '>')
                {
                    clear ();
                    add ();
                    gc ();
                    CS = ALE;
                }
                else if ( c == '@' )
                    return Lex(LEX_FIN);
                else if ( c == '!' )
        {
            clear ();
            add ();
            gc ();
            CS = NEQ;
        }
        else
            CS = DELIM;
                break;
            case IDENT:
                if ( isalpha(c) || isdigit(c) )
                {
                    add ();
                    gc ();
                }
                else
                if ( j = look (buf, TW) )
                    return Lex (words[j], j);
                else
                {
                    j = TID.put(buf);
                    return Lex (LEX_ID, j);
                }
                break;
            case NUMB:
                if ( isdigit(c) )
                {
                    d = d * 10 + (c - '0');
                    gc();
                }
                else
                    return Lex ( LEX_NUM, d );
                break;
            case COM:
                if ( c == '}' )
                {
                    gc ();
                    CS = H;
                }
                else if (c == '@' || c == '{' )
                    throw c;
                else
                    gc ();
                break;
            case ALE:
                if ( c == '=' )
                {
                    add ();
                    gc ();
                    j = look ( buf, TD );
                    return Lex ( dlms[j], j );
                }
                else
                {
                    j = look (buf, TD);
                    return Lex ( dlms[j], j );
                }
                break;
            case NEQ:
                if ( c == '=' )
                {
                    add ();
                    gc ();
                    j = look ( buf, TD );
                    return Lex ( LEX_NEQ, j );
                }
                else
                    throw '!';
                break;
            case DELIM:
                clear ();
                add ();
                if ( j = look(buf, TD) )
                {
                    gc ();
                    return Lex ( dlms[j], j );
                }
                else
                    throw c;
                break;
            case PREPROC:
                if ( isalpha(c) ) {

                }
                break;
        } // end switch
    }
    while ( true );
}

int main() {
    Scanner my_scan("test.txt");
    Lex my_lex;
    cout << "hello" << endl;
    my_lex = my_scan.get_lex();
    cout << my_lex.get_type() << ' ' << my_lex.get_value() << endl;
    return 0;
}