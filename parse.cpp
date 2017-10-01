/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2017.
*/

#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

#include "scan.h"


const string names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof",
                        "eql", "neql", "less", "more", "leql", "meql",
                        "if", "fi", "do", "od", "check"};
const string symbols[] = {"read", "write", "id", "literal", "gets",
                       "+", "-", "*", "/", "(", ")", "$$",
                        "==", "!=", "<", ">", "<=", ">=",
                        "if", "fi", "do", "od", "check"};

static token input_token;
bool assignId = false;
string arg1;

string token_string(token t){
  string s = "\"" + names[t] + "\"";
  if(t == t_id || t == t_literal){
    s = s + " (" + token_image + ")";
  }
  return s;
}


void error (token unexpected_token) {
    cerr << "Syntax error: unexpected token " +token_string(unexpected_token) + " on line ";
    cerr << ln;
    cerr << ", col ";
    cerr << col << endl;
    string e = "Syntax error: unexpected token " + token_string(unexpected_token);
    throw e;
    //exit (1);
}

void match (token expected) {
    if (input_token == expected) {
        //cout << "matched " << names[input_token];
        //if (input_token == t_id  || input_token == t_literal)
            //printf (": %s", token_image);
            //cout << "\"" << token_image << "\"";
        //printf ("\n");
        //cout << endl;
        input_token = scan ();
    }else{
      cerr << "Syntax error: received an " +token_string(input_token) + " when expecting an " + token_string(expected) + " on line ";
      cerr << ln;
      cerr << ", col ";
      cerr << col << endl;
      throw "Syntax error: received an "+ token_string(input_token) +" when expecting an " + token_string(expected);
    }
}

void program ();
void stmt_list ();
void stmt ();
string expr ();
string term_tail (string st);
string term ();
string factor_tail (string st);
string factor ();
string add_op ();
string mul_op ();

string rln ();
string rln_op ();
string expr_tail (string st);

void program () {
    try{
        switch (input_token) {
            case t_id:
            case t_read:
            case t_write:
            case t_if:
            case t_do:
            case t_check:
            case t_eof:
                //cout << "predict program --> stmt_list eof" << endl;
                cout << "(program" << endl;
                cout << "[ ";
                stmt_list ();
                match (t_eof);
                cout << "]";
                cout << "\n)" << endl;
                break;
            default: error (input_token);
        }
    }catch(const string e){
      while(1){
        if(input_token==t_id ||
          input_token==t_read ||
          input_token==t_write ||
          input_token==t_if ||
          input_token==t_do ||
          input_token==t_check){
          program();
          return;
        }else if(
          input_token==t_eof
          ){
          return;
        }else{
          input_token = scan();
        }
      }
    }
}

void stmt_list () {
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
            //cout << "predict stmt_list --> stmt stmt_list" << endl;
            stmt ();
            stmt_list ();
            break;
        case t_fi:
        case t_od:
        case t_eof:
            //cout << "predict stmt_list --> epsilon" << endl;
            break;          /*  epsilon production */
        default: error (input_token);
    }
}

void stmt () {
  try{
    switch (input_token) {
        case t_id:
            //cout << "predict stmt --> id gets rln" << endl;
            cout << "(:= ";
            assignId = true;
            cout << "\"" << token_image << "\" ";
            match (t_id);
            match (t_gets);
            cout << rln ();
            cout << ")" << endl;
            break;
        case t_read:
            //cout <<  "predict stmt --> read id" << endl;
            cout << "(read ";
            match (t_read);
            cout << "\"" << token_image << "\"";
            match (t_id);
            cout << ")" << endl;
            break;
        case t_write:
            //cout <<  "predict stmt --> write rln" << endl;
            cout << "(write ";
            match (t_write);
            cout << rln ();
            cout << ")" << endl;
            break;
        case t_if:
            //cout << "predict stmt --> if rln stmt_list fi" <<endl;
            cout << "(if " << endl;
            match (t_if);
            cout << "(";
            cout << rln ();
            cout << ")";
            cout << "\n[ ";
            stmt_list ();
            cout << "]" << endl;
            match (t_fi);
            cout << ")" << endl;
            break;
        case t_do:
            //cout << "predict stmt --> do stmt_list od" <<endl;
            cout << "(do " << endl;
            match (t_do);
            cout << "[ ";
            stmt_list ();
            cout << "]" << endl;
            match (t_od);
            cout << ")" << endl;
            break;
        case t_check:
            //cout << "predict stmt --> check rln" <<endl;
            cout << "(check ";
            match (t_check);
            cout << rln ();
            cout << ")" << endl;
            break;
        default: error (input_token);
    }
  }catch(const string e){
    while(1){
      if(input_token==t_id ||
        input_token==t_read ||
        input_token==t_write ||
        input_token==t_if ||
        input_token==t_do ||
        input_token==t_check){
        stmt();
        return;
      }else if(
        input_token==t_eof ||
        input_token==t_od ||
        input_token==t_fi
        ){
        return;
      }else{
        input_token = scan();
      }
    }
  }
}

string expr () {
  try{
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:{
            //cout << "predict expr --> term term_tail" << endl;
            string term_str = term ();
            string expr_str = term_tail (term_str);
            return expr_str;
            break;}
        default: error (input_token);
    }
  }catch(const string e){
    while(1){
      if(input_token==t_lparen ||
        input_token==t_id ||
        input_token==t_literal){
          expr();
          return "";
      }else if(
        input_token==t_rparen ||
        input_token==t_read ||
        input_token==t_write ||
        input_token==t_if ||
        input_token==t_do ||
        input_token==t_check ||
        input_token==t_fi ||
        input_token==t_od ||
        input_token==t_eof ||
        input_token==t_eql ||
        input_token==t_neql ||
        input_token==t_less ||
        input_token==t_more ||
        input_token==t_leql ||
        input_token==t_meql
      ){
        return "";
      }else{
        input_token = scan();
      }
    }
  }
}

