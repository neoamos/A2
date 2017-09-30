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

token scan() {
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */

    /* skip white space */
    while (isspace(c)) {
        //c = getchar();
        //cin >> c;
        c = cin.get();
    }
    if (cin.eof())
        return t_eof;
    if (isalpha(c)) {
        do {
            token_image[i++] = c;
            c = cin.get();
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
            c = cin.get();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = cin.get()) != '=') {
                //fprintf(stderr, "error\n");
                cerr << "error" << endl;
                exit(1);
            } else {
                //c = getchar();
                c = cin.get();
                return t_gets;
            }
            break;
        case '+': c = cin.get(); return t_add;
        case '-': c = cin.get(); return t_sub;
        case '*': c = cin.get(); return t_mul;
        case '/': c = cin.get(); return t_div;
        case '(': c = cin.get(); return t_lparen;
        case ')': c = cin.get(); return t_rparen;

        case '=': 
            if ((c = cin.get()) != '=') {
                cerr << "error" << endl;
                exit(1);
            } else {
                c = cin.get();
                return t_eql;
            }
            break;
        case '<': 
            if ((c = cin.get()) == '=') {
                c = cin.get();
                return t_leql;
            } else if (c == '>') {
                c = cin.get();
                return t_neql; 
            } else {
                return t_less;
            }
            break;
        case '>':
            if ((c = cin.get()) == '=') {
                c = cin.get();
                return t_meql;
            } else {
                return t_more;
            }
            break;
        default:
            //printf("error\n");
            cout << "error" << endl;
            exit(1);
    }
}
