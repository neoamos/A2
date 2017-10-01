/* Simple ad-hoc scanner for the calculator language.
    Michael L. Scott, 2008-2017.
*/

#include <cstring>
#include <cctype>
#include <iostream>
#include <cstdlib>
using namespace std;

#include "scan.h"

char token_image[100];
int ln = 1;
int col = 1;
int next_ln = 1;
int next_col = 1;

char get_next(){
  char n = cin.get();
  if(n == '\n'){
    next_ln += 1;
    next_col = 1;
  }else{
    next_col += 1;
  }
  return n;
}

token lexical_error(){
  cout << "Lexical error.  Restarting scanner" << endl;;
  return scan();
}

token scan() {
    col = next_col;
    ln = next_ln;
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */

    /* skip white space */
    while (isspace(c)) {
        //c = getchar();
        //cin >> c;
        c = get_next();
    }
    if (cin.eof())
        return t_eof;
    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            c = get_next();
        } while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return t_read;
        else if (!strcmp(token_image, "write")) return t_write;
        else if (!strcmp(token_image, "if")) return t_if;
        else if (!strcmp(token_image, "fi")) return t_fi;
        else if (!strcmp(token_image, "do")) return t_do;
        else if (!strcmp(token_image, "od")) return t_od;
        else if (!strcmp(token_image, "check")) return t_check;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            token_image[i++] = c;
            c = get_next();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = get_next()) != '=') {
                //fprintf(stderr, "error\n");
                return lexical_error();
            } else {
                //c = getchar();
                c = get_next();
                return t_gets;
            }
            break;
        case '+': c = get_next(); return t_add;
        case '-': c = get_next(); return t_sub;
        case '*': c = get_next(); return t_mul;
        case '/': c = get_next(); return t_div;
        case '(': c = get_next(); return t_lparen;
        case ')': c = get_next(); return t_rparen;

        case '=':
            if ((c = get_next()) != '=') {
                return lexical_error();
            } else {
                c = get_next();
                return t_eql;
            }
            break;
        case '<':
            if ((c = get_next()) == '=') {
                c = get_next();
                return t_leql;
            } else if (c == '>') {
                c = get_next();
                return t_neql;
            } else {
                return t_less;
            }
            break;
        case '>':
            if ((c = get_next()) == '=') {
                c = get_next();
                return t_meql;
            } else {
                return t_more;
            }
            break;
        default:
            return lexical_error();
    }
}