string expr_tail (string st) {
    switch (input_token) {
        case t_eql:
        case t_neql:
        case t_less:
        case t_more:
        case t_leql:
        case t_meql:{
            //cout << "predict expr_tail --> rln_op expr" << endl;
            string op_str = rln_op ();
            string expr_str = expr ();
            expr_str = op_str + " " + st + " " + expr_str;
            return expr_str;
            break;}
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            //cout << "predict expr_tail --> epsilon" << endl;
            return st;
            break;          /*  epsilon production */
        default: error (input_token);
    }

}

string rln () {
  try{
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:{
            //cout << "predict expr --> expr expr_tail" << endl;
            string expr_str = expr ();
            string rln_str = expr_tail (expr_str);
            return rln_str;
            break;}
        default: error (input_token);
    }
  }catch(const string e){
    while(1){
      if(input_token==t_lparen ||
        input_token==t_id ||
        input_token==t_literal){
          rln();
          return "";
      }else if(
        input_token==t_rparen ||
        input_token==t_read ||
        input_token==t_write ||
        input_token==t_if ||
        input_token==t_do ||
        input_token==t_check ||
        input_token==t_fi ||
        input_token==t_od ||
        input_token==t_eof
      ){
        return "";
      }else{
        input_token = scan();
      }
    }
  }
}

string term_tail (string st) {
    switch (input_token) {
        case t_add:
        case t_sub:{
            //cout << "predict term_tail --> add_op term term_tail" << endl;
            string op_str = add_op ();
            string term_str = term ();
            term_str = "(" + op_str + " " + st + " " + term_str + ")";
            term_str = term_tail (term_str);
            return term_str;
            break;}
        case t_eql:
        case t_neql:
        case t_less:
        case t_more:
        case t_leql:
        case t_meql:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            //cout << "predict term_tail --> epsilon" << endl;
            return st;
            break;          /*  epsilon production */
        default: error (input_token);
    }
}

string term () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:{
            //cout << "predict term --> factor factor_tail" << endl;
            string factor_str = factor ();
            string term_str = factor_tail (factor_str);
            return term_str;
            break;}
        default: error (input_token);
    }
}

string factor_tail (string st) {
    switch (input_token) {
        case t_mul:
        case t_div:{
            //cout << "predict factor_tail --> mul_op factor factor_tail" << endl;
            string op_str = mul_op ();
            string factor_str = factor ();
            factor_str = "(" + op_str + " " + st + " " + factor_str + ")";
            factor_str = factor_tail (factor_str);
            return factor_str;
            break;}
        case t_add:
        case t_sub:
        case t_eql:
        case t_neql:
        case t_less:
        case t_more:
        case t_leql:
        case t_meql:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
        case t_fi:
        case t_od:
        case t_eof:
            //cout << "predict factor_tail --> epsilon" << endl;
            return st;
            break;          /*  epsilon production */
        default: error (input_token);
    }
}

string factor () {
    switch (input_token) {
        case t_id :{
            //cout << "predict factor --> id"  << endl;
            string factor_str = "(id \"" + string(token_image) + "\")";
            match (t_id);
            return factor_str;
            break;}
        case t_literal:{
            //cout << "predict factor --> literal"  << endl;
            string factor_str = "(num \"" +  string(token_image) + "\")";
            match (t_literal);
            return factor_str;
            break;}
        case t_lparen:{
            //cout << "predict factor --> lparen expr rparen"  << endl;
            match (t_lparen);
            string rln_str = rln ();
            match (t_rparen);
            return rln_str;
            break;}
        default: error (input_token);
    }
}

string add_op () {
    switch (input_token) {
        case t_add:{
            //cout << "predict add_op --> add" << endl;
            string op_str = symbols[input_token];
            match (t_add);
            return op_str;
            break;}
        case t_sub:{
            //cout << "predict add_op --> sub" << endl;
            string op_str = symbols[input_token];;
            match (t_sub);
            return op_str;
            break;}
        default: error (input_token);
    }
}

string mul_op () {
    switch (input_token) {
        case t_mul:{
            //cout << "predict mul_op --> mul" << endl;
            string op_str = symbols[input_token];;
            match (t_mul);
            return op_str;
            break;}
        case t_div:{
            //cout << "predict mul_op --> div\n" << endl;
            string op_str = symbols[input_token];;
            match (t_div);
            return op_str;
            break;}
        default: error (input_token);
    }
}

string rln_op () {
    switch (input_token) {
        case t_eql:{
            //cout << "predict rln_op --> eql" << endl;
            string op_str = symbols[input_token];;
            match (t_eql);
            return op_str;
            break;}
        case t_neql:{
            //cout << "predict rln_op --> neql" << endl;
            string op_str = symbols[input_token];;
            match (t_neql);
            return op_str;
            break;}
        case t_less:{
            //cout << "predict rln_op --> t_less" << endl;
            string op_str = symbols[input_token];;
            match (t_less);
            return op_str;
            break;}
        case t_more:{
            //cout << "predict rln_op --> t_more" <<endl;
            string op_str = symbols[input_token];;
            match (t_more);
            return op_str;
            break;}
        case t_leql:{
            //cout << "predict rln_op --> leql" << endl;
            string op_str = symbols[input_token];;
            match (t_leql);
            return op_str;
            break;}
        case t_meql:{
            //cout << "predict rln_op --> meql" << endl;
            string op_str = symbols[input_token];;
            match (t_meql);
            return op_str;
            break;}
        default: error (input_token);
    }
}

int main () {
    input_token = scan ();
    program ();
    return 0;
}
